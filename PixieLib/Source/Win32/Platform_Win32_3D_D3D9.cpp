//*** Platform_Win32_3D_D3D9.cpp ***

#include "Platform_Win32_3D_D3D9.h"
#include "Debug.h"
#include "StandardLibrary.h"

#define WIN32_LEAN_AND_MEAN 
#define VC_EXTRALEAN
#include <d3d9.h>

#include "Platform_Win32_3D_D3D9_VertexBuffer.h"
#include "Platform_Win32_3D_D3D9_IndexBuffer.h"
#include "Platform_Win32_3D_D3D9_Texture.h"

//*** Constructor ***

Platform_Win32_3D_D3D9::Platform_Win32_3D_D3D9(struct HWND__* windowHandle, bool fullscreen, int screenWidth, int screenHeight):
	windowHandle_(windowHandle),
	fullscreen_(fullscreen),
	screenWidth_(screenWidth),
	screenHeight_(screenHeight),
	d3dDLL_(0),
	direct3D_(0),
	device_(0)
	{
	DebugPrint(("Creating Platform_Win32_3D_D3D9...\n\tfullscreen:%d\n\tscreenWidth:%d\n\tscreenHeight:%d\n",fullscreen,screenWidth,screenHeight));
	}


//*** Setup ***

bool Platform_Win32_3D_D3D9::Setup()
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

	// Set antialias quality
	D3DMULTISAMPLE_TYPE multiSampleType=D3DMULTISAMPLE_NONE;
	DWORD multiSampleQuality=0;

	// Check for antialiasing
/*	if( SUCCEEDED(direct3D_->CheckDeviceMultiSampleType( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL , D3DFMT_A8R8G8B8, !fullscreen_, D3DMULTISAMPLE_2_SAMPLES, &multiSampleQuality) ) )
		{
		multiSampleType=D3DMULTISAMPLE_2_SAMPLES;
		}
	else
		{
		multiSampleQuality=0;
		}
*/
	// Get present parameters
	D3DPRESENT_PARAMETERS d3dpp;
	if (fullscreen_)
		{
//		SetWindowPos(windowHandle_,HWND_TOPMOST,0,0,0,0,SWP_NOSIZE | SWP_NOMOVE | SWP_NOREDRAW | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOSENDCHANGING);
//		SetWindowLong(windowHandle_,GWL_STYLE,WS_POPUPWINDOW|WS_VISIBLE);
		ZeroMemory( &d3dpp, sizeof(d3dpp) );

		d3dpp.BackBufferWidth=screenWidth_;
		d3dpp.BackBufferHeight=screenHeight_;
		d3dpp.BackBufferFormat=D3DFMT_A8R8G8B8;
		d3dpp.BackBufferCount=2;
		d3dpp.MultiSampleType=multiSampleType;
		d3dpp.MultiSampleQuality=multiSampleQuality;
		d3dpp.SwapEffect=D3DSWAPEFFECT_FLIP;
		d3dpp.hDeviceWindow=windowHandle_;
		d3dpp.Windowed=FALSE;
		d3dpp.EnableAutoDepthStencil=TRUE;
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
		d3dpp.BackBufferFormat=D3DFMT_A8R8G8B8;
		d3dpp.EnableAutoDepthStencil = TRUE;
		d3dpp.BackBufferCount=1;
		d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
		d3dpp.MultiSampleType=multiSampleType;
		d3dpp.MultiSampleQuality=multiSampleQuality;
		d3dpp.FullScreen_RefreshRateInHz=D3DPRESENT_RATE_DEFAULT;
		d3dpp.PresentationInterval=D3DPRESENT_INTERVAL_ONE;
		}

	// Set device adapter to use
	UINT AdapterToUse=D3DADAPTER_DEFAULT;
	D3DDEVTYPE DeviceType=D3DDEVTYPE_HAL;
		
	// Look for 'NVIDIA NVPerfHUD' adapter
	// If it is present, override default settings
	for (UINT Adapter=0;Adapter<direct3D_->GetAdapterCount();Adapter++)
		{
		D3DADAPTER_IDENTIFIER9 Identifier;
		HRESULT res=direct3D_->GetAdapterIdentifier(Adapter,0,&Identifier);
		if (res==S_OK && strcmp(Identifier.Description,"NVIDIA NVPerfHUD")==0)
			{
			AdapterToUse=Adapter;
			DeviceType=D3DDEVTYPE_REF;
			break;
			}
		}

	// Create the D3DDevice
	HRESULT ret=direct3D_->CreateDevice(AdapterToUse,DeviceType,windowHandle_,D3DCREATE_PUREDEVICE | D3DCREATE_HARDWARE_VERTEXPROCESSING,&d3dpp,&device_);
	if (FAILED(ret) || !device_)
		{
		DebugPrint(("Couldn't create Direct3D Device\n"));
		return false;
		}


	// Turn on antialiasing
	if (multiSampleType!=D3DMULTISAMPLE_NONE)
		{
		device_->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS,TRUE);
		}

	device_->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);
