//*** ArchiveGenerator.cpp ***

#include "ArchiveGenerator.h"
#include "Debug.h"
#include "Platform_FileSystem.h"
#include "Platform_FileSystem_Directory.h"
#include "Platform_FileSystem_File.h"
#include "StandardLibrary.h"

extern const char* Pixie_Archive_Header;


//*** Constructor ***

ArchiveGenerator::ArchiveGenerator(ProgressCallback progressCallback):
	progressCallback_(progressCallback),
	filedata_(16*1024*1024)
	{
	SetProgress(Phase_NotStarted,"");
	}


//*** GenerateCPP ***

void ArchiveGenerator::GenerateCPP(const char* directoryPath, const char* archiveFilename, const char* archiveName)
	{
	SetProgress(Phase_Initializing,"");

	// Make sure structures are empty
	root_.subDirectories.Clear();
	root_.files.Clear();
	filedata_.Clear();

	// Create archive
	CreateArchive(directoryPath);

	int version=0;

	SetProgress(Phase_CreatingHeaders,"");

	// Calculate header size
	DynamicBuffer header(1024*1024);
	WriteDirectoryList(&header,&root_,0);
	int headerSize=header.GetSize();
	headerSize+=StrLen(Pixie_Archive_Header);
	headerSize+=sizeof(version);

	// Generate header and fixup offsets
	header.Clear();
	WriteDirectoryList(&header,&root_,headerSize);

	// Create output buffer
	DynamicBuffer buffer(filedata_.GetSize());
	
	// Write header
	buffer.Write(Pixie_Archive_Header,StrLen(Pixie_Archive_Header));
	buffer.Write(&version);

	// Write directory list
	buffer.Write(static_cast<unsigned char*>(header.GetPointer()),header.GetSize());

	// Write content of files
	buffer.Write(static_cast<unsigned char*>(filedata_.GetPointer()),filedata_.GetSize());

	SetProgress(Phase_GeneratingArray,"0%");

	DynamicBuffer textBuffer(buffer.GetSize()*5+buffer.GetSize()/100+1024);

	// Write array declaration
	char str[4096];
	SNPrintF(str,4096,"unsigned char %s[%d] = {\n",archiveName,buffer.GetSize());
	textBuffer.Write(str,StrLen(str));

	// Write content of buffers
	unsigned char* data=static_cast<unsigned char*>(buffer.GetPointer());
	for (unsigned int i=0; i<buffer.GetSize(); i++ )
		{
		// Write hexadecimal value
		SNPrintF(str,4096,"0x%02X,",data[i]);
		textBuffer.Write(str,StrLen(str));

		// Every 100 bytes, do a line break
		if (i%100==99)
			{
			textBuffer.Write("\n");
			}

		static int lastPercentagePrinted=0;
		int percentageDone=(int)(100.0f*((float)i/(float)buffer.GetSize()));
		if (lastPercentagePrinted!=percentageDone)
			{
			DebugPrint(("GenerateCPP: %d%% done\n",percentageDone));
			char buffer[6];
			SNPrintF(buffer,5,"%d%%",percentageDone);
			SetProgress(Phase_GeneratingArray,buffer);

			lastPercentagePrinted=percentageDone;
			}
		}
	
	SetProgress(Phase_GeneratingArray,"100%");

	// Write right curly brace at end of array declaration
	SNPrintF(str,4096,"\n};");
	textBuffer.Write(str,StrLen(str));


	// Generate CPP filename
	Platform_FileSystem_File* file=0;
	if (Platform::GetPlatform_FileSystem())
		{
		file=Platform::GetPlatform_FileSystem()->CreateFileObject(archiveFilename);
		}

	SetProgress(Phase_WritingArchive,"");

	// Create output file
	bool result=file->Create();
	Assert(result, "Couldn't open file");
	if (!result)
		{
		delete file;
		return;
		}

	file->Write(static_cast<unsigned char*>(textBuffer.GetPointer()),textBuffer.GetSize());

	// Close the file
	file->Close();
	delete file;

	SetProgress(Phase_Done,"");
	}


