//*** Platform_Win32_Screen_D3D9.cpp ***

#include "Platform_Win32_Screen_D3D9.h"

#define WIN32_LEAN_AND_MEAN 
#define VC_EXTRALEAN
#include <d3d9.h>

#include "Platform_OS.h"
#define DebugPrint(x) Platform::GetPlatform_OS()->OutputDebugText x;

//*** Constructor ***

Platform_Win32_Screen_D3D9::Platform_Win32_Screen_D3D9(struct HWND__* windowHandle, bool fullscreen, int screenWidth, int screenHeight, bool interpolationMode):
	windowHandle_(windowHandle),
	interpolationMode_(interpolationMode),
	fullscreen_(fullscreen),
	screenWidth_(screenWidth),
	screenHeight_(screenHeight),
	d3dDLL_(0),
	direct3D_(0),
	device_(0),
	backbuffer16a_(0),
	backbuffer16b_(0),
	backbuffer32a_(0),
	backbuffer32b_(0),
	backbufferWidth_(0),
	backbufferHeight_(0),
	usingDynamicTexture_(false),
	buffer1Filled_(false),
	buffer2Filled_(false),
	lastPresentWidth_(0),
	lastPresentHeight_(0)
	{
	DebugPrint(("Creating Platform_Win32_Screen_D3D9...\n\tfullscreen:%d\n\tscreenWidth:%d\n\tscreenHeight:%d\n\tinterpolationMode:%d\n",fullscreen,screenWidth,screenHeight,interpolationMode));
	}


//*** Setup ***

