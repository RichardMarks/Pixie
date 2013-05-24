/**
 * \class	ArchiveFile
 * 
 * \ingroup	core
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 */

#ifndef __ArchiveFile_H__
#define __ArchiveFile_H__

// Includes

// Forward declares
class StaticBuffer;
class Platform_FileSystem_File;

// ArchiveFile
class ArchiveFile
	{
	public:
		ArchiveFile(Platform_FileSystem_File* file, int offset, int size, bool sharedFile);
		ArchiveFile(const StaticBuffer* memoryBuffer, int offset, int size);
		ArchiveFile(const ArchiveFile* archiveFile);
		~ArchiveFile();

		// SeekOrigin
		enum SeekOrigin
			{
			SEEK_FROM_START = 0,		///< Seek from the start of the file
			SEEK_FROM_CURRENT = 1,		///< Seek from the current position in the file
			SEEK_FROM_END = 2,			///< Seek from the end of the file
			};

		bool Open() const;
		bool Close() const;
		int GetSize() const;
		int Seek(int offset, SeekOrigin origin = SEEK_FROM_START) const;
		int Tell() const;

		int Read(char* value, int count = 1) const;
		int Read(short* value, int count = 1) const;
		int Read(int* value, int count = 1) const;
		int Read(long* value, int count = 1) const;
		int Read(unsigned char* value, int count = 1) const;
		int Read(unsigned short* value, int count = 1) const;
		int Read(unsigned int* value, int count = 1) const;
		int Read(unsigned long* value, int count = 1) const;
		int Read(float* value, int count = 1) const;
		int Read(double* value, int count = 1) const;
		int Read(bool* value, int count = 1) const;

	private:
		Platform_FileSystem_File* file_;
		bool sharedFile_;
		const StaticBuffer* buffer_;
		mutable int position_;
		int offset_;
		int size_;
	};


#endif /* __ArchiveFile_H__ */
