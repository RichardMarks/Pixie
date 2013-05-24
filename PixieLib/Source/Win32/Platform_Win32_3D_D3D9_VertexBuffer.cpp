//*** Platform_Win32_3D_D3D9_VertexBuffer.cpp ***

#include "Platform_Win32_3D_D3D9_VertexBuffer.h"
#include "Debug.h"

#define WIN32_LEAN_AND_MEAN 
#define VC_EXTRALEAN
#include <d3d9.h>

//*** Constructor ***

Platform_Win32_3D_D3D9_VertexBuffer::Platform_Win32_3D_D3D9_VertexBuffer(IDirect3DDevice9* device, int vertexFormat, int vertexCount, bool dynamic):
	device_(device),
	vertexFormat_(vertexFormat),
	vertexCount_(vertexCount ),
	dynamic_(dynamic),
	vertexSize_(0),
	d3dFVF_(0),
	d3dVertexBuffer_(0),
	lockedData_(0)
	{
	Assert(ValidateVertexFormat(vertexFormat),"Invalid vertex format");
	if (!ValidateVertexFormat(vertexFormat))
		{
		return;
		}
	d3dFVF_=CalculateFVF(vertexFormat);
	vertexSize_=CalculateSize(vertexFormat);
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
	HRESULT hr=device->CreateVertexBuffer(vertexSize_*vertexCount_,usage,d3dFVF_,pool,&d3dVertexBuffer_,0);
	Assert(SUCCEEDED(hr),"Failed to create vertex buffer");
	}


//*** Destructor ***

Platform_Win32_3D_D3D9_VertexBuffer::~Platform_Win32_3D_D3D9_VertexBuffer()
	{
	d3dVertexBuffer_->Release();
	}


//*** Lock ***

void Platform_Win32_3D_D3D9_VertexBuffer::Lock(int startVertex, int vertexCount)
	{
	Assert(!lockedData_,"Vertex buffer is aready locked");
	if (lockedData_)
		{
		return;
		}

	DWORD flags=0;
	if (dynamic_)
		{
		flags=D3DLOCK_DISCARD/*|D3DLOCK_NOOVERWRITE*/;
		}
	HRESULT hr=d3dVertexBuffer_->Lock(0,0,reinterpret_cast<void**>(&lockedData_),flags);
	Assert(SUCCEEDED(hr),"Failed to lock vertex buffer");
	}


//*** Unlock ***

void Platform_Win32_3D_D3D9_VertexBuffer::Unlock()
	{
	Assert(lockedData_,"Vertex buffer is not locked");
	if (!lockedData_)
		{
		return;
		}

	HRESULT hr=d3dVertexBuffer_->Unlock();
	Assert(SUCCEEDED(hr),"Failed to unlock vertex buffer");
	lockedData_=0;
	}


//*** GetPositionData ***

float* Platform_Win32_3D_D3D9_VertexBuffer::GetPositionData()
	{
	Assert(lockedData_,"Vertex buffer is not locked");
	Assert(vertexFormat_ & VertexFormat_Position,"Component does not exist in vertexbuffer");
	if (!lockedData_ || !(vertexFormat_ & VertexFormat_Position))
		{
		return 0;
		}

	return reinterpret_cast<float*>(lockedData_);
	}


//*** GetPositionStride ***

int Platform_Win32_3D_D3D9_VertexBuffer::GetPositionStride()
	{
	Assert(lockedData_,"Vertex buffer is not locked");
	if (!lockedData_)
		{
		return 0;
		}

	return vertexSize_;
	}


//*** GetScreenPositionData ***

float* Platform_Win32_3D_D3D9_VertexBuffer::GetScreenPositionData()
	{
	Assert(lockedData_,"Vertex buffer is not locked");
	Assert(vertexFormat_ & VertexFormat_ScreenPosition,"Component does not exist in vertexbuffer");
	if (!lockedData_ || !(vertexFormat_ & VertexFormat_ScreenPosition))
		{
		return 0;
		}

	return reinterpret_cast<float*>(lockedData_);
	}