bool Platform_Win32_Screen_D3D9::Setup()
	{

	// Create D3D object
	d3dDLL_=LoadLibrary("d3d9.dll");
	if (!d3dDLL_)
		{
		DebugPrint(("Couldn't find d3d9.dll\n"));
		return false;
		}

	// Create DirectDraw object
	typedef IDirect3D9 * (WINAPI *Direct3DCreate9Definition)( unsigned int SDKVersion );
	Direct3DCreate9Definition Direct3DCreate9=(Direct3DCreate9Definition)GetProcAddress((HMODULE)d3dDLL_, "Direct3DCreate9"); 
	if (Direct3DCreate9==0)
		{
		DebugPrint(("Failed to find function entry point in d3d9.dll\n"));
		return false;
		}

	direct3D_=Direct3DCreate9(D3D_SDK_VERSION);
	if (!direct3D_)
		{
		DebugPrint(("Couldn't create Direct3D object\n"));
		return false;
		}


	// Get present parameters
	D3DPRESENT_PARAMETERS d3dpp;
	if (fullscreen_)
		{
		SetWindowPos(windowHandle_,HWND_TOPMOST,0,0,0,0,SWP_NOSIZE | SWP_NOMOVE | SWP_NOREDRAW | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOSENDCHANGING);
		SetWindowLong(windowHandle_,GWL_STYLE,WS_POPUPWINDOW|WS_VISIBLE);
		ZeroMemory( &d3dpp, sizeof(d3dpp) );

		d3dpp.BackBufferWidth=screenWidth_;
		d3dpp.BackBufferHeight=screenHeight_;
		d3dpp.BackBufferFormat=D3DFMT_X8R8G8B8;
		d3dpp.BackBufferCount=2;
		d3dpp.MultiSampleType=D3DMULTISAMPLE_NONE;
		d3dpp.MultiSampleQuality=0;
		d3dpp.SwapEffect=D3DSWAPEFFECT_FLIP;
		d3dpp.hDeviceWindow=windowHandle_;
		d3dpp.Windowed=0;
		d3dpp.EnableAutoDepthStencil=FALSE;
		d3dpp.AutoDepthStencilFormat=D3DFMT_D24S8;
		d3dpp.Flags=0;
		d3dpp.FullScreen_RefreshRateInHz=D3DPRESENT_RATE_DEFAULT;
		d3dpp.PresentationInterval=D3DPRESENT_INTERVAL_ONE;
		}
	else
		{
		ZeroMemory( &d3dpp, sizeof(d3dpp) );
		d3dpp.Windowed = TRUE;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.BackBufferFormat=D3DFMT_UNKNOWN;
		d3dpp.EnableAutoDepthStencil = FALSE;
		d3dpp.BackBufferCount=1;
		d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
		d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
		d3dpp.FullScreen_RefreshRateInHz=D3DPRESENT_RATE_DEFAULT;
		d3dpp.PresentationInterval=D3DPRESENT_INTERVAL_ONE;
		}

	// Set device adapter to use
	UINT AdapterToUse=D3DADAPTER_DEFAULT;
	D3DDEVTYPE DeviceType=D3DDEVTYPE_HAL;
	
	// Create the D3DDevice
	HRESULT ret=direct3D_->CreateDevice(AdapterToUse,DeviceType,windowHandle_, D3DCREATE_SOFTWARE_VERTEXPROCESSING,&d3dpp,&device_);
	if (FAILED(ret) || !device_)
		{
		DebugPrint(("Couldn't create Direct3D Device\n"));
		return false;
		}


	// Use the right type of filtering for the requested interpolation mode
	device_->SetSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_NONE );
	if (interpolationMode_)
		{
		device_->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR );
		device_->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR );
		}
	else
		{
		device_->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_POINT);
		device_->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_POINT);
		}

	device_->SetSamplerState(0,D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	device_->SetSamplerState(0,D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

	DebugPrint(("...Platform_Win32_Screen_D3D9 created\n"));
	return true;
	}


//*** Destructor ***

Platform_Win32_Screen_D3D9::~Platform_Win32_Screen_D3D9()
	{
	DebugPrint(("Destroying Platform_Win32_Screen_D3D9...\n"));

	if (backbuffer16a_)
		{
		backbuffer16a_->Release();
		}

	if (backbuffer16b_)
		{
		backbuffer16b_->Release();
		}

	if (backbuffer32a_)
		{
		backbuffer32a_->Release();
		}

	if (backbuffer32b_)
		{
		backbuffer32b_->Release();
		}
	if (device_)
		{
		device_->Release();
		}

	if (direct3D_)
		{
		direct3D_->Release();
		}

	if (d3dDLL_)
		{
		FreeLibrary((HMODULE)d3dDLL_);
		}

	if (fullscreen_)
		{
		SetWindowLong(windowHandle_,GWL_STYLE,WS_OVERLAPPEDWINDOW | WS_VISIBLE );
		}

	SetWindowPos(windowHandle_,HWND_NOTOPMOST,0,0,0,0,SWP_NOSIZE | SWP_NOMOVE | SWP_NOREDRAW | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOSENDCHANGING);


	DebugPrint(("... Platform_Win32_Screen_D3D9 destroyed\n"));
	}


//*** Present ***

bool Platform_Win32_Screen_D3D9::Present(unsigned short* bitmapData, int bitmapWidth, int bitmapHeight, unsigned short modulate, unsigned short backgroundColor)
	{
	lastPresentWidth_=bitmapWidth;
	lastPresentHeight_=bitmapHeight;
	unsigned int color=RGB16TO32(backgroundColor);
		
	
	HRESULT result=S_OK;

	result=device_->Clear( 0, 0, D3DCLEAR_TARGET, color, 0, 0);
	if (FAILED(result))
		{
		DebugPrint(("Clear failed\n"));
		return false;
		}

	result=device_->BeginScene();
	if (FAILED(result))
		{
		DebugPrint(("BeginScene failed\n"));
		return false;
		}
	

	if (!CopyBitmapToBackBuffer(bitmapData,bitmapWidth,bitmapHeight,backgroundColor))
		{
		DebugPrint(("CopyBitmapToBackBuffer failed\n"));
		return false;
		}

	if (buffer2Filled_)
		{
		if (!CopyBackBufferToFrontBuffer(bitmapWidth, bitmapHeight, modulate))
			{		
			DebugPrint(("CopyBackBufferToFrontBuffer failed\n"));
			return false;
			}
		}
	
	result=device_->EndScene();
	if (FAILED(result))
		{
		DebugPrint(("EndScene failed\n"));
		return false;
		}	
	
	result=device_->Present(0, 0, 0, 0 );
	if (FAILED(result))
		{
		DebugPrint(("Present failed\n"));
		return false;
		}

	// Swap buffers
	IDirect3DTexture9* temp=backbuffer16b_;
	backbuffer16b_=backbuffer16a_;
	backbuffer16a_=temp;
	if (!buffer1Filled_)
		{
		buffer1Filled_=true;
		}
	else if (!buffer2Filled_)
		{
		buffer2Filled_=true;
		}


	return true;
	}


//*** Present ***

bool Platform_Win32_Screen_D3D9::Present(unsigned int* bitmapData, int bitmapWidth, int bitmapHeight, unsigned int modulate, unsigned int backgroundColor)
	{
	lastPresentWidth_=bitmapWidth;
	lastPresentHeight_=bitmapHeight;
	unsigned int color=backgroundColor;
		
	
	HRESULT result=S_OK;

	result=device_->Clear( 0, 0, D3DCLEAR_TARGET, color, 0, 0);
	if (FAILED(result))
		{
		DebugPrint(("Clear failed\n"));
		return false;
		}

	result=device_->BeginScene();
	if (FAILED(result))
		{
		DebugPrint(("BeginScene failed\n"));
		return false;
		}
	

	if (!CopyBitmapToBackBuffer(bitmapData,bitmapWidth,bitmapHeight,backgroundColor))
		{
		DebugPrint(("CopyBitmapToBackBuffer failed\n"));
		return false;
		}

	if (buffer2Filled_)
		{
		if (!CopyBackBufferToFrontBuffer(bitmapWidth, bitmapHeight, modulate))
			{		
			DebugPrint(("CopyBackBufferToFrontBuffer failed\n"));
			return false;
			}
		}
	
	result=device_->EndScene();
	if (FAILED(result))
		{
		DebugPrint(("EndScene failed\n"));
		return false;
		}	
	
	result=device_->Present(0, 0, 0, 0 );
	if (FAILED(result))
		{
		DebugPrint(("Present failed\n"));
		return false;
		}

	// Swap buffers
	IDirect3DTexture9* temp=backbuffer32b_;
	backbuffer32b_=backbuffer32a_;
	backbuffer32a_=temp;
	if (!buffer1Filled_)
		{
		buffer1Filled_=true;
		}
	else if (!buffer2Filled_)
		{
		buffer2Filled_=true;
		}


	return true;
	}


//*** CopyBitmapToBackBuffer ***

bool Platform_Win32_Screen_D3D9::CopyBitmapToBackBuffer(unsigned short* bitmapData, int bitmapWidth, int bitmapHeight, unsigned short backgroundColor)
	{
	if (!backbuffer16a_ || backbufferWidth_<bitmapWidth || backbufferHeight_<bitmapHeight)
		{
		if (!CreateBackBuffers(bitmapWidth,bitmapHeight))
			{
			DebugPrint(("Couldn't create backbuffers\n"));
			return false;
			}
		}

	D3DLOCKED_RECT lockedRect;
	HRESULT result=S_OK;
	if (usingDynamicTexture_)
		{
		result=backbuffer16a_->LockRect(0,&lockedRect,0,D3DLOCK_DISCARD | D3DLOCK_NOOVERWRITE );
		}
	else
		{
		RECT rect;
		rect.top=0; 
		rect.left=0;
		rect.right=bitmapWidth;
		rect.bottom=bitmapHeight;
		result=backbuffer16a_->LockRect(0,&lockedRect,&rect,D3DLOCK_DISCARD | D3DLOCK_NOOVERWRITE );
		}

	if (FAILED(result))
		{
		DebugPrint(("Failed to lock backbuffer\n"));
		return false;
		}

	unsigned char* destination=static_cast<unsigned char*>(lockedRect.pBits);
	int pitch=lockedRect.Pitch;
	unsigned char* source=reinterpret_cast<unsigned char*>(bitmapData);
	int bitmapPitch=bitmapWidth*2;
	int dwords=bitmapWidth/2;


	if (pitch==bitmapPitch)
		{
		memcpy(destination,source,pitch*bitmapHeight);
		}
	else
		{
		if (interpolationMode_)
			{
			for (int y=0; y<bitmapHeight; y++)
				{
				memcpy(destination,source,bitmapPitch);
				destination[bitmapPitch]=(unsigned char)(backgroundColor & 0xff);
				destination[bitmapPitch+1]=(unsigned char)(backgroundColor >> 8);
				destination+=pitch;
				source+=bitmapPitch;
				}
			}
		else
			{
			for (int y=0; y<bitmapHeight; y++)
				{
				memcpy(destination,source,bitmapPitch);
				destination+=pitch;
				source+=bitmapPitch;
				}
			}
		}

/*	if (interpolationMode_)
		{
		unsigned short* dest=static_cast<unsigned short*>(destination);
		int width=bitmap->GetWidth();
		for (int x=0; x<width; x++)
			{
			*dest=backgroundColor;
			dest++;
			}
		dest+=pitch;
		for (int x=0; x<width; x++)
			{
			*dest=backgroundColor;
			dest++;
			}
		}
*/


	result=backbuffer16a_->UnlockRect(0);
	if (FAILED(result))
		{
		DebugPrint(("Failed to unlock backbuffer\n"));
		return false;
		}

	return true;
	}


//*** CopyBackBufferToFrontBuffer ***

bool Platform_Win32_Screen_D3D9::CopyBackBufferToFrontBuffer(int bitmapWidth, int bitmapHeight, unsigned short modulate)
	{
	struct Vertex
		{
		float x;
		float y;
		float z;
		float rhw;
		unsigned int color;
		float u;
		float v;
		};

	Vertex quad[4];

	float x1=0;
	float y1=0;
	float x2=0;
	float y2=0;

	unsigned int color=RGB16TO32(modulate);
	if (interpolationMode_)
		{
		float hscale=screenWidth_/(float)bitmapWidth;
		float vscale=screenHeight_/(float)bitmapHeight;
		float pixelScale=min(hscale,vscale);

		float hborder=(screenWidth_-pixelScale*bitmapWidth)/2;
		float vborder=(screenHeight_-pixelScale*bitmapHeight)/2;
		x1=hborder;
		y1=vborder;
		x2=x1+pixelScale*bitmapWidth;
		y2=y1+pixelScale*bitmapHeight;
		}
	else
		{
		int hscale=screenWidth_/bitmapWidth;
		int vscale=screenHeight_/bitmapHeight;
		int pixelScale=min(hscale,vscale);

		int hborder=(screenWidth_-pixelScale*bitmapWidth)/2;
		int vborder=(screenHeight_-pixelScale*bitmapHeight)/2;
		x1=(float)hborder;
		y1=(float)vborder;
		x2=x1+(float)(pixelScale*bitmapWidth);
		y2=y1+(float)(pixelScale*bitmapHeight);
		}
	

	quad[0].x=x2-0.5f;
	quad[0].y=y2-0.5f;
	quad[0].z=0;
	quad[0].rhw=0.5f;
	quad[0].color=color;
	quad[0].u=bitmapWidth/(float)backbufferWidth_;
	quad[0].v=bitmapHeight/(float)backbufferHeight_;	

	quad[1].x=x1-0.5f;
	quad[1].y=y2-0.5f;
	quad[1].z=0;
	quad[1].rhw=0.5f;
	quad[1].color=color;
	quad[1].u=0;
	quad[1].v=bitmapHeight/(float)backbufferHeight_;	

	quad[2].x=x2-0.5f;
	quad[2].y=y1-0.5f;
	quad[2].z=0;
	quad[2].rhw=0.5f;
	quad[2].color=color;
	quad[2].u=bitmapWidth/(float)backbufferWidth_;
	quad[2].v=0;	

	quad[3].x=x1-0.5f;
	quad[3].y=y1-0.5f;
	quad[3].z=0;
	quad[3].rhw=0.5f;
	quad[3].color=color;
	quad[3].u=0;
	quad[3].v=0;	

	HRESULT result=S_OK;

	
	result=device_->SetTexture(0,backbuffer16b_);
	if (FAILED(result))
		{
		DebugPrint(("SetTexture failed\n"));
		return false;
		}
	
	result=device_->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
	if (FAILED(result))
		{
		DebugPrint(("SetFVF failed\n"));
		return false;
		}	


	result=device_->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
	if (FAILED(result))
		{
		DebugPrint(("SetFVF failed\n"));
		return false;
		}	

	result=device_->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,quad,sizeof(Vertex));
	if (FAILED(result))
		{
		DebugPrint(("DrawPrimitiveUP\n"));
		return false;
		}


	return true;
	}


