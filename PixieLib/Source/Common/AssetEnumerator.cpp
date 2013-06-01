//*** AssetEnumerator.cpp ***

#include "AssetEnumerator.h"
#include "Debug.h"
#include "Platform_FileSystem.h"
#include "Platform_FileSystem_Directory.h"
#include "Archive.h"
#include "ArchiveDirectory.h"
#include "Asset.h"
#include "StandardLibrary.h"
#include "ArchiveManager.h"
#include "Filename.h"

//*** Constructor ***

AssetEnumerator::AssetEnumerator(const Filename& filename):
	directory_(0),
	archiveDirectory_(0),
	nameBuffer_(0),
	nameBufferSize_(0)
	{
	CreateFromFilename(filename);
	}


//*** Constructor ***

AssetEnumerator::AssetEnumerator(const StringId& filenameId):
	directory_(0),
	archiveDirectory_(0),
	nameBuffer_(0),
	nameBufferSize_(0)
	{
	Filename filename(filenameId);
	CreateFromFilename(filename);
	}


//*** CreateFromFilename ***

void AssetEnumerator::CreateFromFilename(const Filename& filename)
	{
	// First, try and get the directory from the archive manager, if it is present
	if (ArchiveManager::IsInstanceCreated())
		{
		archiveDirectory_=siArchiveManager->GetDirectory(filename.GetStringId());
		}

	// If we couldn't get the directory from the archive manager, try to get it from the file system, if it is present
	if (!archiveDirectory_ && Platform::GetPlatform_FileSystem())
		{
		directory_=Platform::GetPlatform_FileSystem()->CreateDirectoryObject(filename.GetString());
		}
	
	// If we couldn't get the directory from neither the archive manager nor the filesystem, we trigger an assert for
	// debug builds, and just reports the directory as being empty for release builds.
	Assert(archiveDirectory_ || directory_,
		"Error creating AssetEnumerator - No filesystem or archive manager present, or directory not found.");
	}


//*** Destructor ***

AssetEnumerator::~AssetEnumerator()
	{
	if (directory_)
		{
		delete directory_;
		}
	if (nameBuffer_)
		{
		delete nameBuffer_;
		}
	}


//*** Copy Constructor ***

AssetEnumerator::AssetEnumerator(const AssetEnumerator& assetEnumerator):
	directory_(0),
	archiveDirectory_(assetEnumerator.archiveDirectory_),
	nameBuffer_(0),
	nameBufferSize_(0)
	{
	if (assetEnumerator.directory_ && Platform::GetPlatform_FileSystem())
		{
		directory_=Platform::GetPlatform_FileSystem()->CreateDirectoryObject(assetEnumerator.directory_->GetPath());
		}
	}


//*** Copy Operator ***

const AssetEnumerator& AssetEnumerator::operator=(const AssetEnumerator& assetEnumerator)
	{
	if (directory_)
		{
		delete directory_;
		directory_=0;
		}

	if (assetEnumerator.directory_ && Platform::GetPlatform_FileSystem())
		{
		directory_=Platform::GetPlatform_FileSystem()->CreateDirectoryObject(assetEnumerator.directory_->GetPath());
		}

	archiveDirectory_=assetEnumerator.archiveDirectory_;

	return *this;
	}


//*** GetAssetCount ***

int AssetEnumerator::GetAssetCount()
	{
	if (directory_)
		{
		return directory_->GetFileCount();
		}

	if (archiveDirectory_)
		{
		return archiveDirectory_->GetFileCount();
		}

	return 0;
	}


//*** GetAssetFilename ***

const char* AssetEnumerator::GetAssetFilename(int index)
	{
	if (directory_)
		{
		const char* filename=directory_->GetFile(index);
		int size=StrLen(directory_->GetPath())+StrLen(filename)+2;
		if (!nameBuffer_ || nameBufferSize_<size)
			{
			if (nameBuffer_)
				{
				delete nameBuffer_;
				}
			nameBufferSize_=size;
			nameBuffer_=new char[nameBufferSize_];
			}
		SNPrintF(nameBuffer_,nameBufferSize_,"%s/%s",directory_->GetPath(), filename);
		return nameBuffer_;
		}

	if (archiveDirectory_)
		{
		return archiveDirectory_->GetFile(index).GetString();
		}

	return 0;
	}


//*** GetSubdirectoryCount ***

int AssetEnumerator::GetSubdirectoryCount()
	{
	if (directory_)
		{
		return directory_->GetSubdirectoryCount();
		}

	if (archiveDirectory_)
		{
		return archiveDirectory_->GetSubdirectoryCount();
		}

	return 0;
	}


//*** GetSubdirectoryName ***

const char* AssetEnumerator::GetSubdirectoryName(int index)
	{
	if (directory_)
		{
		const char* dirname=directory_->GetSubdirectory(index);
		int size=StrLen(directory_->GetPath())+StrLen(dirname)+2;
		if (!nameBuffer_ || nameBufferSize_<size)
			{
			if (nameBuffer_)
				{
				delete nameBuffer_;
				}
			nameBufferSize_=size;
			nameBuffer_=new char[nameBufferSize_];
			}
		SNPrintF(nameBuffer_,nameBufferSize_,"%s/%s",directory_->GetPath(), dirname);
		return nameBuffer_;
		}


	if (archiveDirectory_)
		{
		return archiveDirectory_->GetSubdirectory(index).GetString();
		}

	return 0;	
	}
