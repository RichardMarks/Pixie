//*** ArchiveFile.cpp ***

#include "ArchiveFile.h"
#include "StaticBuffer.h"
#include "Platform.h"
#include "Platform_FileSystem.h"
#include "Platform_FileSystem_File.h"
#include "Debug.h"

//*** Constructor ***

ArchiveFile::ArchiveFile(Platform_FileSystem_File* file, int offset, int size, bool sharedFile):
	file_(file),
	buffer_(0),
	position_(0),
	offset_(offset),
	size_(size),
	sharedFile_(sharedFile)
	{

	}


//*** Constructor ***

ArchiveFile::ArchiveFile(const StaticBuffer* memoryBuffer, int offset, int size):
	file_(0),
	buffer_(memoryBuffer),
	position_(0),
	offset_(offset),
	size_(size),
	sharedFile_(false)
	{

	}


//*** Copy Constructor ***

ArchiveFile::ArchiveFile(const ArchiveFile* archiveFile):
	file_(archiveFile->file_),
	buffer_(archiveFile->buffer_),
	position_(archiveFile->position_),
	offset_(archiveFile->offset_),
	size_(archiveFile->size_),
	sharedFile_(archiveFile->sharedFile_)
	{
	if (!sharedFile_ && file_)
		{

		file_=Platform::GetPlatform_FileSystem()->CreateFileObject(archiveFile->file_->GetPath());
		file_->Open();
		}
	}


//*** Destructor ***

ArchiveFile::~ArchiveFile()
	{
	if (file_ && !sharedFile_)
		{
		delete file_;
		}

	}


//*** Create ***

bool ArchiveFile::Open() const
	{
	position_=0;
	return true;
	}


//*** Close ***

bool ArchiveFile::Close() const
	{
	position_=-1;
	return true;
	}


//*** GetSize ***

int ArchiveFile::GetSize() const
	{
	return size_;
	}


//*** Seek ***

int ArchiveFile::Seek(int offset, SeekOrigin origin) const
	{
	switch (origin)
		{
		case SEEK_FROM_START:
			{
			position_=offset;
			return position_;
			} break;

		case SEEK_FROM_CURRENT:
			{
			position_+=offset;
			return position_;
			} break;

		case SEEK_FROM_END:
			{
			position_=size_+offset;
			return position_;
			} break;
		}

	Assert(false,"Invalid seek origin");
	return -1;
	}


//*** Tell ***

int ArchiveFile::Tell() const
	{
	return position_;
	}
	

//*** READMACRO ***

// This macro defines the code that is to be executed for all of the overloaded Read methods
// The code is identical for all the different Read methods, only the input parameters change,
// so it makes more sense to use a macro rather than copy-paste. This macro is undefined as
// soon as it is not needed anymore

#define READMACRO()  \
	/* Calculate the total number of bytes to be read */					\
	int totalSize=sizeof(*value)*count;										\
	/* Make sure we're not trying to read more bytes than there is */		\
	if (position_+totalSize>size_)											\
		{																	\
		count=(size_-position_)/sizeof(*value);								\
		totalSize=sizeof(*value)*count;										\
		}																	\
	/* Read the actual data	*/												\
	if (buffer_)															\
		{																	\
		buffer_->SetPosition(offset_+position_);							\
		buffer_->Read(value,count);											\
		}																	\
	if (file_)																\
		{																	\
		file_->SetPosition(offset_+position_);								\
		file_->Read(value,count);											\
		}																	\
	/* Update the current position */										\
	position_+=totalSize;													\
	/* Return the number of elements written */				 				\
	return count;															\


//** Read methods */
	
int ArchiveFile::Read(char* value, int count) const
	{
	READMACRO();
	}

int ArchiveFile::Read(short* value, int count) const
	{
	READMACRO();
	}

int ArchiveFile::Read(int* value, int count) const
	{
	READMACRO();
	}

int ArchiveFile::Read(long* value, int count) const
	{
	READMACRO();
	}

int ArchiveFile::Read(unsigned char* value, int count) const
	{
	READMACRO();
	}

int ArchiveFile::Read(unsigned short* value, int count) const
	{
	READMACRO();
	}

int ArchiveFile::Read(unsigned int* value, int count) const
	{
	READMACRO();
	}

int ArchiveFile::Read(unsigned long* value, int count) const
	{
	READMACRO();
	}

int ArchiveFile::Read(float* value, int count) const
	{
	READMACRO();
	}

int ArchiveFile::Read(double* value, int count) const
	{
	READMACRO();
	}

int ArchiveFile::Read(bool* value, int count) const
	{
	READMACRO();
	}

// No need for the read macro anymore, so undefine it
#undef READMACRO