//*** CopyBitmapToBackBuffer ***

bool Platform_Win32_Screen_D3D9::CopyBitmapToBackBuffer(unsigned int* bitmapData, int bitmapWidth, int bitmapHeight, unsigned int backgroundColor)
	{
	if (!backbuffer32a_ || backbufferWidth_<bitmapWidth || backbufferHeight_<bitmapHeight)
		{
		if (!CreateBackBuffers(bitmapWidth,bitmapHeight))
			{
			DebugPrint(("Couldn't create backbuffers\n"));
			return false;
			}
		}

	D3DLOCKED_RECT lockedRect;
	HRESULT result=S_OK;
	if (usingDynamicTexture_)
		{
		result=backbuffer32a_->LockRect(0,&lockedRect,0,D3DLOCK_DISCARD | D3DLOCK_NOOVERWRITE );
		}
	else
		{
		RECT rect;
		rect.top=0; 
		rect.left=0;
		rect.right=bitmapWidth;
		rect.bottom=bitmapHeight;
		result=backbuffer32a_->LockRect(0,&lockedRect,&rect,D3DLOCK_DISCARD | D3DLOCK_NOOVERWRITE );
		}

	if (FAILED(result))
		{
		DebugPrint(("Failed to lock backbuffer\n"));
		return false;
		}

	unsigned char* destination=static_cast<unsigned char*>(lockedRect.pBits);
	int pitch=lockedRect.Pitch;
	unsigned char* source=reinterpret_cast<unsigned char*>(bitmapData);
	int bitmapPitch=bitmapWidth*4;
	int dwords=bitmapWidth;


	if (pitch==bitmapPitch)
		{
		memcpy(destination,source,pitch*bitmapHeight);
		}
	else
		{
		if (interpolationMode_)
			{
			for (int y=0; y<bitmapHeight; y++)
				{
				memcpy(destination,source,bitmapPitch);
//				*((unsigned int*)(destination[bitmapPitch]))=backgroundColor;
				destination+=pitch;
				source+=bitmapPitch;
				}
			}
		else
			{
			for (int y=0; y<bitmapHeight; y++)
				{
				memcpy(destination,source,bitmapPitch);
				destination+=pitch;
				source+=bitmapPitch;
				}
			}
		}

/*	if (interpolationMode_)
		{
		unsigned short* dest=static_cast<unsigned short*>(destination);
		int width=bitmap->GetWidth();
		for (int x=0; x<width; x++)
			{
			*dest=backgroundColor;
			dest++;
			}
		dest+=pitch;
		for (int x=0; x<width; x++)
			{
			*dest=backgroundColor;
			dest++;
			}
		}
*/


	result=backbuffer32a_->UnlockRect(0);
	if (FAILED(result))
		{
		DebugPrint(("Failed to unlock backbuffer\n"));
		return false;
		}

	return true;
	}