//	device_->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);
	device_->SetRenderState(D3DRS_LIGHTING,TRUE);
//	device_->SetRenderState(D3DRS_NORMALIZENORMALS,TRUE);
//	device_->SetRenderState(D3DRS_SPECULARENABLE,TRUE);
//	device_->SetRenderState(D3DRS_LOCALVIEWER,TRUE);
	device_->SetRenderState(D3DRS_COLORVERTEX,TRUE);

	device_->SetRenderState(D3DRS_ALPHATESTENABLE,TRUE);
	device_->SetRenderState(D3DRS_ALPHAFUNC,D3DCMP_GREATEREQUAL);
	device_->SetRenderState(D3DRS_ALPHAREF,1);
	device_->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
	device_->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	device_->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);

	device_->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE,D3DMCS_MATERIAL);
	device_->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE,D3DMCS_MATERIAL);
	device_->SetRenderState(D3DRS_SPECULARMATERIALSOURCE,D3DMCS_MATERIAL);
	device_->SetRenderState(D3DRS_EMISSIVEMATERIALSOURCE,D3DMCS_MATERIAL);

	device_->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE,D3DMCS_COLOR1);
	device_->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE,D3DMCS_COLOR1);
	device_->SetRenderState(D3DRS_SPECULARMATERIALSOURCE,D3DMCS_COLOR1);
//	device_->SetRenderState(D3DRS_EMISSIVEMATERIALSOURCE,D3DMCS_MATERIAL);

	device_->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_MODULATE);
	device_->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE);
	device_->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_DIFFUSE);

//	device_->SetTextureStageState(1,D3DTSS_COLOROP,D3DTOP_SUBTRACT);
//	device_->SetTextureStageState(1,D3DTSS_COLORARG1,D3DTA_CURRENT);
//	device_->SetTextureStageState(1,D3DTSS_COLORARG2,D3DTA_TEXTURE);

	device_->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_MODULATE);
	device_->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);
	device_->SetTextureStageState(0,D3DTSS_ALPHAARG2,D3DTA_DIFFUSE);

	device_->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
	device_->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
	device_->SetSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_LINEAR);

	device_->SetSamplerState(1,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
	device_->SetSamplerState(1,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
	device_->SetSamplerState(1,D3DSAMP_MIPFILTER,D3DTEXF_LINEAR);

	// Enable fog
/*
	unsigned int color=0;
    float Start   = 0.0f;    // For linear mode
    float End     = 17.0f;
	device_->SetRenderState(D3DRS_FOGENABLE,TRUE);
	device_->SetRenderState(D3DRS_FOGVERTEXMODE,D3DFOG_LINEAR);
    device_->SetRenderState(D3DRS_FOGCOLOR, color);
    device_->SetRenderState(D3DRS_FOGSTART, *static_cast<DWORD*>(&Start));
    device_->SetRenderState(D3DRS_FOGEND,   *static_cast<DWORD*>(&End));
*/

	// Setup default material
	D3DMATERIAL9 mtrl;
	ZeroMemory( &mtrl, sizeof(mtrl) );
	mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
	mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
	mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
	mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
	mtrl.Power=4;
	mtrl.Specular.r=1.0f;
	mtrl.Specular.g=1.0f;
	mtrl.Specular.b=1.0f;
	mtrl.Specular.a=1.0f;
	device_->SetMaterial( &mtrl );
	
	
	lightCount_=8;

/*
	D3DLIGHT9 light;
	memset(&light,0,sizeof(light));

	light.Type = D3DLIGHT_DIRECTIONAL;

	D3DCOLORVALUE d3dcolor;
	d3dcolor.r=1;
	d3dcolor.g=1;
	d3dcolor.b=1;
	d3dcolor.a=1;

	light.Diffuse=d3dcolor;
	light.Specular=d3dcolor;

	light.Direction.x=0;
	light.Direction.y=0;
	light.Direction.z=-1;

	device_->SetLight(0,&light);

	device_->LightEnable(0,TRUE);
*/


	DebugPrint(("...Platform_Win32_3D_D3D9 created\n"));
	return true;
	}


//*** Destructor ***

Platform_Win32_3D_D3D9::~Platform_Win32_3D_D3D9()
	{
	DebugPrint(("Destroying Platform_Win32_3D_D3D9...\n"));

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
//		SetWindowLong(windowHandle_,GWL_STYLE,WS_OVERLAPPEDWINDOW | WS_VISIBLE );
		}

//	SetWindowPos(windowHandle_,HWND_NOTOPMOST,0,0,0,0,SWP_NOSIZE | SWP_NOMOVE | SWP_NOREDRAW | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOSENDCHANGING);


	DebugPrint(("... Platform_Win32_3D_D3D9 destroyed\n"));
	}


