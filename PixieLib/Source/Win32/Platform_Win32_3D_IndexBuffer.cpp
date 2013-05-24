//*** Platform_Win32_3D_IndexBuffer.cpp ***

#include "Platform_Win32_3D_IndexBuffer.h"
#include "Platform_Win32_3D_Technology.h"
#include "Platform_Win32_3D.h"
#include "StandardLibrary.h"
#include "Debug.h"


//*** Constructor ***

Platform_Win32_3D_IndexBuffer::Platform_Win32_3D_IndexBuffer(Platform_Win32_3D* win32_3d, int indexCount, bool dynamic):
	win32_3d_(win32_3d),
	indexCount_(indexCount),
	dynamic_(dynamic),
	indexData_(0),
	locked_(false),
	technologyIndexBuffer_(0)
	{
	indexData_=new unsigned short[indexCount_];
	}


//*** Destructor ***

Platform_Win32_3D_IndexBuffer::~Platform_Win32_3D_IndexBuffer()
	{
	delete[] indexData_;

	if (technologyIndexBuffer_)
		{
		delete technologyIndexBuffer_;
		technologyIndexBuffer_=0;
		}

	win32_3d_->IndexBufferDeleted(this);
	}


//*** Reset ***

void Platform_Win32_3D_IndexBuffer::Reset(Platform_Win32_3D_Technology* technology)
	{
	bool copyData=false;

	if (technologyIndexBuffer_)
		{
		delete technologyIndexBuffer_;
		technologyIndexBuffer_=0;
		copyData=true;	// If the buffer was recreated rather than created initially, we need to copy the data to the new buffer
		}

	technologyIndexBuffer_=technology->CreateIndexBuffer(indexCount_,dynamic_);
	if (copyData)
		{
		// Locking and unlocking will force a rewrite of the buffer data
		Lock();
		Unlock();
		}
	}


//*** Lock ***

void Platform_Win32_3D_IndexBuffer::Lock(int startIndex, int indexCount)
	{
	Assert(!locked_,"Index buffer is already locked");
	Assert(technologyIndexBuffer_,"No technology instance!");
	if (locked_ || !technologyIndexBuffer_)
		{
		return;
		}

	technologyIndexBuffer_->Lock();
	locked_=true;
	}


//*** Unlock ***

void Platform_Win32_3D_IndexBuffer::Unlock()
	{
	Assert(locked_,"Index buffer is not locked");
	Assert(technologyIndexBuffer_,"No technology instance!");
	if (!locked_ || !technologyIndexBuffer_)
		{
		return;
		}

	// Copy the data to the actual index buffer
	int indexStride=technologyIndexBuffer_->GetIndexStride();
	if (indexStride==sizeof(unsigned short))
		{
		MemCpy(technologyIndexBuffer_->GetIndexData(),indexData_,sizeof(unsigned short)*indexCount_);
		}
	else
		{
		unsigned char* target=reinterpret_cast<unsigned char*>(technologyIndexBuffer_->GetIndexData());
		unsigned short* source=indexData_;
		for (int i=0; i<indexCount_; i++)
			{
			unsigned short* indexBuffer=reinterpret_cast<unsigned short*>(target);
			*indexBuffer=*source;
			source++;
			target+=indexStride;
			}
		}

	technologyIndexBuffer_->Unlock();
	locked_=false;
	}


//*** GetIndexData ***

unsigned short* Platform_Win32_3D_IndexBuffer::GetIndexData()
	{
	Assert(locked_,"Index buffer is not locked");
	Assert(technologyIndexBuffer_,"No technology instance!");
	if (!locked_ || !technologyIndexBuffer_)
		{
		return 0;
		}

	return indexData_;
	}


//*** GetIndexStride ***

int Platform_Win32_3D_IndexBuffer::GetIndexStride()
	{
	Assert(locked_,"Index buffer is not locked");
	Assert(technologyIndexBuffer_,"No technology instance!");
	if (!locked_ || !technologyIndexBuffer_)
		{
		return 0;
		}

	return sizeof(unsigned short);
	}

		
//*** Bind ***

void Platform_Win32_3D_IndexBuffer::Bind()
	{
	Assert(!locked_,"Index buffer is locked");
	Assert(technologyIndexBuffer_,"No technology instance!");
	if (locked_ || !technologyIndexBuffer_)
		{
		return;
		}

	technologyIndexBuffer_->Bind();
	
	win32_3d_->IndexBufferBound(this);
	}	

