//*** ArchiveManager.cpp ***

#include "ArchiveManager.h"
#include "Debug.h"
#include "HashTableIterator.h"
#include "ArchiveDirectory.h"
#include "ArchiveFile.h"
#include "Platform_FileSystem.h"
#include "Platform_FileSystem_Directory.h"
#include "Platform_FileSystem_File.h"
#include "StandardLibrary.h"
#include "Archive.h"
#include "StringId.h"
#include "Asset.h"
#include "Filename.h"

//*** Constructor ***

ArchiveManager::ArchiveManager()
	{
	RescanHardDrive();
	}


//*** Destructor ***

ArchiveManager::~ArchiveManager()
	{
	UnloadAllArchives();
	}


//*** LoadArchive ***

void ArchiveManager::LoadArchive(const Filename& filename)
	{
	// Cache is no longer valid after we add a new archive
	ClearDirectoryCache();

	// Create new archive entry for the archive
	ArchiveEntry entry;
	entry.archive=new Archive(filename);
	entry.filename=filename.GetStringId();
	entry.memoryBuffer=0;
	const ArchiveDirectory* rootDirectory=entry.archive->GetRootDirectory();
	if (rootDirectory)
		{
		entry.rootDirectory=GetRootDirectory(rootDirectory->GetPath().GetString());	
		}

	// Add entry to list of archives
	archives_.Add(entry);
	}


//*** LoadArchive ***

void ArchiveManager::LoadArchive(const void* memoryBuffer, int size)
	{
	// Cache is no longer valid after we add a new archive
	ClearDirectoryCache();

	// Create new archive entry for the archive
	ArchiveEntry entry;
	entry.archive=new Archive(memoryBuffer,size);
	entry.filename=StringId();
	entry.memoryBuffer=memoryBuffer;
	entry.rootDirectory=GetRootDirectory(entry.archive->GetRootDirectory()->GetPath().GetString());	

	// Add entry to list of archives
	archives_.Add(entry);
	}


//*** UnloadArchive ***

void ArchiveManager::UnloadArchive(const Filename& filename)
	{
	// Cache is no longer valid after we remove an archive
	ClearDirectoryCache();

	// Remove the specified archive
	for (int i=0; i<archives_.GetItemCount(); i++)
		{
		ArchiveEntry& entry=archives_.Get(i);
		if (entry.filename==filename.GetStringId())
			{
			delete entry.archive;
			archives_.Remove(i);
			return;
			}
		}

	Assert(false,"Archive could not be found");
	}


//*** UnloadArchive ***

void ArchiveManager::UnloadArchive(const void* memoryBuffer)
	{
	// Cache is no longer valid after we remove an archive
	ClearDirectoryCache();

	// Remove the specified archive
	for (int i=0; i<archives_.GetItemCount(); i++)
		{
		ArchiveEntry& entry=archives_.Get(i);
		if (entry.memoryBuffer==memoryBuffer)
			{
			delete entry.archive;
			archives_.Remove(i);
			return;
			}
		}

	Assert(false,"Archive could not be found");
	}


//*** UnloadAllArchives ***

void ArchiveManager::UnloadAllArchives()
	{
	// Cache is no longer valid after we remove archives
	ClearDirectoryCache();

	for (int i=0; i<archives_.GetItemCount(); i++)
		{
		ArchiveEntry& entry=archives_.Get(i);
		delete entry.archive;
		}
	archives_.Clear(false);
	}


//*** RescanHardDrive ***

void ArchiveManager::RescanHardDrive()
	{
	// Cache is no longer valid after a rescan
	ClearDirectoryCache();

	// Reset list of root directories
	rootDirectories_.Clear(false);	

	// We can only scan the harddrive if we have a valid file system
	if (Platform::GetPlatform_FileSystem())
		{
		// Create a directory object for the current path
		Platform_FileSystem_Directory* directory=Platform::GetPlatform_FileSystem()->CreateDirectoryObject(".");
		if (directory->Exists())
			{
			// Add all subdirectories found in the current path
			for (int i=0; i<directory->GetSubdirectoryCount(); i++)
				{
				rootDirectories_.Add(directory->GetSubdirectory(i));
				}
			} 
		delete directory;
		}
	}