//*** IsCompressedTextureFormatOk ***
/*
bool Platform_Win32_3D_D3D9::IsCompressedTextureFormatOk( D3DFORMAT TextureFormat,  D3DFORMAT AdapterFormat ) 
	{
    HRESULT hr = device_->CheckDeviceFormat( D3DADAPTER_DEFAULT,
                                          D3DDEVTYPE_HAL,
                                          AdapterFormat,
                                          0,
                                          D3DRTYPE_TEXTURE,
                                          TextureFormat);

    return SUCCEEDED( hr );
	}
*/


//*** BeginScene ***

void Platform_Win32_3D_D3D9::BeginScene(unsigned int color, float z, unsigned int stencil)
	{
	device_->Clear( 0, 0, D3DCLEAR_TARGET|D3DCLEAR_STENCIL|D3DCLEAR_ZBUFFER,color, z, stencil );
	device_->BeginScene();
	}


 //*** EndScene ***

void Platform_Win32_3D_D3D9::EndScene()
	{
	device_->EndScene();
	device_->Present(0, 0, 0, 0 );
	}


 //*** CreateVertexBuffer ***

Platform_3D_VertexBuffer* Platform_Win32_3D_D3D9::CreateVertexBuffer(int vertexFormat, int vertexCount, bool dynamic)
	{
	return new Platform_Win32_3D_D3D9_VertexBuffer(device_, vertexFormat, vertexCount, dynamic);
	}


//*** CreateIndexBuffer ***

Platform_3D_IndexBuffer* Platform_Win32_3D_D3D9::CreateIndexBuffer(int indexCount, bool dynamic)
	{
	return new Platform_Win32_3D_D3D9_IndexBuffer(device_, indexCount, dynamic);
	}


//*** CreateTexture ***

Platform_3D_Texture* Platform_Win32_3D_D3D9::CreateTexture(void* data, int size)
	{
	return new Platform_Win32_3D_D3D9_Texture(device_, data, size);
	}


//*** CreateTexture ***

Platform_3D_Texture* Platform_Win32_3D_D3D9::CreateTexture(int width, int height, void* data)
	{
	return new Platform_Win32_3D_D3D9_Texture(device_, width, height, data);
	}


//*** Render ***

void Platform_Win32_3D_D3D9::Render(int startVertex, int vertexCount, int startIndex, int indexCount)
	{
	device_->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,startVertex,0,vertexCount,startIndex,indexCount/3);
	}


//*** Render ***

void Platform_Win32_3D_D3D9::Render(int startVertex, int vertexCount)
	{
	device_->DrawPrimitive(D3DPT_TRIANGLELIST,startVertex,vertexCount/3);
	}


//*** RenderStrip ***

void Platform_Win32_3D_D3D9::RenderStrip(int startVertex, int vertexCount, int startIndex, int indexCount)
	{
	device_->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,startVertex,0,vertexCount,startIndex,indexCount-2);
	}


//*** RenderStrip ***

