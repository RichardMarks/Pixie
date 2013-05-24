//*** PixieAGT.cpp ***

#include "PixieAGT.h"

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

// Forward declares
void ProgressReport(ArchiveGenerator::ProgressData& data);
void DeflateArchive(const char* arg);
void CreateArchive(const char* arg, bool cpp);
void DeflateDirectory(const char* rootPath, Archive* archive, const ArchiveDirectory* directory);

int rootPathLength=0;

//*** PixieAGTMain ***

void PixieAGTMain(const char* commandLine)
	{
	CommandLine cmd(commandLine);

	printf("Pixie Archive Generation Tool 1.0\n\n");

	// Get the full path
	if (cmd.GetCommandLineStringCount()!=2)
		{
		printf("SYNTAX: PixieAGT path\n\n");
		return;
		}

	// Determine if the input argument is a folder that we should make an archive for, or an archive to deflate
	bool deflate=false;
	Platform_FileSystem_Directory* dir=Platform::GetPlatform_FileSystem()->CreateDirectoryObject(cmd.GetCommandLineString(1).GetString());
	if (!dir->Exists())
		{
		// If the argument isn't an existing directory, it might be a file
		Platform_FileSystem_File* file=Platform::GetPlatform_FileSystem()->CreateFileObject(cmd.GetCommandLineString(1).GetString());
		if (file->Exists())
			{
			// Yes, it's a file, so check that it's a Pixie archive file
			if (!file->Open())
				{
				delete file;
				printf("The specified file could not be opened.\n\n");
				return;
				}
			char buf[16];
			file->Read(buf,16);
			file->Close();
			delete file;
			if (StrNCmp(buf,"PIXIE_ARCHIVE",13)!=0)
				{
				printf("The specified file is not a valid Pixie archive.\n\n");
				return;
				}
			deflate=true;
			}
		else
			{
			// Nope, not an existing file or folder, so we can't proces sit
			delete dir;
			printf("Specified file/folder does not exist\n\n");
			return;
			}
		}
	delete dir;
	
	
	if (deflate) // Deflate the archive (restore all the files stored in it)
		{
		printf("Deflating archive: %s\n\n",cmd.GetCommandLineString(1).GetString());
		DeflateArchive(cmd.GetCommandLineString(1).GetString());
		}
	
	else // Generate an archive from the folder
		{
		printf("Creating archive from folder: %s\n\n",cmd.GetCommandLineString(1).GetString());
		CreateArchive(cmd.GetCommandLineString(1).GetString(), cmd.IsCommandLineFlagSet("cpp"));
		}
	}


//*** CreateDirectory ***

void CreateArchive(const char* arg, bool cpp)
	{
	char* path=StrDup(arg);
	int len=StrLen(path);

	// Remove trailing '/' or '\'
	if (path[len-1]=='\\' || path[len-1]=='/')
		{
		path[len-1]=0;
		len--;
		}
	
	// Find the part of the path which is the folder name
	const char* folderStart=StrRChr(path,'\\');
	const char* folderStart2=StrRChr(path,'/');
	if (folderStart==0 || (folderStart2 && folderStart2>folderStart))
		{
		folderStart=folderStart2;
		}
	if (folderStart>0)
		{
		folderStart++;
		}
	else
		{
		folderStart=path;
		}
	
	// Store the length of the root path, for later use (the part of the path excluding the folder name)
	rootPathLength=(int)(folderStart-path);


	// Generate archive filename (add the path only in this step - the rest will be appended later)
	char* archiveFilename=(char*)Malloc(len+5+StrLen("Archive_"));
	StrCpy(archiveFilename,path);

	ArchiveGenerator generator(ProgressReport);
	
	if (cpp) // Generate CPP array
		{
		// Generate the name of the cpp array
		char* cppName=(char*)Malloc(StrLen(folderStart)+StrLen("Archive_")+1);
		StrCpy(cppName,"Archive_");
		StrCat(cppName,folderStart);

		// Generate the full filename
		char* path2=StrDup(path);
		*((char*)folderStart)=0;
		StrCpy(archiveFilename,path);
		StrCat(archiveFilename,cppName);
		StrCat(archiveFilename,".cpp");
		
		// Generate file
		generator.GenerateCPP(path2,archiveFilename,cppName);

		// Cleanup
		Free(path2);
		Free(cppName);
		}

	else // Generate archive file
		{
		StrCat(archiveFilename,".dat");
		generator.GenerateFile(path,archiveFilename);
		}

	// Cleanup
	Free(path);
	Free(archiveFilename);
	}