//*** GenerateFile ***

void ArchiveGenerator::GenerateFile(const char* directoryPath, const char* archiveFilename)
	{
	SetProgress(Phase_Initializing,"");

	// Make sure structures are empty
	root_.subDirectories.Clear();
	root_.files.Clear();
	filedata_.Clear();

	// Create archive
	CreateArchive(directoryPath);

	int version=0;

	SetProgress(Phase_CreatingHeaders,"");

	// Calculate header size
	DynamicBuffer header(1024*1024);
	WriteDirectoryList(&header,&root_,0);
	int headerSize=header.GetSize();
	headerSize+=StrLen(Pixie_Archive_Header);
	headerSize+=sizeof(version);

	// Generate header and fixup offsets
	header.Clear();
	WriteDirectoryList(&header,&root_,headerSize);
	
	// Generate .DAT filename
	Platform_FileSystem_File* file=0;
	if (Platform::GetPlatform_FileSystem())
		{
		file=Platform::GetPlatform_FileSystem()->CreateFileObject(archiveFilename);
		}

	Assert(file, "Couldn't create file object");
	if (!file)
		{
		return;
		}

	SetProgress(Phase_WritingArchive,"");

	// Create output file
	bool result=file->Create();
	Assert(result, "Couldn't open file");
	if (!result)
		{
		return;
		}

	// Write header
	file->Write(Pixie_Archive_Header,StrLen(Pixie_Archive_Header));
	file->Write(&version);

	// Write directory list
	file->Write(static_cast<unsigned char*>(header.GetPointer()),header.GetSize());

	// Write content of files
	file->Write(static_cast<unsigned char*>(filedata_.GetPointer()),filedata_.GetSize());


	// Close the file
	file->Close();
	delete file;

	SetProgress(Phase_Done,"");
	}


//*** WriteDirectoryList ***
	
void ArchiveGenerator::WriteDirectoryList(DynamicBuffer* buffer, DirectoryEntry* directoryEntry, int offset)
	{
	// Write directory name
	int nameLength=StrLen(directoryEntry->name);
	buffer->Write(&nameLength);
	buffer->Write(directoryEntry->name,nameLength);
	
	// Write files
	int fileCount=directoryEntry->files.GetItemCount();
	buffer->Write(&fileCount);
	for (int i=0; i<fileCount; i++)
		{
		FileEntry& fileEntry=directoryEntry->files.Get(i);
		
		// Fixup offset to compensate for header
		fileEntry.offset+=offset;

		// Write filename
		int nameLength=StrLen(fileEntry.filename);
		buffer->Write(&nameLength);
		buffer->Write(fileEntry.filename,nameLength);

		// Write file offset and size
		int fileOffset=fileEntry.offset;
		buffer->Write(&fileOffset);
		int fileSize=fileEntry.size;
		buffer->Write(&fileSize);
		}

	// Write subdirectories
	int directoryCount=directoryEntry->subDirectories.GetItemCount();
	buffer->Write(&directoryCount);
	for (int i=0; i<directoryCount; i++)
		{
		DirectoryEntry& subDirectoryEntry=directoryEntry->subDirectories.Get(i);
		WriteDirectoryList(buffer,&subDirectoryEntry,offset);
		}
	}


//*** CreateArchive ***

void ArchiveGenerator::CreateArchive(const char* directoryName)
	{
	// Create a directory object for the specified directory, which we'll use to enumerate files
	Platform_FileSystem_Directory* directory=0;
	if (Platform::GetPlatform_FileSystem())
		{
		directory=Platform::GetPlatform_FileSystem()->CreateDirectoryObject(directoryName);
		}

	const char* folderStart=StrRChr(directoryName,'\\');
	const char* folderStart2=StrRChr(directoryName,'/');
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
		folderStart=directoryName;
		}
	int rootPathLength=(int)(folderStart-directoryName);


	// Add all the files to the archive (will recurse down all subdirectories)
	ProcessDirectory(rootPathLength,directory,&root_);

	// Delete the directory object
	delete directory;
	}


