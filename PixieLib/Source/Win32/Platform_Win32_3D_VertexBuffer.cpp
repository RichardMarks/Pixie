//*** Platform_Win32_3D_VertexBuffer.cpp ***

#include "Platform_Win32_3D_VertexBuffer.h"
#include "Platform_Win32_3D_Technology.h"
#include "Platform_Win32_3D.h"
#include "StandardLibrary.h"
#include "Debug.h"


//*** Constructor ***

Platform_Win32_3D_VertexBuffer::Platform_Win32_3D_VertexBuffer(Platform_Win32_3D* win32_3d, int vertexFormat, int vertexCount, bool dynamic):
	win32_3d_(win32_3d),
	vertexFormat_(vertexFormat),
	vertexCount_(vertexCount ),
	dynamic_(dynamic),
	vertexSize_(0),
	vertexData_(0),
	locked_(false),
	technologyVertexBuffer_(0)
	{
	Assert(ValidateVertexFormat(vertexFormat),"Invalid vertex format");
	if (!ValidateVertexFormat(vertexFormat))
		{
		return;
		}

	vertexSize_=CalculateSize(vertexFormat);
	vertexData_=static_cast<unsigned char*>(Malloc(vertexSize_*vertexCount_));
	}


//*** Destructor ***

Platform_Win32_3D_VertexBuffer::~Platform_Win32_3D_VertexBuffer()
	{
	if (vertexData_)
		{
		Free(vertexData_);
		vertexData_=0;
		}

	if (technologyVertexBuffer_)
		{
		delete technologyVertexBuffer_;
		technologyVertexBuffer_=0;
		}

	win32_3d_->VertexBufferDeleted(this);
	}


//*** Reset ***

void Platform_Win32_3D_VertexBuffer::Reset(Platform_Win32_3D_Technology* technology)
	{
	bool copyData=false;

	if (technologyVertexBuffer_)
		{
		delete technologyVertexBuffer_;
		technologyVertexBuffer_=0;
		copyData=true;	// If the buffer was recreated rather than created initially, we need to copy the data to the new buffer
		}

	technologyVertexBuffer_=technology->CreateVertexBuffer(vertexFormat_,vertexCount_,dynamic_);
	if (copyData)
		{
		// Locking and unlocking will force a rewrite of the buffer data
		Lock();
		Unlock();
		}
	}


//*** Lock ***

void Platform_Win32_3D_VertexBuffer::Lock(int startVertex, int vertexCount)
	{
	Assert(!locked_,"Vertex buffer is aready locked");
	Assert(technologyVertexBuffer_,"No technology instance!");
	if (locked_ || !technologyVertexBuffer_)
		{
		return;
		}

	technologyVertexBuffer_->Lock();
	locked_=true;
	}


//*** Unlock ***

void Platform_Win32_3D_VertexBuffer::Unlock()
	{
	Assert(locked_,"Vertex buffer is not locked");
	Assert(technologyVertexBuffer_,"No technology instance!");
	if (!locked_ || !technologyVertexBuffer_)
		{
		return;
		}

	// Copy the data to the actual vertex buffer. For the current technologies, we know we can do this with
	// a straight MemCpy, but that might change if new implementations are added, and in that case, this code 
	// needs to become more complex
	void* data=0; 
	if (vertexFormat_ & VertexFormat_Position)
		{
		data=technologyVertexBuffer_->GetPositionData();
		}
	else
		{
		data=technologyVertexBuffer_->GetScreenPositionData();
		}
	MemCpy(data,vertexData_,vertexSize_*vertexCount_);

	technologyVertexBuffer_->Unlock();
	locked_=false;
	}


//*** GetPositionData ***

float* Platform_Win32_3D_VertexBuffer::GetPositionData()
	{
	Assert(locked_,"Vertex buffer is not locked");
	Assert(technologyVertexBuffer_,"No technology instance!");
	Assert(vertexFormat_ & VertexFormat_Position,"Component does not exist in vertexbuffer");
	if (!locked_ || !technologyVertexBuffer_ || !(vertexFormat_ & VertexFormat_Position))
		{
		return 0;
		}

	return reinterpret_cast<float*>(vertexData_);
	}


//*** GetPositionStride ***

int Platform_Win32_3D_VertexBuffer::GetPositionStride()
	{
	Assert(locked_,"Vertex buffer is not locked");
	Assert(technologyVertexBuffer_,"No technology instance!");
	if (!locked_ || !technologyVertexBuffer_)
		{
		return 0;
		}

	return vertexSize_;
	}


//*** GetScreenPositionData ***

float* Platform_Win32_3D_VertexBuffer::GetScreenPositionData()
	{
	Assert(locked_,"Vertex buffer is not locked");
	Assert(technologyVertexBuffer_,"No technology instance!");
	Assert(vertexFormat_ & VertexFormat_ScreenPosition,"Component does not exist in vertexbuffer");
	if (!locked_ || !technologyVertexBuffer_ || !(vertexFormat_ & VertexFormat_ScreenPosition))
		{
		return 0;
		}

	unsigned char* data=vertexData_;
	if (vertexFormat_ & VertexFormat_Position)
		{
		data+=sizeof(float)*3;
		}
	return reinterpret_cast<float*>(data);
	}