//*** ProgressReport ***

void ProgressReport(ArchiveGenerator::ProgressData& data)
	{
	switch (data.phase)
		{
		case ArchiveGenerator::Phase_Initializing:
			{
			printf("\nInitializing...");
			} break;

		case ArchiveGenerator::Phase_ScanningDirectory:
			{
			printf("\nScanning directory: %s",data.text+rootPathLength);
			} break;

		case ArchiveGenerator::Phase_AddingFile:
			{
			printf("\nAdding File: %s",data.text+rootPathLength);
			} break;

		case ArchiveGenerator::Phase_CreatingHeaders:
			{
			printf("\nCreating headers");
			} break;

		case ArchiveGenerator::Phase_WritingArchive:
			{
			printf("\nWriting archive file");
			} break;

		case ArchiveGenerator::Phase_GeneratingArray:
			{
			// Back up one line, to have the progress % updated on the same line until it reaches 100
			HANDLE hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
			CONSOLE_SCREEN_BUFFER_INFO i;
			GetConsoleScreenBufferInfo(hConsole,&i);
			COORD c=i.dwCursorPosition;
			c.Y--;
			SetConsoleCursorPosition(hConsole,c);
			printf("\nGenerating array code: %s done",data.text);
			} break;

		case ArchiveGenerator::Phase_Done:
			{
			printf("\nDone!");
			} break;

		}
	}


//*** DeflateArchive ***

void DeflateArchive(const char* arg)
	{
	char* path=StrDup(arg);
	int len=StrLen(path);

	// Remove trailing '/' or '\' (if any)
	if (path[len-1]=='\\' || path[len-1]=='/')
		{
		path[len-1]=0;
		len--;
		}
	
	// Get the path (strip away the filename)
	const char* fileStart=StrRChr(path,'\\');
	const char* fileStart2=StrRChr(path,'/');
	if (fileStart==0 || (fileStart2 && fileStart2>fileStart))
		{
		fileStart=fileStart2;
		}
	if (fileStart>0)
		{
		fileStart++;
		}
	else
		{
		fileStart=path;
		}
	*(char*)fileStart=0;

	// Load archive
	Archive archive(arg);

	// Start with the root directory, which recursively deflates all subdirectories
	const ArchiveDirectory* root=archive.GetRootDirectory();
	DeflateDirectory(path, &archive, root);

	// Cleanup
	Free(path);

	printf("\nDone!\n");
	}


//*** DeflateDirectory ***

void DeflateDirectory(const char* rootPath, Archive* archive, const ArchiveDirectory* directory)
	{
	printf("\nProcessing directory: %s",directory->GetPath().GetString());

	// Create this directory 
	char* path=(char*)Malloc(StrLen(rootPath)+StrLen(directory->GetPath().GetString())+1);
	StrCpy(path,rootPath);
	StrCat(path,directory->GetPath().GetString());
	Platform_FileSystem_Directory* dir=Platform::GetPlatform_FileSystem()->CreateDirectoryObject(path);
	Free(path);
	if (!dir->Exists())
		{
		dir->Create();
		}
	delete dir;

	// Process files
	for (int i=0; i<directory->GetFileCount(); i++)
		{
		printf("\nProcessing file: %s",directory->GetFile(i).GetString());
		
		// Load file data
		ArchiveFile* file=archive->GetFile(directory->GetFile(i));
		int size=file->GetSize();
		unsigned char* buffer=new unsigned char[size];
		file->Open();
		file->Read(buffer,size);
		file->Close();
		delete file;

		// Generate full path
		char* path=(char*)Malloc(StrLen(rootPath)+StrLen(directory->GetFile(i).GetString())+1);
		StrCpy(path,rootPath);
		StrCat(path,directory->GetFile(i).GetString());

		// Save file 
		Platform_FileSystem_File* f=Platform::GetPlatform_FileSystem()->CreateFileObject(path);
		Free(path);
		f->Create();
		f->Write(buffer,size);
		f->Close();
		delete f;
		delete buffer;
		}

	// Process subdirectories
	for (int i=0; i<directory->GetSubdirectoryCount(); i++)
		{
		const ArchiveDirectory* subdir=archive->GetDirectory(directory->GetSubdirectory(i));
		DeflateDirectory(rootPath,archive,subdir);
		}
	}