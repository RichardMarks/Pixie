//*** PixiePPT.cpp ***

#include "PixiePPT.h"

#include <stdio.h>
#define WIN32_LEAN_AND_MEAN 
#define VC_EXTRALEAN
#include <windows.h>

#include "CommandLine.h"
#include "StandardLibrary.h"
#include "Filename.h"
#include "ArchiveGenerator.h"
#include "Archive.h"
#include "ArchiveDirectory.h"
#include "ArchiveFile.h"
#include "Platform_FileSystem.h"
#include "Platform_FileSystem_Directory.h"
#include "Platform_FileSystem_File.h"

#include "tinyxml2.h"

// Forward declares

//*** PixiePPTMain ***

void BackupFile(const char* srcFilename)
{
    Platform_FileSystem& fileSystem = *Platform::GetPlatform_FileSystem();
    Platform_FileSystem_File& sourceFile = *fileSystem.CreateFileObject(srcFilename);
    
    if (!sourceFile.Exists())
    {
        printf("File not found %s.\n\n", srcFilename);
        delete &sourceFile;
        return;
    }

    char* destFilename = new char[StrLen(srcFilename) + 5];
	StrCpy(destFilename, srcFilename);
	StrCat(destFilename, ".bak");

    Platform_FileSystem_File& destFile = *fileSystem.CreateFileObject(destFilename);
    
    if (!destFile.Create())
    {
        printf("An error occured trying to write to %s.\n\n", destFilename);
        destFile.Close();
        sourceFile.Close();

        delete [] destFilename;
        delete &destFile;
        delete &sourceFile;
        return;
    }

    if (!sourceFile.Open())
    {
        printf("An error occured trying to read from %s.\n\n", srcFilename);
        destFile.Close();
        sourceFile.Close();

        delete [] destFilename;
        delete &destFile;
        delete &sourceFile;
        return;
    }

    sourceFile.SetPosition(0, Platform_FileSystem_File::ORIGIN_END);
    int sourceSize = sourceFile.GetPosition();
    sourceFile.SetPosition(0, Platform_FileSystem_File::ORIGIN_START);
    
    unsigned char* sourceBuffer = new unsigned char[sourceSize];
    sourceFile.Read(sourceBuffer, sourceSize);
    destFile.Write(sourceBuffer, sourceSize);

    if (destFile.GetPosition() != sourceSize)
    {
        printf("An error occured writing %s.\n\n", destFilename);
        destFile.Close();
        sourceFile.Close();
        delete [] sourceBuffer;
        delete [] destFilename;
        delete &destFile;
        delete &sourceFile;
        return;
    }

    destFile.Close();
    sourceFile.Close();

    delete [] sourceBuffer;
    delete [] destFilename;
    delete &destFile;
    delete &sourceFile;
}