//*** CopyBackBufferToFrontBuffer ***

bool Platform_Win32_Screen_D3D9::CopyBackBufferToFrontBuffer(int bitmapWidth, int bitmapHeight, unsigned int modulate)
	{
	struct Vertex
		{
		float x;
		float y;
		float z;
		float rhw;
		unsigned int color;
		float u;
		float v;
		};

	Vertex quad[4];

	float x1=0;
	float y1=0;
	float x2=0;
	float y2=0;

	unsigned int color=modulate;
	if (interpolationMode_)
		{
		float hscale=screenWidth_/(float)bitmapWidth;
		float vscale=screenHeight_/(float)bitmapHeight;
		float pixelScale=min(hscale,vscale);

		float hborder=(screenWidth_-pixelScale*bitmapWidth)/2;
		float vborder=(screenHeight_-pixelScale*bitmapHeight)/2;
		x1=hborder;
		y1=vborder;
		x2=x1+pixelScale*bitmapWidth;
		y2=y1+pixelScale*bitmapHeight;
		}
	else
		{
		int hscale=screenWidth_/bitmapWidth;
		int vscale=screenHeight_/bitmapHeight;
		int pixelScale=min(hscale,vscale);

		int hborder=(screenWidth_-pixelScale*bitmapWidth)/2;
		int vborder=(screenHeight_-pixelScale*bitmapHeight)/2;
		x1=(float)hborder;
		y1=(float)vborder;
		x2=x1+(float)(pixelScale*bitmapWidth);
		y2=y1+(float)(pixelScale*bitmapHeight);
		}
	

	quad[0].x=x2-0.5f;
	quad[0].y=y2-0.5f;
	quad[0].z=0;
	quad[0].rhw=0.5f;
	quad[0].color=color;
	quad[0].u=bitmapWidth/(float)backbufferWidth_;
	quad[0].v=bitmapHeight/(float)backbufferHeight_;	

	quad[1].x=x1-0.5f;
	quad[1].y=y2-0.5f;
	quad[1].z=0;
	quad[1].rhw=0.5f;
	quad[1].color=color;
	quad[1].u=0;
	quad[1].v=bitmapHeight/(float)backbufferHeight_;	

	quad[2].x=x2-0.5f;
	quad[2].y=y1-0.5f;
	quad[2].z=0;
	quad[2].rhw=0.5f;
	quad[2].color=color;
	quad[2].u=bitmapWidth/(float)backbufferWidth_;
	quad[2].v=0;	

	quad[3].x=x1-0.5f;
	quad[3].y=y1-0.5f;
	quad[3].z=0;
	quad[3].rhw=0.5f;
	quad[3].color=color;
	quad[3].u=0;
	quad[3].v=0;	

	HRESULT result=S_OK;

	
	result=device_->SetTexture(0,backbuffer32b_);
	if (FAILED(result))
		{
		DebugPrint(("SetTexture failed\n"));
		return false;
		}
	
	result=device_->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
	if (FAILED(result))
		{
		DebugPrint(("SetFVF failed\n"));
		return false;
		}	


	result=device_->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
	if (FAILED(result))
		{
		DebugPrint(("SetFVF failed\n"));
		return false;
		}	

	result=device_->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,quad,sizeof(Vertex));
	if (FAILED(result))
		{
		DebugPrint(("DrawPrimitiveUP\n"));
		return false;
		}


	return true;
	}