//*** ProcessDirectory ***

void ArchiveGenerator::ProcessDirectory(int rootPathLength, Platform_FileSystem_Directory* directory, DirectoryEntry* directoryEntry)
	{
	SetProgress(Phase_ScanningDirectory,directory->GetPath());

	// Store the name of the directory
	const char* path=directory->GetPath();
	path+=rootPathLength;
	if (path==0)
		{
		path="";
		}
	const char* name=StrRChr(path,'/');
	if (!name)
		{
		name=path;
		}
	if (name[0]=='/')
		{
		name++;	
		}
	MemSet(directoryEntry->name,0,260);
	StrNCpy(directoryEntry->name,name,Max(259,StrLen(name)));

	// Loop through all the files of this directory
	for (int i=0; i<directory->GetFileCount(); i++)
		{
		// Generate the full path for this filename
		char filename[4096];
		SNPrintF(filename,4096,"%s/%s",directory->GetPath(),directory->GetFile(i));

		SetProgress(Phase_AddingFile,filename);

		// Create a file object for the file
		Platform_FileSystem_File* file=0;
		if (Platform::GetPlatform_FileSystem())
			{
			file=Platform::GetPlatform_FileSystem()->CreateFileObject(filename);
			}

		// Open the file
		int fileSize=0;
		unsigned char* data=0;
		bool result=file->Open();
		Assert(result, "Couldn't open file");
		if (result)
			{
			// Determine file size
			file->SetPosition(0,Platform_FileSystem_File::ORIGIN_END);
			fileSize=file->GetPosition();
			file->SetPosition(0,Platform_FileSystem_File::ORIGIN_START);

			// Allocate a buffer for the file data
			data=new unsigned char[fileSize];

			// Read the file data
			file->Read(data,fileSize);		

			// Close the file
			file->Close();
			}
		delete file;

		// Create a file entry for this file
		FileEntry fileEntry;

		// Store filename
		MemSet(fileEntry.filename,0,260);
		StrNCpy(fileEntry.filename,directory->GetFile(i),Max(259,StrLen(directory->GetFile(i))));

		// Store the file offset and size
		fileEntry.offset=filedata_.GetSize();
		fileEntry.size=fileSize;

		directoryEntry->files.Add(fileEntry);

		if (data)
			{
			// Write file data to the archive buffer
			filedata_.Write(data,fileSize);

			// Delete the temporary buffer for file data
			delete data;
			}
		}

	// Loop through each subdirectory
	for (int i=0; i<directory->GetSubdirectoryCount(); i++)
		{
		// Generate the full path for this directory
		char directoryName[4096];
		SNPrintF(directoryName,4096,"%s/%s",directory->GetPath(),directory->GetSubdirectory(i));

		// Create a directory object for the subdirectory
		Platform_FileSystem_Directory* directory=0;
		if (Platform::GetPlatform_FileSystem())
			{
			directory=Platform::GetPlatform_FileSystem()->CreateDirectoryObject(directoryName);
			}

		// Create a new entry for this subdirectory
		directoryEntry->subDirectories.Add(DirectoryEntry());
		DirectoryEntry* subDirectoryEntry=&directoryEntry->subDirectories.Get(directoryEntry->subDirectories.GetItemCount()-1);

		// Process the subdirectory
		ProcessDirectory(rootPathLength, directory, subDirectoryEntry);

		// Delete the directory object
		delete directory;
		}

	}


//*** SetProgress ***

void ArchiveGenerator::SetProgress(Phase phase, const char* text)
	{
	progressData_.phase=phase;
	StrNCpy(progressData_.text,text,1023);
	progressData_.text[1023]=0;
	if (progressCallback_)
		{
		progressCallback_(progressData_);
		}
	}