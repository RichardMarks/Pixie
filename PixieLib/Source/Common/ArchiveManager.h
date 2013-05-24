/**
 * \class	ArchiveManager
 * 
 * \ingroup	core
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 */

#ifndef __ArchiveManager_H__
#define __ArchiveManager_H__

// Includes
#include "Singleton.h"
#include "Array.h"
#include "StringId.h"
#include "HashTable.h"
#include "HashTableKey_StringId.h"

// Forward declares
class ArchiveFile;
class ArchiveDirectory;
class Archive;
class Filename;


// ArchiveManager
class ArchiveManager:public Singleton<ArchiveManager>
	{
	public:
		ArchiveManager();

		virtual ~ArchiveManager();

		void LoadArchive(
			const Filename& filename	///< Full path to archive file
			);
		
		void LoadArchive(
			const void* memoryBuffer,	///< Pointer to archive data
			int size	///< Size of archive data
			);

		void UnloadArchive(
			const Filename& filename	///< Full path to archive file
			);

		void UnloadArchive(
			const void* memoryBuffer	///< Pointer to archive data
			);

		void UnloadAllArchives();

		void RescanHardDrive();

		ArchiveFile* GetFile(
			const Filename& filename	///< The filename of the file to get
			) const;


		const ArchiveDirectory* GetDirectory(
			const Filename& path 	///< The path of the directory to get
			) const;

	private:
		StringId GetRootDirectory(const char* path) const;
		bool RootDirectoryExists(StringId directoryName) const;
		void ClearDirectoryCache();
	private:
		struct ArchiveEntry
			{
			StringId rootDirectory;
			StringId filename;
			const void* memoryBuffer;
			Archive* archive;
			};

		Array<ArchiveEntry> archives_;

		Array<StringId> rootDirectories_;

		mutable HashTable<HashTableKey_StringId,ArchiveDirectory*> directoryCache_;
	};

#define siArchiveManager ArchiveManager::GetInstance()

#endif /* __ArchiveManager_H__ */
