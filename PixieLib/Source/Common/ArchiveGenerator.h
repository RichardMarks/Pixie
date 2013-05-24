/**
 * \class	ArchiveGenerator
 * 
 * \ingroup	core
 * \brief	Utility class used to pack files together into an Archive file
 * \author	Mattias Gustavsson	
 * 
 * With this utility class, you can very easily pack your game data together in one big Archive file, which can be used 
 * to speed up loading times.
 */

#ifndef __ArchiveGenerator_H__
#define __ArchiveGenerator_H__

// Includes
#include "Array.h"
#include "DynamicBuffer.h"

// Forward declares
class DynamicBuffer;
class Platform_FileSystem_Directory;

// ArchiveGenerator
class ArchiveGenerator
	{
	public:
		enum Phase
			{
			Phase_NotStarted,
			Phase_Initializing,
			Phase_ScanningDirectory,
			Phase_AddingFile,
			Phase_CreatingHeaders,
			Phase_WritingArchive,
			Phase_GeneratingArray,
			Phase_Done,
			};
		struct ProgressData
			{
			Phase phase;
			char text[1024];
			};
		typedef void (*ProgressCallback)(ProgressData& data);


		/**
		 * Constructor
		 */
		ArchiveGenerator(ProgressCallback progressCallback = 0);

		/**
		 * Creates an archive containting all the files in the specified directory and
		 * all its subdirectories. Generates a cpp file which defines an array initialized 
		 * with the archive data, so it can be included in the project and used as a
		 * static buffer for the Asset::SetArchive method.
		 */
		void GenerateCPP(
			const char* directoryPath,	///< Directory to build archive from
			const char* archiveFilename,
			const char* archiveName
			);

		/**
		 * Creates an archive containting all the files in the specified directory and
		 * all its subdirectories. Generates a binary file with the archive data, so it 
		 * can be loaded on startup, and used with the Asset::SetArchive method.
		 */
		void GenerateFile(
			const char* directoryPath,	///< Directory to build archive from
			const char* archiveFilename
			);

	private:
		struct DirectoryEntry;

		/** 
		 * Create a new DynamicBuffer containing an archive with all the files in the specified
		 * directory and all its subdirectories
		 */
		void CreateArchive(const char* directoryName);

		 /**
		  * Scans a directory and all its subdirectories, adding all files to the buffer
		  */
		void ProcessDirectory(
			int rootPathLength,
			Platform_FileSystem_Directory* directory,	///< Directory object to process
			DirectoryEntry* directoryEntry	///< Archive entry for directory
			);

		void WriteDirectoryList(
			DynamicBuffer* buffer, 
			DirectoryEntry* directoryEntry,
			int offset
			);

		void SetProgress(Phase phase, const char* text);
	private:
		ProgressCallback progressCallback_;
		ProgressData progressData_;

		struct FileEntry
			{
			char filename[260];
			int offset;
			int size;
			};
		struct DirectoryEntry
			{
			char name[260];
			Array<DirectoryEntry> subDirectories;
			Array<FileEntry> files;
			};

		DirectoryEntry root_;
		DynamicBuffer filedata_;
	};


#endif /* __ArchiveGenerator_H__ */
