//*** Platform_Win32_3D_Texture.cpp ***

#include "Platform_Win32_3D_Texture.h"
#include "Platform_Win32_3D_Technology.h"
#include "Platform_Win32_3D.h"
#include "Image.h"
#include "Debug.h"
#include "Asset.h"
#include "StandardLibrary.h"

//*** Constructor ***

Platform_Win32_3D_Texture::Platform_Win32_3D_Texture(Platform_Win32_3D* win32_3d, const Asset& asset):
	win32_3d_(win32_3d),
	width_(0),
	height_(0),
	size_(0),
	textureData_(0),
	technologyTexture_(0)
	{
	size_=asset.GetSize();
	textureData_=static_cast<unsigned char*>(Malloc(size_));
	if (asset.Open())
		{
		asset.Read(textureData_,size_);
		asset.Close();
		}
	}


//*** Constructor ***

Platform_Win32_3D_Texture::Platform_Win32_3D_Texture(Platform_Win32_3D* win32_3d, const Image& image):
	win32_3d_(win32_3d),
	width_(0),
	height_(0),
	size_(0),
	textureData_(0),
	technologyTexture_(0)
	{
	width_=image.GetWidth();
	height_=image.GetHeight();
	textureData_=static_cast<unsigned char*>(Malloc(width_*height_*sizeof(unsigned int)));
	for (int y=0; y<height_; y++)
		{
		for (int x=0; x<width_; x++)
			{
			(reinterpret_cast<unsigned int*>(textureData_))[x+y*width_]=image.GetPixel(x,y);
			}
		}
	}


//*** Destructor ***

Platform_Win32_3D_Texture::~Platform_Win32_3D_Texture()
	{
	if (technologyTexture_)
		{
		delete technologyTexture_;
		technologyTexture_=0;
		}

	Free(textureData_);

	win32_3d_->TextureDeleted(this);
	}


//*** Reset ***

void Platform_Win32_3D_Texture::Reset(Platform_Win32_3D_Technology* technology)
	{
	if (technologyTexture_)
		{
		delete technologyTexture_;
		technologyTexture_=0;
		}

	if (size_>0)
		{
		technologyTexture_=technology->CreateTexture(textureData_,size_);
		}
	else
		{
		technologyTexture_=technology->CreateTexture(width_,height_,textureData_);
		}
	}


//*** Bind ***

void Platform_Win32_3D_Texture::Bind(int stage)
	{
	Assert(technologyTexture_,"No technology instance!");
	if (!technologyTexture_)
		{
		return;
		}

	technologyTexture_->Bind(stage);

	win32_3d_->TextureBound(stage,this);
	}


//*** HasAlphaChannel ***

bool Platform_Win32_3D_Texture::HasAlphaChannel()
	{
	Assert(technologyTexture_,"No technology instance!");
	if (!technologyTexture_)
		{
		return 0;
		}

	return technologyTexture_->HasAlphaChannel();
	}


//*** GetWidth ***

int Platform_Win32_3D_Texture::GetWidth()
	{
	Assert(technologyTexture_,"No technology instance!");
	if (!technologyTexture_)
		{
		return 0;
		}

	return technologyTexture_->GetWidth();
	}


//*** GetHeight ***

int Platform_Win32_3D_Texture::GetHeight()
	{
	Assert(technologyTexture_,"No technology instance!");
	if (!technologyTexture_)
		{
		return 0;
		}

	return technologyTexture_->GetHeight();
	}


//*** Lock ***

void Platform_Win32_3D_Texture::Lock()
	{
	Assert(technologyTexture_,"No technology instance!");
	if (!technologyTexture_)
		{
		return;
		}

	technologyTexture_->Lock();
	}


//*** Unlock ***

void Platform_Win32_3D_Texture::Unlock()
	{
	Assert(technologyTexture_,"No technology instance!");
	if (!technologyTexture_)
		{
		return;
		}

	technologyTexture_->Unlock();
	}


//*** GetTexelData ***

unsigned char* Platform_Win32_3D_Texture::GetTexelData()
	{
	Assert(technologyTexture_,"No technology instance!");
	if (!technologyTexture_)
		{
		return 0;
		}

	return technologyTexture_->GetTexelData();
	}


//*** GetPitch ***

int Platform_Win32_3D_Texture::GetPitch()
	{
	Assert(technologyTexture_,"No technology instance!");
	if (!technologyTexture_)
		{
		return 0;
		}

	return technologyTexture_->GetPitch();
	}
