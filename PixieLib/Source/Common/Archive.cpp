//*** Archive.cpp ***

#include "Archive.h"
#include "Debug.h"
#include "StaticBuffer.h"
#include "HashTableIterator.h"
#include "Platform_FileSystem.h"
#include "Platform_FileSystem_File.h"
#include "ArchiveFile.h"
#include "ArchiveDirectory.h"
#include "StandardLibrary.h"
#include "Asset.h"
#include "Filename.h"

const char* Pixie_Archive_Header="PIXIE_ARCHIVE";


//*** Constructor ***

Archive::Archive(const void* memoryBuffer, int size):
	archiveBuffer_(0),
	archiveFile_(0)
	{
	// Create a new StaticBuffer mapped to the specified memory area
	archiveBuffer_=new StaticBuffer(memoryBuffer, size);

	// Read header
	char header[256];
	archiveBuffer_->Read(header,StrLen(Pixie_Archive_Header));
	Assert(StrNICmp(header,Pixie_Archive_Header,StrLen(Pixie_Archive_Header))==0,"Invalid archive header");
	if (StrNICmp(header,Pixie_Archive_Header,StrLen(Pixie_Archive_Header))!=0)
		{
		delete archiveBuffer_;
		archiveBuffer_=0;
		return;
		}

	// Read version
	int version=0;
	archiveBuffer_->Read(&version);
	Assert(version==0,"Invalid archive version");
	if (version!=0)
		{
		delete archiveBuffer_;
		archiveBuffer_=0;
		return;
		}

	// Read directory list
	ReadDirectory(&root_,0);

	}


//*** Constructor ***

Archive::Archive(const Filename& filename):
	archiveBuffer_(0),
	archiveFile_(0)
	{
	// Create the file object for the specified archive file
	archiveFile_=0;
	if (Platform::GetPlatform_FileSystem())
		{
		archiveFile_=Platform::GetPlatform_FileSystem()->CreateFileObject(filename.GetString());
		}

	if (!archiveFile_)
		{
		Assert(false,"No file system present");
		delete archiveFile_;
		archiveFile_=0;
		return;
		}

	// Open the file
	if (!archiveFile_->Open())
		{
		Assert(false,"Couldn't open archive file");
		delete archiveFile_;
		archiveFile_=0;
		return;
		}

	// Read header
	char header[256];
	archiveFile_->Read(header,StrLen(Pixie_Archive_Header));
	Assert(StrNICmp(header,Pixie_Archive_Header,StrLen(Pixie_Archive_Header))==0,"Invalid archive header");
	if (StrNICmp(header,Pixie_Archive_Header,StrLen(Pixie_Archive_Header))!=0)
		{
		delete archiveFile_;
		archiveFile_=0;
		return;
		}

	// Read version
	int version=0;
	archiveFile_->Read(&version);
	Assert(version==0,"Invalid archive version");
	if (version!=0)
		{
		delete archiveFile_;
		archiveFile_=0;
		return;
		}

	// Read directory list
	ReadDirectory(&root_,0);
	}


//*** Destructor ***

Archive::~Archive()
	{
	// Delete the static buffer 
	if (archiveBuffer_)
		{
		delete archiveBuffer_;
		}

	// Delete archive file
	if (archiveFile_)
		{
		archiveFile_->Close();
		delete archiveFile_;
		}

	// Loop through the hash table of directories and delete all 
	HashTableIterator<HashTableKey_StringId,ArchiveDirectory*> it(directories_);
	while (it.IsValid())
		{
		ArchiveDirectory* dirEntry=it.GetCurrent();
		delete dirEntry;
		it.MoveNext();
		}
	}


//*** GetRootDirectory ***

const ArchiveDirectory* Archive::GetRootDirectory() const
	{
	if (files_.GetItemCount()==0 && directories_.GetItemCount()==0)
		{
		return 0;
		}
	return GetDirectory(root_.name);
	}


//*** GetDirectory ***

const ArchiveDirectory* Archive::GetDirectory(const Filename& path) const
	{
	// Try and find a directory with the specified path in the hash table
	HashTableIterator<HashTableKey_StringId,ArchiveDirectory*> it(directories_);
	if (it.Find(HashTableKey_StringId(path.GetStringId())))
		{
		ArchiveDirectory* entry=it.GetCurrent();
		return entry;
		}

	return 0;
	}

		
//*** GetFile ***

