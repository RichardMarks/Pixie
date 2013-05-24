/**
 * \class	ArchiveDirectory
 * 
 * \ingroup	core
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 */

#ifndef __ArchiveDirectory_H__
#define __ArchiveDirectory_H__

// Includes
#include "StringId.h"
#include "Array.h"

// Forward declares

// ArchiveDirectory
class ArchiveDirectory
	{
	public:
		StringId GetPath() const;

		int GetSubdirectoryCount() const;
		StringId GetSubdirectory(int index) const;

		int GetFileCount() const;
		StringId GetFile(int index) const;

	private:
		void SetPath(const char* fullpath);
		void AddSubdirectory(const char* fullpath);
		void AddFile(const char* fullpath);

	private:
		friend class Archive;
		friend class ArchiveManager;

		StringId path_;
		Array<StringId> subdirectories_;
		Array<StringId> files_;
	};


#endif /* __ArchiveDirectory_H__ */