//*** GetScreenPositionStride ***

int Platform_Win32_3D_D3D9_VertexBuffer::GetScreenPositionStride()
	{
	Assert(lockedData_,"Vertex buffer is not locked");
	if (!lockedData_)
		{
		return 0;
		}

	return vertexSize_;
	}


//*** GetNormalData ***

float* Platform_Win32_3D_D3D9_VertexBuffer::GetNormalData()
	{
	Assert(lockedData_,"Vertex buffer is not locked");
	Assert(vertexFormat_ & VertexFormat_Normal,"Component does not exist in vertexbuffer");
	if (!lockedData_ || !(vertexFormat_ & VertexFormat_Normal))
		{
		return 0;
		}

	unsigned char* data=lockedData_;
	if (vertexFormat_ & VertexFormat_Position)
		{
		data+=sizeof(float)*3;
		}
	if (vertexFormat_ & VertexFormat_ScreenPosition)
		{
		data+=sizeof(float)*4;
		}
	return reinterpret_cast<float*>(data);
	}


//*** GetNormalStride ***

int Platform_Win32_3D_D3D9_VertexBuffer::GetNormalStride()
	{
	Assert(lockedData_,"Vertex buffer is not locked");
	if (!lockedData_)
		{
		return 0;
		}

	return vertexSize_;
	}


//*** GetDiffuseData ***

unsigned int* Platform_Win32_3D_D3D9_VertexBuffer::GetDiffuseData()
	{
	Assert(lockedData_,"Vertex buffer is not locked");
	Assert(vertexFormat_ & VertexFormat_Diffuse,"Component does not exist in vertexbuffer");
	if (!lockedData_ || !(vertexFormat_ & VertexFormat_Diffuse))
		{
		return 0;
		}

	unsigned char* data=lockedData_;
	if (vertexFormat_ & VertexFormat_Position)
		{
		data+=sizeof(float)*3;
		}
	if (vertexFormat_ & VertexFormat_ScreenPosition)
		{
		data+=sizeof(float)*4;
		}
	if (vertexFormat_ & VertexFormat_Normal)
		{
		data+=sizeof(float)*3;
		}
	return reinterpret_cast<unsigned int*>(data);
	}


//*** GetDiffuseStride ***

int Platform_Win32_3D_D3D9_VertexBuffer::GetDiffuseStride()
	{
	Assert(lockedData_,"Vertex buffer is not locked");
	if (!lockedData_)
		{
		return 0;
		}

	return vertexSize_;
	}


//*** GetSpecularData ***

unsigned int* Platform_Win32_3D_D3D9_VertexBuffer::GetSpecularData()
	{
	Assert(lockedData_,"Vertex buffer is not locked");
	Assert(vertexFormat_ & VertexFormat_Specular,"Component does not exist in vertexbuffer");
	if (!lockedData_ || !(vertexFormat_ & VertexFormat_Specular))
		{
		return 0;
		}

	unsigned char* data=lockedData_;
	if (vertexFormat_ & VertexFormat_Position)
		{
		data+=sizeof(float)*3;
		}
	if (vertexFormat_ & VertexFormat_ScreenPosition)
		{
		data+=sizeof(float)*4;
		}
	if (vertexFormat_ & VertexFormat_Normal)
		{
		data+=sizeof(float)*3;
		}
	if (vertexFormat_ & VertexFormat_Diffuse)
		{
		data+=sizeof(unsigned int);
		}
	return reinterpret_cast<unsigned int*>(data);
	}


//*** GetSpecularStride ***

int Platform_Win32_3D_D3D9_VertexBuffer::GetSpecularStride()
	{
	Assert(lockedData_,"Vertex buffer is not locked");
	if (!lockedData_)
		{
		return 0;
		}

	return vertexSize_;
	}


//*** GetUVData ***

