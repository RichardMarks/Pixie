/**
 * \class	Archive
 * 
 * \ingroup	core
 * \brief	Simple system for accessing archive content
 * \author	Mattias Gustavsson	
 * 
 * An Archive is what you get when packing lots of files together into one big file. Check out ArchiveGenerator for a 
 * utility class for generating archives.
 * 
 * This archive class can be passed to the static method Asset::SetArchive, to direct all Asset requests to the 
 * specified archive.
 */

#ifndef __Archive_H__
#define __Archive_H__

// Includes
#include "StringId.h"
#include "Array.h"
#include "HashTable.h"
#include "HashTableKey_StringId.h"

// Forward declares
class StaticBuffer;
class Platform_FileSystem_File;
class ArchiveFile;
class ArchiveDirectory;
class Filename;


// Archive
class Archive
	{
	public:
		/** 
		 * Creates a new archive from the specified memory area with the specified size. It doesn't make a copy of the 
		 * data passed in, so make sure to keep the data around until you destroy the archive.
		 */
		Archive(
			const void* memoryBuffer,	///< Pointer to archive data
			int size	///< Size of archive data
			);

		/** 
		 * Creates a new archive from the specified file. It doesn't load the entire archive into memory, but it opens 
		 * the file and keeps it open until the archive is destroyed. Data is only loaded as it is read trough Assets.
		 */
		Archive(
			const Filename& filename	///< Full path to archive file
			);

		/**
		 * Destructor
		 */
		~Archive();

		/** 
         * This method is used to get the data of the file with the specified name  from the archive
		 *
		 * \returns	A pointer to a StaticBuffer containing the file data of the requested file. If the file doesn't 
		 *			exist, GetFile will assert and return 0
		 */
		ArchiveFile* GetFile(
			const Filename& filename	///< The name of the file to get
			) const;


		const ArchiveDirectory* GetDirectory(const Filename& path) const;

		const ArchiveDirectory* GetRootDirectory() const;


	private:
		struct DirectoryEntry;

		void ReadDirectory(DirectoryEntry* directoryEntry,char* parentPath);

	private:
		StaticBuffer* archiveBuffer_;	///< The memory buffer holding the archive data
		Platform_FileSystem_File* archiveFile_;	///< The file containing the archive data

		struct FileEntry
			{
			StringId filename;
			int offset;
			int size;
			};
		
		struct DirectoryEntry
			{
			StringId name;
			Array<DirectoryEntry> subDirectories;
			Array<FileEntry> files;
			};

		DirectoryEntry root_;

		HashTable<HashTableKey_StringId,FileEntry> files_; ///< Hash table holding a FileEntry for each file in the archive
		HashTable<HashTableKey_StringId,ArchiveDirectory*> directories_; ///< Hash table holding a DirectoryEntry for each directory in the archive
	};


#endif /* __Archive_H__ */