//*** GetScreenPositionStride ***

int Platform_Win32_3D_VertexBuffer::GetScreenPositionStride()
	{
	Assert(locked_,"Vertex buffer is not locked");
	Assert(technologyVertexBuffer_,"No technology instance!");
	if (!locked_ || !technologyVertexBuffer_)
		{
		return 0;
		}

	return vertexSize_;
	}


//*** GetNormalData ***

float* Platform_Win32_3D_VertexBuffer::GetNormalData()
	{
	Assert(locked_,"Vertex buffer is not locked");
	Assert(technologyVertexBuffer_,"No technology instance!");
	Assert(vertexFormat_ & VertexFormat_Normal,"Component does not exist in vertexbuffer");
	if (!locked_ || !technologyVertexBuffer_ || !(vertexFormat_ & VertexFormat_Normal))
		{
		return 0;
		}

	unsigned char* data=vertexData_;
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

int Platform_Win32_3D_VertexBuffer::GetNormalStride()
	{
	Assert(locked_,"Vertex buffer is not locked");
	Assert(technologyVertexBuffer_,"No technology instance!");
	if (!locked_ || !technologyVertexBuffer_)
		{
		return 0;
		}

	return vertexSize_;
	}


//*** GetDiffuseData ***

unsigned int* Platform_Win32_3D_VertexBuffer::GetDiffuseData()
	{
	Assert(locked_,"Vertex buffer is not locked");
	Assert(technologyVertexBuffer_,"No technology instance!");
	Assert(vertexFormat_ & VertexFormat_Diffuse,"Component does not exist in vertexbuffer");
	if (!locked_ || !technologyVertexBuffer_ || !(vertexFormat_ & VertexFormat_Diffuse))
		{
		return 0;
		}

	unsigned char* data=vertexData_;
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

int Platform_Win32_3D_VertexBuffer::GetDiffuseStride()
	{
	Assert(locked_,"Vertex buffer is not locked");
	Assert(technologyVertexBuffer_,"No technology instance!");
	if (!locked_ || !technologyVertexBuffer_)
		{
		return 0;
		}

	return vertexSize_;
	}


//*** GetSpecularData ***

unsigned int* Platform_Win32_3D_VertexBuffer::GetSpecularData()
	{
	Assert(locked_,"Vertex buffer is not locked");
	Assert(technologyVertexBuffer_,"No technology instance!");
	Assert(vertexFormat_ & VertexFormat_Specular,"Component does not exist in vertexbuffer");
	if (!locked_ || !technologyVertexBuffer_ || !(vertexFormat_ & VertexFormat_Specular))
		{
		return 0;
		}

	unsigned char* data=vertexData_;
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

int Platform_Win32_3D_VertexBuffer::GetSpecularStride()
	{
	Assert(locked_,"Vertex buffer is not locked");
	Assert(technologyVertexBuffer_,"No technology instance!");
	if (!locked_ || !technologyVertexBuffer_)
		{
		return 0;
		}

	return vertexSize_;
	}


//*** GetUVData ***

float* Platform_Win32_3D_VertexBuffer::GetUVData(int textureSet)
	{
	Assert(locked_,"Vertex buffer is not locked");
	Assert(technologyVertexBuffer_,"No technology instance!");
	Assert(GetNumberOfTextureSets(vertexFormat_)>0,"Component does not exist in vertexbuffer");
	Assert(textureSet>=0 && textureSet<GetNumberOfTextureSets(vertexFormat_),"Invalid texture set index");
	if (!locked_ || !technologyVertexBuffer_ || GetNumberOfTextureSets(vertexFormat_)<=0 || textureSet<0 || textureSet>=GetNumberOfTextureSets(vertexFormat_))
		{
		return 0;
		}

	unsigned char* data=vertexData_;
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

int Platform_Win32_3D_VertexBuffer::GetUVStride(int textureSet)
	{
	Assert(locked_,"Vertex buffer is not locked");
	Assert(technologyVertexBuffer_,"No technology instance!");
	if (!locked_ || !technologyVertexBuffer_)
		{
		return 0;
		}

	return vertexSize_;
	}


//*** Bind ***

void Platform_Win32_3D_VertexBuffer::Bind()
	{
	Assert(technologyVertexBuffer_,"No technology instance!");
	Assert(!locked_,"Vertex buffer is locked");
	if (locked_ || !technologyVertexBuffer_)
		{
		return;
		}

	technologyVertexBuffer_->Bind();

	win32_3d_->VertexBufferBound(this);
	}



//*** ValidateVertexFormat ***

bool Platform_Win32_3D_VertexBuffer::ValidateVertexFormat(int vertexFormat)
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


//*** CalculateSize ***

int Platform_Win32_3D_VertexBuffer::CalculateSize(int vertexFormat)
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

int Platform_Win32_3D_VertexBuffer::GetNumberOfTextureSets(int vertexFormat)
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