float* Platform_Win32_3D_D3D9_VertexBuffer::GetUVData(int textureSet)
	{
	Assert(lockedData_,"Vertex buffer is not locked");
	Assert(GetNumberOfTextureSets(vertexFormat_)>0,"Component does not exist in vertexbuffer");
	Assert(textureSet>=0 && textureSet<GetNumberOfTextureSets(vertexFormat_),"Invalid texture set index");
	if (!lockedData_ || GetNumberOfTextureSets(vertexFormat_)<=0 || textureSet<0 || textureSet>=GetNumberOfTextureSets(vertexFormat_))
		{
		return 0;
		}

	unsigned char* data=lockedData_;
	if (vertexFormat_ & VertexFormat_Position)
		{
		data+=sizeof(float)*3;
		}
	if (vertexFormat_ & VertexFormat_ScreenPosition)
		{
		data+=sizeof(float)*4;
		}
	if (vertexFormat_ & VertexFormat_Normal)
		{
		data+=sizeof(float)*3;
		}
	if (vertexFormat_ & VertexFormat_Diffuse)
		{
		data+=sizeof(unsigned int);
		}
	if (vertexFormat_ & VertexFormat_Specular)
		{
		data+=sizeof(unsigned int);
		}
	data+=sizeof(float)*2*textureSet;
	return reinterpret_cast<float*>(data);
	}


//*** GetUVStride ***

int Platform_Win32_3D_D3D9_VertexBuffer::GetUVStride(int textureSet)
	{
	Assert(lockedData_,"Vertex buffer is not locked");
	if (!lockedData_)
		{
		return 0;
		}

	return vertexSize_;
	}


//*** Bind ***

void Platform_Win32_3D_D3D9_VertexBuffer::Bind()
	{
	Assert(!lockedData_,"Vertex buffer is locked");
	if (lockedData_)
		{
		return;
		}

	device_->SetFVF(d3dFVF_);
	device_->SetStreamSource(0,d3dVertexBuffer_,0,vertexSize_);
	}



//*** ValidateVertexFormat ***

bool Platform_Win32_3D_D3D9_VertexBuffer::ValidateVertexFormat(int vertexFormat)
	{
	int texturesetFlags=0;
	
	if (vertexFormat & VertexFormat_1TextureSets)
		{
		texturesetFlags++;
		}

	if (vertexFormat & VertexFormat_2TextureSets)
		{
		texturesetFlags++;
		}

	if (vertexFormat & VertexFormat_3TextureSets)
		{
		texturesetFlags++;
		}

	if (vertexFormat & VertexFormat_4TextureSets)
		{
		texturesetFlags++;
		}

	if (vertexFormat & VertexFormat_5TextureSets)
		{
		texturesetFlags++;
		}

	if (vertexFormat & VertexFormat_6TextureSets)
		{
		texturesetFlags++;
		}

	if (vertexFormat & VertexFormat_7TextureSets)
		{
		texturesetFlags++;
		}

	if (vertexFormat & VertexFormat_8TextureSets)
		{
		texturesetFlags++;
		}
	
	if (texturesetFlags>1)
		{
		return false;
		}

	if (!(vertexFormat & VertexFormat_Position) && !(vertexFormat & VertexFormat_ScreenPosition))
		{
		return false;
		}

	if ((vertexFormat & VertexFormat_Position) && (vertexFormat & VertexFormat_ScreenPosition))
		{
		return false;
		}

	if ((vertexFormat & VertexFormat_ScreenPosition) && (vertexFormat & VertexFormat_Normal))
		{
		return false;
		}

	return true;
	}


//*** CalculateFVF ***