//*** GetFile ***

ArchiveFile* ArchiveManager::GetFile(const Filename& filename) const
	{
	// Check if there's a root directory on the harddrive which might override archives
	bool rootDirectoryExists=false;
	if (rootDirectories_.GetItemCount()>0)
		{
		StringId rootDirectory=GetRootDirectory(filename.GetString());
		rootDirectoryExists=RootDirectoryExists(rootDirectory);
		}
	if (rootDirectoryExists && Platform::GetPlatform_FileSystem())
		{
		// Check if the file exists
		Platform_FileSystem_File* file=Platform::GetPlatform_FileSystem()->CreateFileObject(filename.GetString());
		if (file->Exists())
			{
			// Get the file size
			int size=0;
			if (file->Open())
				{
				file->SetPosition(0,Platform_FileSystem_File::ORIGIN_END);
				size=file->GetPosition();

				// Return a new ArchiveFile representing this file
				return new ArchiveFile(file,0,size,false);
				}
			}

		delete file;
		}

	// Check each matching archive, in reverse order they where added (get file from most recent added archive first)
	for (int i=archives_.GetItemCount()-1; i>=0; i--)
		{
		const ArchiveEntry& entry=archives_.Get(i);
		ArchiveFile* file=entry.archive->GetFile(filename);
		if (file)
			{
			return file;
			}
		}

	return 0;
	}


//*** GetDirectory ***