void PixiePPTMain(const char* commandLine)
{
    printf("Pixie Project Patch Tool 1.0\n\n");

	CommandLine cmd(commandLine);
	if (cmd.GetCommandLineStringCount() != 2)
	{
	    printf("SYNTAX: PixiePPT name_of_project.vcproj\n\n");
		return;
	}

    const char* sourceFilename = cmd.GetCommandLineString(1).GetString();
    BackupFile(sourceFilename);

    tinyxml2::XMLDocument doc;
    doc.LoadFile(sourceFilename);

    tinyxml2::XMLElement* vspnode = doc.FirstChildElement("VisualStudioProject");
    const char* projectname = vspnode->Attribute("Name");
    const char* projectguid = vspnode->Attribute("ProjectGUID");
    printf(
        "Reading Project Attributes...\n"
        "Name: %s\n"
        "GUID: %s\n", 
        projectname, 
        projectguid);

    // read configurations
    printf("Reading Project Configurations...");
    tinyxml2::XMLElement* configurationsnode = vspnode->FirstChildElement("Configurations");

    tinyxml2::XMLElement* cfgnodedebug = NULL;
    tinyxml2::XMLElement* cfgnoderelease = NULL;
    for (
        tinyxml2::XMLElement* configurationnode = configurationsnode->FirstChildElement("Configuration"); 
        configurationnode != NULL; 
        configurationnode = configurationnode->NextSiblingElement("Configuration"))
    {
        const char* cfgname = configurationnode->Attribute("Name");
        if (StrNCmp(cfgname, "Debug|", StrLen("Debug")) == 0)
        {
            cfgnodedebug = configurationnode;
        }
        else
        {
            cfgnoderelease = configurationnode;
        }
    }
    printf("Done.\n");

    if (cfgnodedebug != NULL)
    {
        const char* cfgname = cfgnodedebug->Attribute("Name");
        printf("Patching %s Configuration...", cfgname);

        // patch output directory
        cfgnodedebug->SetAttribute("OutputDirectory", ".\\BuildTemp\\$(ConfigurationName)");
        // patch intermediate directory
        cfgnodedebug->SetAttribute("IntermediateDirectory", ".\\BuildTemp\\$(ConfigurationName)");
        // patch character set
        cfgnodedebug->SetAttribute("CharacterSet", "2");

        // find compiler tool
        tinyxml2::XMLElement* compilertoolnode = NULL;
        for (tinyxml2::XMLElement* toolnode = cfgnodedebug->FirstChildElement("Tool");
            toolnode != NULL;
            toolnode = toolnode->NextSiblingElement("Tool"))
        {
            const char* toolname = toolnode->Attribute("Name");
            if (StrCmp(toolname, "VCCLCompilerTool") == 0)
            {
                compilertoolnode = toolnode;
                break;
            }
        }

        if (compilertoolnode != NULL)
        {
            // patch additional options
            compilertoolnode->SetAttribute("AdditionalOptions", "/wd4100");
            // patch additional includes directory
            compilertoolnode->SetAttribute("AdditionalIncludeDirectories", "..\\PixieLib\\Source\\Common");
            // patch preprocessor definitions
            compilertoolnode->SetAttribute("PreprocessorDefinitions", "WIN32;_DEBUG;_WINDOWS;_CRT_SECURE_NO_DEPRECATE;_CRT_NON_CONFORMING_SWPRINTFS");
            // patch warning level
            compilertoolnode->SetAttribute("WarningLevel", "4");
            // patch warn as error
            compilertoolnode->SetAttribute("WarnAsError", "true");
            printf("Done.\n");
        }
        else
        {
            printf("Fail.\n\nError: Unable to locate Compiler Tool Definition. Corrupted vcproj?\n*** Aborting...\n");
            return;
        }
    }
    else
    {
        printf("A Debug Configuration was not found.\n");
    }

    if (cfgnoderelease != NULL)
    {
        const char* cfgname = cfgnoderelease->Attribute("Name");
        printf("Patching %s Configuration...", cfgname);

        // patch output directory
        cfgnoderelease->SetAttribute("OutputDirectory", ".\\BuildTemp\\$(ConfigurationName)");
        // patch intermediate directory
        cfgnoderelease->SetAttribute("IntermediateDirectory", ".\\BuildTemp\\$(ConfigurationName)");
        // patch character set
        cfgnoderelease->SetAttribute("CharacterSet", "2");

        // find compiler tool
        tinyxml2::XMLElement* compilertoolnode = NULL;
        for (tinyxml2::XMLElement* toolnode = cfgnoderelease->FirstChildElement("Tool");
            toolnode != NULL;
            toolnode = toolnode->NextSiblingElement("Tool"))
        {
            const char* toolname = toolnode->Attribute("Name");
            if (StrCmp(toolname, "VCCLCompilerTool") == 0)
            {
                compilertoolnode = toolnode;
                break;
            }
        }

        if (compilertoolnode != NULL)
        {
            // patch additional options
            compilertoolnode->SetAttribute("AdditionalOptions", "/wd4100");
            // patch additional includes directory
            compilertoolnode->SetAttribute("AdditionalIncludeDirectories", "..\\PixieLib\\Source\\Common");
            // patch preprocessor definitions
            compilertoolnode->SetAttribute("PreprocessorDefinitions", "WIN32;NDEBUG;_WINDOWS;_CRT_SECURE_NO_DEPRECATE;_CRT_NON_CONFORMING_SWPRINTFS");
            // patch warning level
            compilertoolnode->SetAttribute("WarningLevel", "4");
            // patch warn as error
            compilertoolnode->SetAttribute("WarnAsError", "true");
            printf("Done.\n");
        }
        else
        {
            printf("Fail.\n\nError: Unable to locate Compiler Tool Definition. Corrupted vcproj?\n*** Aborting...\n");
            return;
        }
    }
    else
    {
        printf("A Release Configuration was not found.\n");
    }

    doc.SaveFile(sourceFilename);
    printf("Done.\n");
}