unsigned long Platform_Win32_3D_D3D9_VertexBuffer::CalculateFVF(int vertexFormat)
	{
	unsigned FVF=0;

	if (vertexFormat & VertexFormat_Position)
		{
		FVF|=D3DFVF_XYZ;
		}

	if (vertexFormat & VertexFormat_ScreenPosition)
		{
		FVF|=D3DFVF_XYZRHW;
		}

	if (vertexFormat & VertexFormat_Normal)
		{
		FVF|=D3DFVF_NORMAL;
		}

	if (vertexFormat & VertexFormat_Diffuse)
		{
		FVF|=D3DFVF_DIFFUSE;
		}

	if (vertexFormat & VertexFormat_Specular)
		{
		FVF|=D3DFVF_SPECULAR;
		}

	if (vertexFormat & VertexFormat_1TextureSets)
		{
		FVF|=D3DFVF_TEX1;
		}

	if (vertexFormat & VertexFormat_2TextureSets)
		{
		FVF|=D3DFVF_TEX2;
		}

	if (vertexFormat & VertexFormat_3TextureSets)
		{
		FVF|=D3DFVF_TEX3;
		}

	if (vertexFormat & VertexFormat_4TextureSets)
		{
		FVF|=D3DFVF_TEX4;
		}

	if (vertexFormat & VertexFormat_5TextureSets)
		{
		FVF|=D3DFVF_TEX5;
		}

	if (vertexFormat & VertexFormat_6TextureSets)
		{
		FVF|=D3DFVF_TEX6;
		}

	if (vertexFormat & VertexFormat_7TextureSets)
		{
		FVF|=D3DFVF_TEX7;
		}

	if (vertexFormat & VertexFormat_8TextureSets)
		{
		FVF|=D3DFVF_TEX8;
		}

	return FVF;
	}


//*** CalculateSize ***

int Platform_Win32_3D_D3D9_VertexBuffer::CalculateSize(int vertexFormat)
	{
	int size=0;


	if (vertexFormat & VertexFormat_Position)
		{
		size+=sizeof(float)*3;
		}

	if (vertexFormat & VertexFormat_ScreenPosition)
		{
		size+=sizeof(float)*4;
		}

	if (vertexFormat & VertexFormat_Normal)
		{
		size+=sizeof(float)*3;
		}

	if (vertexFormat & VertexFormat_Diffuse)
		{
		size+=sizeof(unsigned int);
		}

	if (vertexFormat & VertexFormat_Specular)
		{
		size+=sizeof(unsigned int);
		}

	if (vertexFormat & VertexFormat_1TextureSets)
		{
		size+=sizeof(float)*2;
		}

	if (vertexFormat & VertexFormat_2TextureSets)
		{
		size+=sizeof(float)*2*2;
		}

	if (vertexFormat & VertexFormat_3TextureSets)
		{
		size+=sizeof(float)*2*3;
		}

	if (vertexFormat & VertexFormat_4TextureSets)
		{
		size+=sizeof(float)*2*4;
		}

	if (vertexFormat & VertexFormat_5TextureSets)
		{
		size+=sizeof(float)*2*5;
		}

	if (vertexFormat & VertexFormat_6TextureSets)
		{
		size+=sizeof(float)*2*6;
		}

	if (vertexFormat & VertexFormat_7TextureSets)
		{
		size+=sizeof(float)*2*7;
		}

	if (vertexFormat & VertexFormat_8TextureSets)
		{
		size+=sizeof(float)*2*8;
		}

	return size;
	}


//*** GetNumberOfTextureSets ***

int Platform_Win32_3D_D3D9_VertexBuffer::GetNumberOfTextureSets(int vertexFormat)
	{
	if (vertexFormat & VertexFormat_1TextureSets)
		{
		return 1;
		}

	if (vertexFormat & VertexFormat_2TextureSets)
		{
		return 2;
		}

	if (vertexFormat & VertexFormat_3TextureSets)
		{
		return 3;
		}

	if (vertexFormat & VertexFormat_4TextureSets)
		{
		return 4;
		}

	if (vertexFormat & VertexFormat_5TextureSets)
		{
		return 5;
		}

	if (vertexFormat & VertexFormat_6TextureSets)
		{
		return 6;
		}

	if (vertexFormat & VertexFormat_7TextureSets)
		{
		return 7;
		}

	if (vertexFormat & VertexFormat_8TextureSets)
		{
		return 8;
		}

	return 0;
	}