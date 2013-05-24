//*** Platform_Win32_3D_D3D9_IndexBuffer.cpp ***

#include "Platform_Win32_3D_D3D9_IndexBuffer.h"
#include "Debug.h"

#define WIN32_LEAN_AND_MEAN 
#define VC_EXTRALEAN
#include <d3d9.h>

//*** Constructor ***

Platform_Win32_3D_D3D9_IndexBuffer::Platform_Win32_3D_D3D9_IndexBuffer(IDirect3DDevice9* device, int indexCount, bool dynamic):
	device_(device),
	indexCount_(indexCount ),
	dynamic_(dynamic),
	d3dIndexBuffer_(0),
	lockedData_(0)
	{
	DWORD usage=0;
	D3DPOOL pool=D3DPOOL_DEFAULT;
	if (dynamic)
		{
		usage=D3DUSAGE_DYNAMIC|D3DUSAGE_WRITEONLY;
		pool=D3DPOOL_DEFAULT;
		}
	else
		{
		usage=D3DUSAGE_WRITEONLY;
		pool=D3DPOOL_MANAGED;
		}
	HRESULT hr=device->CreateIndexBuffer(indexCount_*sizeof(unsigned short),usage,D3DFMT_INDEX16,pool,&d3dIndexBuffer_,0);
	Assert(SUCCEEDED(hr),"Failed to create index buffer");
	}


//*** Destructor ***

Platform_Win32_3D_D3D9_IndexBuffer::~Platform_Win32_3D_D3D9_IndexBuffer()
	{
	d3dIndexBuffer_->Release();
	}


//*** Lock ***

void Platform_Win32_3D_D3D9_IndexBuffer::Lock(int startIndex, int indexCount)
	{
	Assert(!lockedData_,"Index buffer is aready locked");
	if (lockedData_)
		{
		return;
		}

	DWORD flags=0;
	if (dynamic_)
		{
		flags=D3DLOCK_DISCARD|D3DLOCK_NOOVERWRITE;
		}
	HRESULT hr=d3dIndexBuffer_->Lock(0,0,reinterpret_cast<void**>(&lockedData_),flags);
	Assert(SUCCEEDED(hr),"Failed to lock vertex buffer");
	}


//*** Unlock ***

void Platform_Win32_3D_D3D9_IndexBuffer::Unlock()
	{
	Assert(lockedData_,"Index buffer is not locked");
	if (!lockedData_)
		{
		return;
		}

	HRESULT hr=d3dIndexBuffer_->Unlock();
	Assert(SUCCEEDED(hr),"Failed to unlock vertex buffer");
	lockedData_=0;
	}


//*** GetIndexData ***

unsigned short* Platform_Win32_3D_D3D9_IndexBuffer::GetIndexData()
	{
	Assert(lockedData_,"Index buffer is not locked");
	if (!lockedData_)
		{
		return 0;
		}

	return reinterpret_cast<unsigned short*>(lockedData_);
	}


//*** GetIndexStride ***

int Platform_Win32_3D_D3D9_IndexBuffer::GetIndexStride()
	{
	Assert(lockedData_,"Index buffer is not locked");
	if (!lockedData_)
		{
		return 0;
		}

	return sizeof(unsigned short);
	}

		
//*** Bind ***

void Platform_Win32_3D_D3D9_IndexBuffer::Bind()
	{
	Assert(!lockedData_,"Index buffer is locked");
	if (lockedData_)
		{
		return;
		}

	device_->SetIndices(d3dIndexBuffer_);
	}