const ArchiveDirectory* ArchiveManager::GetDirectory(const Filename& path) const
	{
	// Check if the directory is already in the directory cache (have been accessed before)
	HashTableIterator<HashTableKey_StringId,ArchiveDirectory*> it(directoryCache_);
	if (it.Find(path.GetStringId()))
		{
		// If so, return it
		return it.GetCurrent();
		}

	// Directory doesn't exist in the cache, so create it and store it in the cache
	ArchiveDirectory* archiveDirectory=new ArchiveDirectory();
	archiveDirectory->path_=path.GetStringId();
	directoryCache_.Insert(HashTableKey_StringId(path.GetStringId()),archiveDirectory);
	
	// The current path is handled differently (by just adding the root directories from HD and all archives)
	static StringId currentPath(".");
	if (path==currentPath)
		{
		// We can only scan the harddrive if we have a valid file system
		if (Platform::GetPlatform_FileSystem())
			{
			// Create a directory object for the current path
			Platform_FileSystem_Directory* directory=Platform::GetPlatform_FileSystem()->CreateDirectoryObject(".");
			if (directory->Exists())
				{
				// Add all subdirectories found in the current path
				for (int i=0; i<directory->GetSubdirectoryCount(); i++)
					{
					archiveDirectory->subdirectories_.Add(directory->GetSubdirectory(i));
					}
				for (int i=0; i<directory->GetFileCount(); i++)
					{
					archiveDirectory->files_.Add(directory->GetFile(i));
					}
				} 
			delete directory;
			}

		// Add contents from each matching archive
		for (int i=0; i<archives_.GetItemCount(); i++)
			{
			const ArchiveEntry& entry=archives_.Get(i);
			const ArchiveDirectory* directory=entry.archive->GetRootDirectory();
			StringId directoryName=directory->GetPath();
			// Only add it if it doesn't already exist (it might have been added by previous archive or HD)
			bool exists=false;
			for (int j=0; j<archiveDirectory->GetSubdirectoryCount(); j++)
				{
				if (archiveDirectory->GetSubdirectory(j)==directoryName)
					{
					exists=true;
					break;
					}
				}
			if (!exists)
				{
				archiveDirectory->subdirectories_.Add(directoryName);
				}
			}

		// Return the new archiveDirectory object
		return archiveDirectory;
		}

	// Check if there's a root directory on the harddrive which might override archives
	StringId rootDirectory=GetRootDirectory(path.GetString());
	if (RootDirectoryExists(rootDirectory) && Platform::GetPlatform_FileSystem())
		{
		// Check if the actual subdirectory exists too
		Platform_FileSystem_Directory* directory=Platform::GetPlatform_FileSystem()->CreateDirectoryObject(path.GetString());
		if (directory->Exists())
			{
			// Add the contents of the directory to the archivedirectory
			for (int i=0; i<directory->GetSubdirectoryCount(); i++)
				{
				// Generate the full path for this directory
				char directoryName[4096];
				SNPrintF(directoryName,4096,"%s/%s",directory->GetPath(),directory->GetSubdirectory(i));

				archiveDirectory->AddSubdirectory(directoryName);
				}
			for (int i=0; i<directory->GetFileCount(); i++)
				{
				// Generate the full path for this file
				char filename[4096];
				SNPrintF(filename,4096,"%s/%s",directory->GetPath(),directory->GetFile(i));

				archiveDirectory->AddFile(filename);
				}
			}
		delete directory;
		}

	// Add contents from each matching archive
	for (int i=0; i<archives_.GetItemCount(); i++)
		{
		const ArchiveEntry& entry=archives_.Get(i);
		if (rootDirectory==entry.rootDirectory)
			{
			const ArchiveDirectory* directory=entry.archive->GetDirectory(path);
			if (directory)
				{
				// Add subdirectories
				for (int i=0; i<directory->GetSubdirectoryCount(); i++)
					{
					StringId subdirectoryName=directory->GetSubdirectory(i);
					// Only add it if it doesn't already exist (it might have been added by previous archive or HD)
					bool exists=false;
					for (int j=0; j<archiveDirectory->GetSubdirectoryCount(); j++)
						{
						if (archiveDirectory->GetSubdirectory(j)==subdirectoryName)
							{
							exists=true;
							break;
							}
						}
					if (!exists)
						{
						archiveDirectory->subdirectories_.Add(subdirectoryName);
						}
					}

				// Add files
				for (int i=0; i<directory->GetFileCount(); i++)
					{
					StringId filename=directory->GetFile(i);
					// Only add it if it doesn't already exist (it might have been added by previous archive or HD pass)
					bool exists=false;
					for (int j=0; j<archiveDirectory->GetFileCount(); j++)
						{
						if (archiveDirectory->GetFile(j)==filename)
							{
							exists=true;
							break;
							}
						}
					if (!exists)
						{
						archiveDirectory->files_.Add(filename);
						}
					}
				}
			}
		}

	// Return the new archiveDirectory object
	return archiveDirectory;
	}


//*** GetRootDirectory ***

StringId ArchiveManager::GetRootDirectory(const char* path) const
	{
	char* tempPath=StrDup(path);
	const char* separator=StrChr(tempPath,'/');
	const char* otherSeparator=StrChr(tempPath,'\\');
	if (!separator || (otherSeparator && otherSeparator<separator))
		{
		separator=otherSeparator;
		}

	if (!separator)
		{
		Free(tempPath);
		return StringId(path);
		}

	*(const_cast<char*>(separator))='\0';

	StringId returnValue=StringId(tempPath);
	Free(tempPath);
	return returnValue;
	}


//*** RootDirectoryExists ***

bool ArchiveManager::RootDirectoryExists(StringId directoryName) const
	{
	for (int i=0; i<rootDirectories_.GetItemCount(); i++)
		{
		if (rootDirectories_.Get(i)==directoryName)
			{
			return true;
			}
		}

	return false;
	}


//*** ClearDirectoryCache *** 

void ArchiveManager::ClearDirectoryCache()
	{
	HashTableIterator<HashTableKey_StringId,ArchiveDirectory*> it(directoryCache_);
	while (it.IsValid())
		{
		delete it.GetCurrent();
		it.MoveNext();
		}
	
	directoryCache_.Clear(false);
	}