void Platform_Win32_3D_D3D9::RenderStrip(int startVertex, int vertexCount)
	{
	device_->DrawPrimitive(D3DPT_TRIANGLESTRIP,startVertex, vertexCount-2);
	}


//*** RenderFan ***

void Platform_Win32_3D_D3D9::RenderFan(int startVertex, int vertexCount, int startIndex, int indexCount)
	{
	device_->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN,startVertex,0,vertexCount,startIndex,indexCount/3);
	}


//*** RenderFan ***

void Platform_Win32_3D_D3D9::RenderFan(int startVertex, int vertexCount)
	{
	device_->DrawPrimitive(D3DPT_TRIANGLEFAN,startVertex,vertexCount/3);
	}


//*** SetWorldMatrix ***

void Platform_Win32_3D_D3D9::SetWorldMatrix(const float worldMatrix[16])
	{
	device_->SetTransform(D3DTS_WORLD,reinterpret_cast<const D3DMATRIX*>(worldMatrix));	
	}


//*** SetViewMatrix ***

void Platform_Win32_3D_D3D9::SetViewMatrix(const float viewMatrix[16])
	{
	device_->SetTransform(D3DTS_VIEW,reinterpret_cast<const D3DMATRIX*>(viewMatrix));	
	}


//*** SetProjectionMatrix ***

void Platform_Win32_3D_D3D9::SetProjectionMatrix(const float projectionMatrix[16])
	{
	device_->SetTransform(D3DTS_PROJECTION,reinterpret_cast<const D3DMATRIX*>(projectionMatrix));	
	}


//*** GetLightCount ***

int Platform_Win32_3D_D3D9::GetLightCount()
	{
	return lightCount_;
	}


//*** EnableDirectionalLight ***

void Platform_Win32_3D_D3D9::EnableDirectionalLight(int lightIndex, float colorR, float colorG, float colorB, float directionX, float directionY, float directionZ)
	{
	Assert(lightIndex>=0 && lightIndex<lightCount_,"Light index out of range");
	if (lightIndex<0 || lightIndex>=lightCount_)
		{
		return;
		}

	D3DLIGHT9 light;
	memset(&light,0,sizeof(light));

	light.Type = D3DLIGHT_DIRECTIONAL;

	D3DCOLORVALUE d3dcolor;
	d3dcolor.r=colorR;
	d3dcolor.g=colorG;
	d3dcolor.b=colorB;
	d3dcolor.a=1;

	light.Diffuse=d3dcolor;
	light.Specular=d3dcolor;

	light.Direction.x=directionX;
	light.Direction.y=directionY;
	light.Direction.z=directionZ;

	device_->SetLight(lightIndex,&light);

	device_->LightEnable(lightIndex,TRUE);
	}


//*** DisableLight ***

void Platform_Win32_3D_D3D9::DisableLight(int lightIndex)
	{
	Assert(lightIndex>=0 && lightIndex<lightCount_,"Light index out of range");
	if (lightIndex<0 || lightIndex>=lightCount_)
		{
		return;
		}

	device_->LightEnable(lightIndex,FALSE);
	}


//*** SetAmbientLight ***

void Platform_Win32_3D_D3D9::SetAmbientLight(unsigned int color)
	{
	device_->SetRenderState(D3DRS_AMBIENT,color);
	}


//*** EnableLighting ***

void Platform_Win32_3D_D3D9::EnableLighting(bool enabled)
	{
	if (enabled)
		{
		device_->SetRenderState(D3DRS_LIGHTING,TRUE);
		}
	else
		{
		device_->SetRenderState(D3DRS_LIGHTING,FALSE);
		}
	}


//*** EnableZRead ***

void Platform_Win32_3D_D3D9::EnableZRead(bool enabled)
	{
	if (enabled)
		{
		device_->SetRenderState(D3DRS_ZENABLE,TRUE);
		}
	else
		{
		device_->SetRenderState(D3DRS_ZENABLE,FALSE);
		}
	}


//*** EnableZWrite ***

void Platform_Win32_3D_D3D9::EnableZWrite(bool enabled)
	{
	if (enabled)
		{
		device_->SetRenderState(D3DRS_ZWRITEENABLE,TRUE);
		}
	else
		{
		device_->SetRenderState(D3DRS_ZWRITEENABLE,FALSE);
		}
	}
		