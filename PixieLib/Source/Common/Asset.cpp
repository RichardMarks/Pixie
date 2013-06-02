//*** Asset.cpp ***

#include "Asset.h"
#include "Debug.h"
#include "Platform_FileSystem.h"
#include "Platform_FileSystem_File.h"
#include "DynamicBuffer.h"
#include "Archive.h"
#include "ArchiveFile.h"
#include "StaticBuffer.h"
#include "ArchiveManager.h"
#include "StandardLibrary.h"
#include "Filename.h"

namespace pixie {
//*** Constructor Memory Buffer***

Asset::Asset(const StaticBuffer* staticBuffer):
	file_(0),
	archiveFile_(0),
	staticBuffer_(staticBuffer),
	dynamicBuffer_(0),
	fileIsOpen_(false),
	readOnly_(false)
	{
	}


//*** Constructor Memory Buffer***

Asset::Asset(DynamicBuffer* dynamicBuffer):
	file_(0),
	archiveFile_(0),
	staticBuffer_(0),
	dynamicBuffer_(dynamicBuffer),
	fileIsOpen_(false),
	readOnly_(false)
	{
	}


//*** Constructor File (Filename) ***

Asset::Asset(const Filename& filename):
	filename_(filename),
	file_(0),
	archiveFile_(0),
	staticBuffer_(0),
	dynamicBuffer_(0),
	fileIsOpen_(false),
	readOnly_(false)
	{
	CreateFromFilename(filename_);
	}


//*** Constructor File (StringId) ***

Asset::Asset(const StringId& filenameId):
	filename_(filenameId),
	file_(0),
	archiveFile_(0),
	staticBuffer_(0),
	dynamicBuffer_(0),
	fileIsOpen_(false),
	readOnly_(false)
	{
	CreateFromFilename(filename_);
	}


//*** Constructor File (const char*) ***

Asset::Asset(const char* filenameStr):
	filename_(filenameStr),
	file_(0),
	archiveFile_(0),
	staticBuffer_(0),
	dynamicBuffer_(0),
	fileIsOpen_(false),
	readOnly_(false)
	{
	CreateFromFilename(filename_);
	}


//*** CreateFromFilename ***

void Asset::CreateFromFilename(const Filename& filename)
	{
	// First, try and get the file from the archive manager, if it is present
	if (ArchiveManager::IsInstanceCreated())
		{
		archiveFile_=siArchiveManager->GetFile(filename);
		}

	// If we couldn't get the directory from the archive manager, try to get it from the file system, if it is present
	if (!archiveFile_ && Platform::GetPlatform_FileSystem())
		{
		file_=Platform::GetPlatform_FileSystem()->CreateFileObject(filename.GetString());
		}

	// If we couldn't get the file from neither the archive manager nor the filesystem, we trigger an assert for
	// debug builds, and just reports the file as being empty for release builds.
	Assert(archiveFile_ || file_,
		"Error creating AssetEnumerator - No filesystem or archive manager present, or file not found.");
	}


//*** Destructor ***

Asset::~Asset()
	{
	if (file_)
		{
		if (fileIsOpen_)
			{
			file_->Close();
			}
		delete file_;
		}

	if (archiveFile_)
		{
		if (fileIsOpen_)
			{
			archiveFile_->Close();
			}
		delete archiveFile_;
		}
	}


//*** Copy Constructor ***

Asset::Asset(const Asset& asset):
	file_(0),
	archiveFile_(0),
	staticBuffer_(0),
	dynamicBuffer_(0),
	fileIsOpen_(false),
	readOnly_(false)
	{
	staticBuffer_=asset.staticBuffer_;
	dynamicBuffer_=asset.dynamicBuffer_;
	fileIsOpen_=asset.fileIsOpen_;
	readOnly_=asset.readOnly_;
	if (asset.file_)
		{
		const char* filename=asset.file_->GetPath();
		if (Platform::GetPlatform_FileSystem())
			{
			file_=file_=Platform::GetPlatform_FileSystem()->CreateFileObject(filename);
			}
		if (fileIsOpen_)
			{
			file_->Open();
			file_->SetPosition(asset.file_->GetPosition());
			}
		}
	if (asset.archiveFile_)
		{
		archiveFile_=new ArchiveFile(asset.archiveFile_);
		if (fileIsOpen_)
			{
			archiveFile_->Open();
			archiveFile_->Seek(asset.archiveFile_->Tell());
			}
		}

	}


//*** Copy Operator ***

const Asset& Asset::operator=(const Asset& asset)
	{
	if (file_)
		{
		if (fileIsOpen_)
			{
			file_->Close();
			}
		delete file_;
		}

	if (archiveFile_)
		{
		if (fileIsOpen_)
			{
			archiveFile_->Close();
			}
		delete archiveFile_;
		}

	dynamicBuffer_=asset.dynamicBuffer_;
	staticBuffer_=asset.staticBuffer_;
	fileIsOpen_=asset.fileIsOpen_;
	readOnly_=asset.readOnly_;
	file_=0;
	if (asset.file_)
		{
		const char* filename=asset.file_->GetPath();
		if (Platform::GetPlatform_FileSystem())
			{
			file_=file_=Platform::GetPlatform_FileSystem()->CreateFileObject(filename);
			}
		if (fileIsOpen_)
			{
			file_->Open();
			file_->SetPosition(asset.file_->GetPosition());
			}
		}

	if (asset.archiveFile_)
		{
		archiveFile_=new ArchiveFile(asset.archiveFile_);
		if (fileIsOpen_)
			{
			archiveFile_->Open();
			archiveFile_->Seek(asset.archiveFile_->Tell());
			}
		}
	return *this;
	}


//*** GetFilename ***

const Filename& Asset::GetFilename() const
	{
	return filename_;
	}


//*** Open ***

bool Asset::Open() const
	{
	if (archiveFile_ && !fileIsOpen_)
		{
		fileIsOpen_=archiveFile_->Open();
		readOnly_=true;
		return fileIsOpen_;
		}

	if (dynamicBuffer_)
		{
		dynamicBuffer_->SetPosition(0);
		readOnly_=true;
		fileIsOpen_=true;
		return fileIsOpen_;
		}

	if (staticBuffer_)
		{
		staticBuffer_->SetPosition(0);
		readOnly_=true;
		fileIsOpen_=true;
		return fileIsOpen_;
		}

	if (file_ && !fileIsOpen_)
		{
		fileIsOpen_=file_->Open();
		readOnly_=true;
		return fileIsOpen_;
		}

	return false;
	}


//*** Create ***

bool Asset::Create()
	{
	if (archiveFile_ && !fileIsOpen_)
		{
		readOnly_=true;
		return false;
		}

	if (dynamicBuffer_)
		{
		readOnly_=false;
		return true;
		}

	if (staticBuffer_)
		{
		readOnly_=true;
		return false;
		}

	if (file_ && !fileIsOpen_)
		{
		fileIsOpen_=file_->Create();
		readOnly_=!fileIsOpen_;
		return fileIsOpen_;
		}

	readOnly_=true;
	return false;
	}


//*** Close ***

bool Asset::Close() const
	{
	readOnly_=true;

	if (archiveFile_ && fileIsOpen_)
		{
		fileIsOpen_=false;
		return archiveFile_->Close();
		}

	if (dynamicBuffer_)
		{
		return true;
		}

	if (staticBuffer_)
		{
		return true;
		}

	if (file_ && fileIsOpen_)
		{
		fileIsOpen_=false;
		return file_->Close();
		}

	return false;
	}


//*** GetSize ***

int Asset::GetSize() const
	{
	if (archiveFile_)
		{
		return archiveFile_->GetSize();
		}

	if (dynamicBuffer_)
		{
		return dynamicBuffer_->GetSize();
		}

	if (staticBuffer_)
		{
		return staticBuffer_->GetSize();
		}

	if (file_)
		{
		int size=0;
		if (fileIsOpen_)
		{
			int currentPosition=file_->GetPosition();
			file_->SetPosition(0,Platform_FileSystem_File::ORIGIN_END);
			size=file_->GetPosition();
			file_->SetPosition(currentPosition,Platform_FileSystem_File::ORIGIN_START);
		}
		else
		{
			if (file_->Open())
			{
				file_->SetPosition(0,Platform_FileSystem_File::ORIGIN_END);
				size=file_->GetPosition();
				file_->Close();
			}
		}
		return size;
		}

	return  0;
	}


//*** Seek ***

int Asset::Seek(int offset, SeekOrigin origin) const
	{
	if (archiveFile_)
		{
		switch (origin)
			{
			case SEEK_FROM_START:
				{
				return archiveFile_->Seek(offset,ArchiveFile::SEEK_FROM_START);
				} break;
			case SEEK_FROM_CURRENT:
				{
				return archiveFile_->Seek(offset,ArchiveFile::SEEK_FROM_CURRENT);
				} break;
			case SEEK_FROM_END:
				{
				return archiveFile_->Seek(offset,ArchiveFile::SEEK_FROM_END);
				} break;
			}
		return -1;
		}

	if (dynamicBuffer_)
		{
		switch (origin)
			{
			case SEEK_FROM_START:
				{
				dynamicBuffer_->SetPosition(offset);
				return dynamicBuffer_->GetPosition();
				} break;
			case SEEK_FROM_CURRENT:
				{
				dynamicBuffer_->SetPosition(dynamicBuffer_->GetPosition()+offset);
				return dynamicBuffer_->GetPosition();
				} break;
			case SEEK_FROM_END:
				{
				dynamicBuffer_->SetPosition(dynamicBuffer_->GetSize()-offset);
				return dynamicBuffer_->GetPosition();
				} break;
			}
		return -1;
		}

	if (staticBuffer_)
		{
		switch (origin)
			{
			case SEEK_FROM_START:
				{
				staticBuffer_->SetPosition(offset);
				return staticBuffer_->GetPosition();
				} break;
			case SEEK_FROM_CURRENT:
				{
				staticBuffer_->SetPosition(staticBuffer_->GetPosition()+offset);
				return staticBuffer_->GetPosition();
				} break;
			case SEEK_FROM_END:
				{
				staticBuffer_->SetPosition(staticBuffer_->GetSize()-offset);
				return staticBuffer_->GetPosition();
				} break;
			}
		return -1;
		}

	if (file_)
		{
		Platform_FileSystem_File::SetPositionOrigin fileOrigin=Platform_FileSystem_File::ORIGIN_START;
		switch (origin)
			{
			case SEEK_FROM_START:
				{
				fileOrigin=Platform_FileSystem_File::ORIGIN_START;
				} break;
			case SEEK_FROM_CURRENT:
				{
				fileOrigin=Platform_FileSystem_File::ORIGIN_CURRENT;
				} break;
			case SEEK_FROM_END:
				{
				fileOrigin=Platform_FileSystem_File::ORIGIN_END;
				} break;
			}
		return file_->SetPosition(offset,fileOrigin);
		}

	return  -1;
	}


//*** Tell ***

int Asset::Tell() const
	{
	if (archiveFile_)
		{
		return archiveFile_->Tell();
		}


	if (dynamicBuffer_)
		{
		return dynamicBuffer_->GetPosition();
		}

	if (staticBuffer_)
		{
		return staticBuffer_->GetPosition();
		}

	if (file_)
		{
		return file_->GetPosition();
		}

	return 0;
	}


//*** WRITEMACRO ***

// This macro defines the code that is to be executed for all of the overloaded Write methods
// The code is identical for all the different Write methods, only the input parameters change,
// so it makes more sense to use a macro rather than copy-paste. This macro is undefined as
// soon as it is not needed anymore

#define WRITEMACRO()								\
	Assert(!readOnly_,"Asset is read only!");		\
	if (readOnly_)									\
		{											\
		return 0;									\
		}											\
	if (dynamicBuffer_)								\
		return dynamicBuffer_->Write(value,count);	\
	if (file_)										\
		return file_->Write(value,count);			\
	return 0;										\


//** Write methods */

int Asset::Write(const char* value, int count)
	{
	WRITEMACRO();
	}

int Asset::Write(const short* value, int count)
	{
	WRITEMACRO();
	}

int Asset::Write(const int* value, int count)
	{
	WRITEMACRO();
	}

int Asset::Write(const long* value, int count)
	{
	WRITEMACRO();
	}

int Asset::Write(const unsigned char* value, int count)
	{
	WRITEMACRO();
	}

int Asset::Write(const unsigned short* value, int count)
	{
	WRITEMACRO();
	}

int Asset::Write(const unsigned int* value, int count)
	{
	WRITEMACRO();
	}

int Asset::Write(const unsigned long* value, int count)
	{
	WRITEMACRO();
	}

int Asset::Write(const float* value, int count)
	{
	WRITEMACRO();
	}

int Asset::Write(const double* value, int count)
	{
	WRITEMACRO();
	}

int Asset::Write(const bool* value, int count)
	{
	WRITEMACRO();
	}

// No need for the write macro anymore, so undefine it
#undef WRITEMACRO


//*** READMACRO ***

// This macro defines the code that is to be executed for all of the overloaded Read methods
// The code is identical for all the different Read methods, only the input parameters change,
// so it makes more sense to use a macro rather than copy-paste. This macro is undefined as
// soon as it is not needed anymore

#define READMACRO()									\
	if (archiveFile_)								\
		return archiveFile_->Read(value,count);		\
	if (dynamicBuffer_)								\
		return dynamicBuffer_->Read(value,count);	\
	if (staticBuffer_)								\
		return staticBuffer_->Read(value,count);	\
	if (file_)										\
		return file_->Read(value,count);			\
	return 0;										\


//** Read methods */


int Asset::Read(char* value, int count) const
	{
	READMACRO();
	}

int Asset::Read(short* value, int count) const
	{
	READMACRO();
	}

int Asset::Read(int* value, int count) const
	{
	READMACRO();
	}

int Asset::Read(long* value, int count) const
	{
	READMACRO();
	}

int Asset::Read(unsigned char* value, int count) const
	{
	READMACRO();
	}

int Asset::Read(unsigned short* value, int count) const
	{
	READMACRO();
	}

int Asset::Read(unsigned int* value, int count) const
	{
	READMACRO();
	}

int Asset::Read(unsigned long* value, int count) const
	{
	READMACRO();
	}

int Asset::Read(float* value, int count) const
	{
	READMACRO();
	}

int Asset::Read(double* value, int count) const
	{
	READMACRO();
	}

int Asset::Read(bool* value, int count) const
	{
	READMACRO();
	}

// No need for the read macro anymore, so undefine it
#undef READMACRO


}; // namespace pixie
