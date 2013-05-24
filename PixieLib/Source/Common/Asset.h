/**
 * \class	Asset
 * 
 * \ingroup	core
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 */

#ifndef __Asset_H__
#define __Asset_H__

// Includes
#include "Filename.h"

// Forward declares
class Platform_FileSystem_File;
class DynamicBuffer;
class StaticBuffer;
class ArchiveFile;
class StringId;

// Asset
class Asset
	{
	public:
		// SeekOrigin
		enum SeekOrigin
			{
			SEEK_FROM_START = 0,		///< Seek from the start of the file
			SEEK_FROM_CURRENT = 1,		///< Seek from the current position in the file
			SEEK_FROM_END = 2,			///< Seek from the end of the file
			};

	public:
		/**
		 * Constructors
		 */
		Asset(const StaticBuffer* memoryBuffer);
		Asset(DynamicBuffer* memoryBuffer);
		Asset(const Filename& filename);
		Asset(const StringId& filename);
		Asset(const char* filename);

		/**
		 * Destructor
		 */
		~Asset();

		Asset(const Asset& asset);

		const Asset& operator=(const Asset&);

		const Filename& GetFilename() const;

		bool Open() const;
		bool Create();
		bool Close() const;
		int GetSize() const;
		int Seek(int offset, SeekOrigin origin = SEEK_FROM_START) const;
		int Tell() const;

		int Write(const char* value, int count = 1);
		int Write(const short* value, int count = 1);
		int Write(const int* value, int count = 1);
		int Write(const long* value, int count = 1);
		int Write(const unsigned char* value, int count = 1);
		int Write(const unsigned short* value, int count = 1);
		int Write(const unsigned int* value, int count = 1);
		int Write(const unsigned long* value, int count = 1);
		int Write(const float* value, int count = 1);
		int Write(const double* value, int count = 1);
		int Write(const bool* value, int count = 1);

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
		void CreateFromFilename(const Filename& filename);
	private:
		mutable DynamicBuffer* dynamicBuffer_;
		mutable const StaticBuffer* staticBuffer_;
		mutable ArchiveFile* archiveFile_;
		mutable Platform_FileSystem_File* file_;
		mutable bool fileIsOpen_;
		mutable bool readOnly_;
		Filename filename_;
	};


#endif /* __Asset_H__ */
