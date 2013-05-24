//*** Platform_Win32_3D_D3D9_Texture.cpp ***

#define WIN32_LEAN_AND_MEAN 
#define VC_EXTRALEAN
#include <d3d9.h>

#include "Platform_Win32_3D_D3D9_Texture.h"
#include "Debug.h"
#include "Asset.h"
#include "StandardLibrary.h"
#include "ColorHelper.h"


//*** Constructor ***

Platform_Win32_3D_D3D9_Texture::Platform_Win32_3D_D3D9_Texture(IDirect3DDevice9* device, int width, int height, void* data):
	device_(device),
	d3dTexture_(0),
	width_(width),
	height_(height),
	depth_(0),
	mipLevels_(0),
	format_(D3DFMT_UNKNOWN)
	{
	int count=0;
	int wc=width_;
	int hc=height_;
	while(wc>0 && hc>0)
		{
		count++;
		wc>>=1;
		hc>>=1;
		}

	device_->CreateTexture(width_,height_,count,0,D3DFMT_A8R8G8B8,D3DPOOL_MANAGED,&d3dTexture_,0);
	int mipLevelCount=d3dTexture_->GetLevelCount();
	int w=width_;
	int h=height_;

	unsigned int* temp=new unsigned int[width_*height_];
	MemCpy(temp,data,width_*height_*sizeof(unsigned int));

	for (int i=0; i<mipLevelCount; i++)
		{
		D3DLOCKED_RECT rect;
		d3dTexture_->LockRect(i,&rect,0,D3DLOCK_DISCARD);
		unsigned char* dest=static_cast<unsigned char*>(rect.pBits);
		
		if (rect.Pitch==(int)sizeof(unsigned int)*w)
			{
			MemCpy(dest,temp,sizeof(unsigned int)*w*h);
			}
		else
			{
			unsigned char* dat=reinterpret_cast<unsigned char*>(temp);
			for (int y=0; y<h; y++)
				{
				MemCpy(dest,dat,sizeof(unsigned int)*w);
				dat+=w;
				dest+=rect.Pitch;
				}
			}
		d3dTexture_->UnlockRect(i);
	
		if (i+1<mipLevelCount)
			{
			w/=2;
			h/=2;
			for (int y=0; y<h; y++)
				{
				for (int x=0; x<w; x++)
					{
					unsigned int c0=temp[x*2+0+(y*2+0)*(w*2)];
					unsigned int c1=temp[x*2+1+(y*2+0)*(w*2)];
					unsigned int c2=temp[x*2+0+(y*2+1)*(w*2)];
					unsigned int c3=temp[x*2+1+(y*2+1)*(w*2)];
					unsigned int a=(((c0 & 0xff000000)>>24)+((c1 & 0xff000000)>>24)+((c2 & 0xff000000)>>24)+((c3 & 0xff000000)>>24))>>2;
					unsigned int r=(((c0 & 0x00ff0000)>>16)+((c1 & 0x00ff0000)>>16)+((c2 & 0x00ff0000)>>16)+((c3 & 0x00ff0000)>>16))>>2;
					unsigned int g=(((c0 & 0x0000ff00)>> 8)+((c1 & 0x0000ff00)>> 8)+((c2 & 0x0000ff00)>> 8)+((c3 & 0x0000ff00)>> 8))>>2;
					unsigned int b=(((c0 & 0x000000ff)    )+((c1 & 0x000000ff)    )+((c2 & 0x000000ff)    )+((c3 & 0x000000ff)    ))>>2;
					temp[x+y*w]=RGBA32((unsigned char)r,(unsigned char)g,(unsigned char)b,(unsigned char)a);
					}
				}
			}
		}

	delete[] temp;
	}


//*** Constructor ***

Platform_Win32_3D_D3D9_Texture::Platform_Win32_3D_D3D9_Texture(IDirect3DDevice9* device, void* data, int size):
	device_(device),
	d3dTexture_(0),
	lockedData_(0),
	lockedSurface_(0),
	lockedPitch_(0),
	width_(0),
	height_(0),
	depth_(0),
	mipLevels_(0),
	format_(D3DFMT_UNKNOWN)
	{
	Assert(false,"Not implemented yet");
/*	D3DXIMAGE_INFO info;
	HRESULT hr=D3DXCreateTextureFromFileInMemoryEx(
		device_,
		data,
		size,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		0,
		D3DFMT_FROM_FILE,
		D3DPOOL_MANAGED,
		D3DX_FILTER_BOX,
		D3DX_FILTER_BOX,
		0,
		&info,
		0,
		&d3dTexture_
		);
	Assert(SUCCEEDED(hr),"Failed to create texture");
*/	}


//*** Destructor ***

Platform_Win32_3D_D3D9_Texture::~Platform_Win32_3D_D3D9_Texture()
	{
	if (d3dTexture_)
		{
		d3dTexture_->Release();
		}
	}

		
//*** Bind ***

void Platform_Win32_3D_D3D9_Texture::Bind(int stage)
	{
	device_->SetTexture(stage,d3dTexture_);
	}


//*** HasAlphaChannel ***

bool Platform_Win32_3D_D3D9_Texture::HasAlphaChannel()
	{
	if (format_==D3DFMT_A8R8G8B8)
		{
		return true;
		}

	if (format_==D3DFMT_DXT5)
		{
		return true;
		}

	return false;
	}


//*** GetWidth ***

int Platform_Win32_3D_D3D9_Texture::GetWidth()
	{
	return width_;
	}


//*** GetHeight ***

int Platform_Win32_3D_D3D9_Texture::GetHeight()
	{
	return height_;
	}


//*** Lock ***

void Platform_Win32_3D_D3D9_Texture::Lock()
	{
	Assert(!lockedData_,"Texture is already locked");
	if (lockedData_)
		{
		return;
		}

	HRESULT hr=device_->CreateOffscreenPlainSurface(width_,height_,D3DFMT_A8R8G8B8,D3DPOOL_DEFAULT,&lockedSurface_,0);
	Assert(SUCCEEDED(hr),"Failed to create plain surface");
	IDirect3DSurface9* thisSurface=0;
	hr=d3dTexture_->GetSurfaceLevel(0,&thisSurface);
	Assert(SUCCEEDED(hr),"Failed to get surface");
//	hr=D3DXLoadSurfaceFromSurface(lockedSurface_,0,0,thisSurface,0,0,D3DX_FILTER_NONE,0);
	Assert(SUCCEEDED(hr),"Failed to copy");
	thisSurface->Release();

	D3DLOCKED_RECT lockedRect;

	lockedSurface_->LockRect(&lockedRect,0,D3DLOCK_READONLY);
	lockedData_=static_cast<unsigned char*>(lockedRect.pBits);
	lockedPitch_=lockedRect.Pitch;

	}


//*** Unlock ***

void Platform_Win32_3D_D3D9_Texture::Unlock()
	{
	Assert(lockedData_,"Texture is not locked");
	if (!lockedData_)
		{
		return;
		}

	d3dTexture_->UnlockRect(0);
	lockedData_=0;
	lockedPitch_=0;
	lockedSurface_->Release();
	lockedSurface_=0;
	}


//*** GetTexelData ***

unsigned char* Platform_Win32_3D_D3D9_Texture::GetTexelData()
	{
	Assert(lockedData_,"Texture is not locked");
	if (!lockedData_)
		{
		return 0;
		}

	return lockedData_;
	}


//*** GetPitch ***

int Platform_Win32_3D_D3D9_Texture::GetPitch()
	{
	Assert(lockedData_,"Texture is not locked");
	if (!lockedData_)
		{
		return 0;
		}

	return lockedPitch_;
	}