//*** SetInterpolationMode *** 

void Platform_Win32_Screen_D3D9::SetInterpolationMode(bool enabled)
	{
	if (interpolationMode_==enabled)
		{
		return;
		}

	interpolationMode_=enabled;

	// Use the right type of filtering for the requested interpolation mode
	if (interpolationMode_)
		{
		device_->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
		device_->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
		}
	else
		{
		device_->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_POINT);
		device_->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_POINT);
		}
	}


//*** CreateBackBuffers ***

bool Platform_Win32_Screen_D3D9::CreateBackBuffers(int width, int height)
	{
	if (backbuffer16a_)
		{
		backbuffer16a_->Release();
		backbuffer16a_=0;
		}

	if (backbuffer16b_)
		{
		backbuffer16b_->Release();
		backbuffer16b_=0;
		}

	if (backbuffer32a_)
		{
		backbuffer32a_->Release();
		backbuffer32a_=0;
		}

	if (backbuffer32b_)
		{
		backbuffer32b_->Release();
		backbuffer32b_=0;
		}

	// Create back buffer
	backbufferWidth_=8;
	backbufferHeight_=8;
	while (backbufferWidth_<width)
		{
		backbufferWidth_*=2;
		}
	while (backbufferHeight_<height)
		{
		backbufferHeight_*=2;
		}

	D3DCAPS9 caps;
	device_->GetDeviceCaps(&caps);
	if (caps.Caps2 & D3DCAPS2_DYNAMICTEXTURES)
		{
		HRESULT ret=device_->CreateTexture(backbufferWidth_,backbufferHeight_,1,D3DUSAGE_DYNAMIC,D3DFMT_R5G6B5,D3DPOOL_DEFAULT,&backbuffer16a_,0);
		if (FAILED(ret))
			{
			DebugPrint(("Dynamic textures not supported, using static textures\n"));
			ret=device_->CreateTexture(backbufferWidth_,backbufferHeight_,1,0,D3DFMT_R5G6B5,D3DPOOL_MANAGED,&backbuffer16a_,0);
			if (FAILED(ret))
				{
				DebugPrint(("Couldn't create backbuffer1\n"));
				return false;
				}			
			ret=device_->CreateTexture(backbufferWidth_,backbufferHeight_,1,0,D3DFMT_X8R8G8B8,D3DPOOL_MANAGED,&backbuffer32a_,0);
			if (FAILED(ret))
				{
				DebugPrint(("Couldn't create backbuffer1\n"));
				return false;
				}			
			ret=device_->CreateTexture(backbufferWidth_,backbufferHeight_,1,0,D3DFMT_R5G6B5,D3DPOOL_MANAGED,&backbuffer16b_,0);
			if (FAILED(ret))
				{
				DebugPrint(("Couldn't create backbuffer2\n"));
				return false;
				}			
			ret=device_->CreateTexture(backbufferWidth_,backbufferHeight_,1,0,D3DFMT_X8R8G8B8,D3DPOOL_MANAGED,&backbuffer32b_,0);
			if (FAILED(ret))
				{
				DebugPrint(("Couldn't create backbuffer2\n"));
				return false;
				}			
			}
		else
			{
			ret=device_->CreateTexture(backbufferWidth_,backbufferHeight_,1,D3DUSAGE_DYNAMIC,D3DFMT_X8R8G8B8,D3DPOOL_DEFAULT,&backbuffer32a_,0);
			if (FAILED(ret))
				{
				DebugPrint(("Couldn't create backbuffer2\n"));
				return false;
				}			
			ret=device_->CreateTexture(backbufferWidth_,backbufferHeight_,1,D3DUSAGE_DYNAMIC,D3DFMT_R5G6B5,D3DPOOL_DEFAULT,&backbuffer16b_,0);
			if (FAILED(ret))
				{
				DebugPrint(("Couldn't create backbuffer2\n"));
				return false;
				}			
			ret=device_->CreateTexture(backbufferWidth_,backbufferHeight_,1,D3DUSAGE_DYNAMIC,D3DFMT_X8R8G8B8,D3DPOOL_DEFAULT,&backbuffer32b_,0);
			if (FAILED(ret))
				{
				DebugPrint(("Couldn't create backbuffer2\n"));
				return false;
				}			
			usingDynamicTexture_=true;
			}
		}
	
	return true;
	}


//*** TransformCursorCoordinates ***

void Platform_Win32_Screen_D3D9::TransformCursorCoordinates(float& x, float& y)
	{
	if (lastPresentWidth_==0 || lastPresentHeight_==0)
		{
		return;
		}

	if (!interpolationMode_)
		{
		int hscale=screenWidth_/lastPresentWidth_;
		int vscale=screenHeight_/lastPresentHeight_;
		int pixelScale=max(1,min(hscale,vscale));
		int hborder=max(0,(screenWidth_-pixelScale*lastPresentWidth_)/2);
		int vborder=max(0,(screenHeight_-pixelScale*lastPresentHeight_)/2);

		x=(x-hborder)/pixelScale;
		y=(y-vborder)/pixelScale;
		return;
		}

	float hscale=screenWidth_/(float)lastPresentWidth_;
	float vscale=screenHeight_/(float)lastPresentHeight_;
	float pixelScale=min(hscale,vscale);

	float hborder=(screenWidth_-pixelScale*lastPresentWidth_)/2;
	float vborder=(screenHeight_-pixelScale*lastPresentHeight_)/2;

	x=(x-hborder)/pixelScale;
	y=(y-vborder)/pixelScale;
	}