ArchiveFile* Archive::GetFile(const Filename& filename) const
	{
	// Try and find a file with the specified filename in the hash table
	HashTableIterator<HashTableKey_StringId,FileEntry> it(files_);
	if (it.Find(HashTableKey_StringId(filename.GetStringId())))
		{
		const FileEntry& entry=it.GetCurrent();

		if (archiveFile_)
			{
			return new ArchiveFile(archiveFile_,entry.offset,entry.size,true);
			}

		if (archiveBuffer_)
			{
			return new ArchiveFile(archiveBuffer_,entry.offset,entry.size);
			}
		}

	return 0;
	}


//*** ReadDirectory ***

void Archive::ReadDirectory(DirectoryEntry* directoryEntry, char* parentPath)
	{
	// Read name length
	int nameLength=0;
	if (archiveFile_)
		{
		archiveFile_->Read(&nameLength);
		}
	else
		{
		archiveBuffer_->Read(&nameLength);
		}

	// Read name
	char* dirname=new char[nameLength+1];
	if (archiveFile_)
		{
		archiveFile_->Read(dirname,nameLength);
		}
	else
		{
		archiveBuffer_->Read(dirname,nameLength);
		}
	dirname[nameLength]=0;
	directoryEntry->name=StringId(dirname);
	delete[] dirname;

	// Generate full path for this directory
	int pathlength=nameLength+1;
	if (parentPath)
		{
		pathlength+=StrLen(parentPath)+1;
		}
	char* fullpath=new char[pathlength];
	if (parentPath)
		{
		SNPrintF(fullpath,pathlength,"%s/%s",parentPath,directoryEntry->name.GetString());
		}
	else
		{
		SNPrintF(fullpath,pathlength,"%s",directoryEntry->name.GetString());
		}

	ArchiveDirectory* archiveDirectory=new ArchiveDirectory();
	directories_.Insert(HashTableKey_StringId(fullpath),archiveDirectory);
	archiveDirectory->SetPath(fullpath);

	// Read file count
	int fileCount=0;
	if (archiveFile_)
		{
		archiveFile_->Read(&fileCount);
		}
	else
		{
		archiveBuffer_->Read(&fileCount);
		}

	// Read files
	for (int i=0; i<fileCount; i++)
		{
		FileEntry fileEntry;

		// Read name length
		int nameLength=0;
		if (archiveFile_)
			{
			archiveFile_->Read(&nameLength);
			}
		else
			{
			archiveBuffer_->Read(&nameLength);
			}

		// Read name
		char* filename=new char[nameLength+1];
		if (archiveFile_)
			{
			archiveFile_->Read(filename,nameLength);
			}
		else
			{
			archiveBuffer_->Read(filename,nameLength);
			}
		filename[nameLength]=0;
		fileEntry.filename=StringId(filename);
		delete[] filename;

		// Read offset
		if (archiveFile_)
			{
			archiveFile_->Read(&fileEntry.offset);
			archiveFile_->Read(&fileEntry.size);
			}
		else
			{
			archiveBuffer_->Read(&fileEntry.offset);
			archiveBuffer_->Read(&fileEntry.size);
			}

		// Add file to directory entry
		directoryEntry->files.Add(fileEntry);

		// Add file to hashtable for quicker lookup
		int fullfilepathlength=StrLen(fullpath)+1+nameLength+1;
		char* fullfilepath=new char[fullfilepathlength];
		SNPrintF(fullfilepath,fullfilepathlength,"%s/%s",fullpath,fileEntry.filename.GetString());
		FileEntry entry(directoryEntry->files.Get(directoryEntry->files.GetItemCount()-1));
		files_.Insert(HashTableKey_StringId(fullfilepath),entry);
		archiveDirectory->AddFile(fullfilepath);
		delete[] fullfilepath;
		}


	// Read subdirectory count
	int subdirectoryCount=0;
	if (archiveFile_)
		{
		archiveFile_->Read(&subdirectoryCount);
		}
	else
		{
		archiveBuffer_->Read(&subdirectoryCount);
		}

	// Read subdirectories
	for (int i=0; i<subdirectoryCount; i++)
		{
		directoryEntry->subDirectories.Add(DirectoryEntry());
		DirectoryEntry* subdirectoryEntry=&directoryEntry->subDirectories.Get(directoryEntry->subDirectories.GetItemCount()-1);
		ReadDirectory(subdirectoryEntry,fullpath);

		int fulldirpathlength=StrLen(fullpath)+1+StrLen(subdirectoryEntry->name.GetString())+1;
		char* fulldirpath=new char[fulldirpathlength];
		SNPrintF(fulldirpath,fulldirpathlength,"%s/%s",fullpath,subdirectoryEntry->name.GetString());
		archiveDirectory->AddSubdirectory(fulldirpath);
		delete[] fulldirpath;
		}

	delete[] fullpath;
	}
