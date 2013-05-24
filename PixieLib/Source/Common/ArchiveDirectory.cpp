//*** ArchiveDirectory.cpp ***

#include "ArchiveDirectory.h"

//*** GetPath ***

StringId ArchiveDirectory::GetPath() const
	{
	return path_;
	}


//*** GetSubdirectoryCount ***

int ArchiveDirectory::GetSubdirectoryCount() const
	{
	return subdirectories_.GetItemCount();
	}


//*** GetSubdirectory ***

StringId ArchiveDirectory::GetSubdirectory(int index) const
	{
	return subdirectories_.Get(index);
	}


//*** GetFileCount ***

int ArchiveDirectory::GetFileCount() const
	{
	return files_.GetItemCount();
	}


//*** GetFile ***

StringId ArchiveDirectory::GetFile(int index) const
	{
	return files_.Get(index);
	}


//*** SetPath ***

void ArchiveDirectory::SetPath(const char* fullpath)
	{
	path_=StringId(fullpath);
	}


//*** AddSubdirectory ***

void ArchiveDirectory::AddSubdirectory(const char* fullpath)
	{
	subdirectories_.Add(StringId(fullpath));
	}


//*** AddFile ***

void ArchiveDirectory::AddFile(const char* fullpath)
	{
	files_.Add(StringId(fullpath));
	}
