//*** Platform_Win32_3D.cpp ***

#include "Platform_Win32_3D.h"
#include "Debug.h"
#include "Platform_Win32_3D_Technology.h"
#include "Platform_Win32_3D_D3D9.h"
#include "Platform_Win32_3D_IndexBuffer.h"
#include "Platform_Win32_3D_VertexBuffer.h"
#include "Platform_Win32_3D_Texture.h"
#include "DebugFont.h"
#include "Platform_Win32_OS.h"
#include "Platform_Time.h"
#include "ArrayIterator.h"

#define WIN32_LEAN_AND_MEAN 
#define VC_EXTRALEAN
#include <windows.h>
#include <stdlib.h>

//*** Constructor ***

Platform_Win32_3D::Platform_Win32_3D(Platform_Win32_OS* os):
	windowHandle_(os->GetWindowHandle()),
	technology_(Technology_Undefined),
	technologyInstance_(0),
	fullscreen_(true),
	fullscreenWidth_(0),
	fullscreenHeight_(0),
	showfps_(false),
	disableOnWmSize_(0),
	ignoreNextOnWmSize_(false),
	lastBoundVertexBuffer_(0),
	lastBoundIndexBuffer_(0)
	{
	for (int i=0; i<8; i++)
		{
		lastBoundTextures_[i]=0;
		}

	// Check commandline flags
	bool forceddraw=false;
	bool forcegdi=false;
	if (os->GetCommandLineString())
		{
		char* cmdline=strdup(os->GetCommandLineString());
		char* token=strtok(cmdline," ");
		while (token)
			{
			if (stricmp(token,"-window")==0)
				{
				fullscreen_=false;
				}
			if (stricmp(token,"-showfps")==0)
				{
				showfps_=true;
				}

			token=strtok(0," ");
			}
		free(cmdline);
		}


	// Determine default screen size
	HWND desktopWindow=GetDesktopWindow();
	RECT desktopRect;
	BOOL result=GetWindowRect(desktopWindow,&desktopRect);
	if (result)
		{
		fullscreenWidth_=desktopRect.right-desktopRect.left;
		fullscreenHeight_=desktopRect.bottom-desktopRect.top;

		windowedWidth_=fullscreenWidth_-fullscreenWidth_/6;
		windowedHeight_=fullscreenHeight_-fullscreenHeight_/6; 
		windowedX_=fullscreenWidth_-windowedWidth_;
		windowedY_=fullscreenHeight_-windowedHeight_;
		windowedX_/=2;
		windowedY_/=2;
		}

	disableOnWmSize_++;


	// Initialize window size
	if (!GetFullscreen())
		{
		SetWindowSize();
		}

/*	if (siEngine->GetCommandLineFlag("forceddraw") || IsRunningOnWine())
		{
		SetTechnology(Technology_DDraw);
		}
	else if (siEngine->GetCommandLineFlag("forcegdi"))
		{
		SetTechnology(Technology_GDI);
		}
	else
*/		{
		SetTechnology(Technology_D3D9); 
		}	

	// Show window
	ShowWindow(windowHandle_,SW_SHOW);

	disableOnWmSize_--;
	}


//*** Destructor ***

Platform_Win32_3D::~Platform_Win32_3D()
	{
	disableOnWmSize_++;

	for (int i=0; i<vertexBuffers_.GetItemCount(); i++)
		{
		delete vertexBuffers_.Get(i);
		}

	for (int i=0; i<indexBuffers_.GetItemCount(); i++)
		{
		delete indexBuffers_.Get(i);
		}

	for (int i=0; i<textures_.GetItemCount(); i++)
		{
		delete textures_.Get(i);
		}

	if (technologyInstance_)
		{
		delete technologyInstance_;
		}

	disableOnWmSize_--;
	}


//*** BeginScene ***

void Platform_Win32_3D::BeginScene(unsigned int color, float z, unsigned int stencil)
	{
	if (!technologyInstance_)
		{
		return;
		}

	technologyInstance_->BeginScene(color,z,stencil);
	}


 //*** EndScene ***

void Platform_Win32_3D::EndScene()
	 {
	 if (!technologyInstance_)
		 {
		 return;
		 }

	 technologyInstance_->EndScene();
	 }


 //*** CreateVertexBuffer ***

Platform_3D_VertexBuffer* Platform_Win32_3D::CreateVertexBuffer(int vertexFormat, int vertexCount, bool dynamic)
	{
	if (!technologyInstance_)
		{
		return 0;
		}


	Platform_Win32_3D_VertexBuffer* vertexBuffer=new Platform_Win32_3D_VertexBuffer(this, vertexFormat, vertexCount, dynamic);
	vertexBuffers_.Add(vertexBuffer);
	vertexBuffer->Reset(technologyInstance_);
	return vertexBuffer;
	}


//*** CreateIndexBuffer ***

Platform_3D_IndexBuffer* Platform_Win32_3D::CreateIndexBuffer(int indexCount, bool dynamic)
	{
	if (!technologyInstance_)
		{
		return 0;
		}

	Platform_Win32_3D_IndexBuffer* indexBuffer=new Platform_Win32_3D_IndexBuffer(this, indexCount, dynamic);
	indexBuffers_.Add(indexBuffer);
	indexBuffer->Reset(technologyInstance_);
	return indexBuffer;
	}


//*** CreateTexture ***

Platform_3D_Texture* Platform_Win32_3D::CreateTexture(const Asset& asset)
	{
	if (!technologyInstance_)
		{
		return 0;
		}

	Platform_Win32_3D_Texture* texture=new Platform_Win32_3D_Texture(this, asset);
	textures_.Add(texture);
	texture->Reset(technologyInstance_);
	return texture;
	}


//*** CreateTexture ***

Platform_3D_Texture* Platform_Win32_3D::CreateTexture(const Image& image)
	{
	if (!technologyInstance_)
		{
		return 0;
		}

	Platform_Win32_3D_Texture* texture=new Platform_Win32_3D_Texture(this, image);
	textures_.Add(texture);
	texture->Reset(technologyInstance_);
	return texture;
	}


//*** Render ***

void Platform_Win32_3D::Render(int startVertex, int vertexCount, int startIndex, int indexCount)
	{
	if (!technologyInstance_)
		{
		return;
		}

	technologyInstance_->Render(startVertex,vertexCount,startIndex,indexCount);
	}


//*** Render ***

void Platform_Win32_3D::Render(int startVertex, int vertexCount)
	{
	if (!technologyInstance_)
		{
		return;
		}

	technologyInstance_->Render(startVertex,vertexCount);
	}


//*** SetWorldMatrix ***

void Platform_Win32_3D::SetWorldMatrix(const float worldMatrix[16])
	{
	if (!technologyInstance_)
		{
		return;
		}

	technologyInstance_->SetWorldMatrix(worldMatrix);
	}


//*** SetViewMatrix ***

void Platform_Win32_3D::SetViewMatrix(const float viewMatrix[16])
	{
	if (!technologyInstance_)
		{
		return;
		}

	technologyInstance_->SetViewMatrix(viewMatrix);
	}


//*** SetProjectionMatrix ***

void Platform_Win32_3D::SetProjectionMatrix(const float projectionMatrix[16])
	{
	if (!technologyInstance_)
		{
		return;
		}

	technologyInstance_->SetProjectionMatrix(projectionMatrix);
	}


//*** GetLightCount ***

int Platform_Win32_3D::GetLightCount()
	{
	if (!technologyInstance_)
		{
		return 0;
		}

	return technologyInstance_->GetLightCount();
	}


//*** EnableDirectionalLight ***

void Platform_Win32_3D::EnableDirectionalLight(int lightIndex, float colorR, float colorG, float colorB, float directionX, float directionY, float directionZ)
	{
	if (!technologyInstance_)
		{
		return;
		}

	technologyInstance_->EnableDirectionalLight(lightIndex, colorR, colorG, colorB, directionX, directionY, directionZ);
	}


//*** DisableLight ***

void Platform_Win32_3D::DisableLight(int lightIndex)
	{
	if (!technologyInstance_)
		{
		return;
		}

	technologyInstance_->DisableLight(lightIndex);
	}


//*** SetAmbientLight ***

void Platform_Win32_3D::SetAmbientLight(unsigned int color)
	{
	if (!technologyInstance_)
		{
		return;
		}

	technologyInstance_->SetAmbientLight(color);
	}

		
//*** EnableLighting ***

void Platform_Win32_3D::EnableLighting(bool enabled)
	{
	if (!technologyInstance_)
		{
		return;
		}

	technologyInstance_->EnableLighting(enabled);
	}


//*** EnableZRead ***

void Platform_Win32_3D::EnableZRead(bool enabled)
	{
	if (!technologyInstance_)
		{
		return;
		}

	technologyInstance_->EnableZRead(enabled);
	}


//*** EnableZWrite ***

void Platform_Win32_3D::EnableZWrite(bool enabled)
	{
	if (!technologyInstance_)
		{
		return;
		}

	technologyInstance_->EnableZWrite(enabled);
	}
		

//*** SetTechnology ***

void Platform_Win32_3D::SetTechnology(Platform_Win32_3D::Technology technology)
	{
	disableOnWmSize_++;

	if (technologyInstance_)
		{
		delete technologyInstance_;
		technologyInstance_=0;
		}

	technology_=technology;

	if (technology_>=Technology_Undefined)
		{
		return;
		}
	
	switch(technology_)
		{
		case Technology_D3D9:
			{
			technologyInstance_=new Platform_Win32_3D_D3D9(windowHandle_, GetFullscreen(), GetWidth(), GetHeight());
			} break;

		}

	if (!technologyInstance_->Setup())
		{
		DowngradeTechnology();
		}
	else
		{
		for (int i=0; i<vertexBuffers_.GetItemCount(); i++)
			{
			vertexBuffers_.Get(i)->Reset(technologyInstance_);
			}

		for (int i=0; i<indexBuffers_.GetItemCount(); i++)
			{
			indexBuffers_.Get(i)->Reset(technologyInstance_);
			}

		for (int i=0; i<textures_.GetItemCount(); i++)
			{
			textures_.Get(i)->Reset(technologyInstance_);
			}

		if (lastBoundVertexBuffer_)	
			{
			lastBoundVertexBuffer_->Bind();
			}
	
		if (lastBoundIndexBuffer_)	
			{
			lastBoundIndexBuffer_->Bind();
			}

		for (int stage=0; stage<8; stage++)
			{
			if (lastBoundTextures_[stage])
				{
				lastBoundTextures_[stage]->Bind(stage);
				}
			}
		}

	disableOnWmSize_--;
	}


//*** DowngradeTechnology ***

void Platform_Win32_3D::DowngradeTechnology()
	{
	disableOnWmSize_++;

	DebugPrint(("Method failed for technology %d, falling back on technology %d\n",technology_,technology_+1));
	
	if (technologyInstance_)
		{
		delete technologyInstance_;
		technologyInstance_=0;
		}

	Technology newTechnology=(Technology)(technology_+1);
	
	if (newTechnology<Technology_Undefined)
		{
		SetTechnology(newTechnology);
		disableOnWmSize_--;
		return;
		}
	
	technology_=Technology_Undefined;
	technologyInstance_=0;

	disableOnWmSize_--;
	}


//*** SetFullscreen ***

void Platform_Win32_3D::SetFullscreen(bool fullscreen)
	{
	if (fullscreen_==fullscreen)
		{
		return;
		}

	fullscreen_=fullscreen;
	
	if (technologyInstance_)
		{
		disableOnWmSize_++;

		delete technologyInstance_;
		technologyInstance_=0;
	
		if (!GetFullscreen())
			{
			SetWindowSize();
			}

		SetTechnology(technology_);

/*		if (technology_==Technology_DDraw && !GetFullscreen())
			{
			SetWindowSize();
			}
*/
		disableOnWmSize_--;
		}
	}


//*** GetFullscreen ***

bool Platform_Win32_3D::GetFullscreen()
	{
	return fullscreen_;
	}

		
//*** SetSize ***

void Platform_Win32_3D::SetSize(int width, int height)
	{
	windowedWidth_=width;
	windowedHeight_=height;

	if (technologyInstance_)
		{
		disableOnWmSize_++;
	
		delete technologyInstance_;
		technologyInstance_=0;

		if (!GetFullscreen())	
			{
			SetWindowSize();
			}	

		SetTechnology(technology_);

		disableOnWmSize_--;

		}

	}


//*** GetWidth ***

int Platform_Win32_3D::GetWidth()
	{
	if (fullscreen_)
		{
		return fullscreenWidth_;
		}
	
	return windowedWidth_;
	}


//*** GetHeight ***

int Platform_Win32_3D::GetHeight()
	{
	if (fullscreen_)
		{
		return fullscreenHeight_;
		}
	
	return windowedHeight_;
	}


//*** OnRestore ***

void Platform_Win32_3D::OnRestore()
	{
	ignoreNextOnWmSize_=true;
	}


//*** OnWmSize ***

void Platform_Win32_3D::OnWmSize(int width, int height)
	{
	if (disableOnWmSize_ || fullscreen_ || ignoreNextOnWmSize_)
		{
		DebugPrint(("OnWmSize(%d,%d) - DISABLED\n",width,height));
		ignoreNextOnWmSize_=false;
		return;
		}

	disableOnWmSize_++;

	DebugPrint(("OnWmSize(%d,%d) fullscreen_=%d\n",width,height,fullscreen_));

/*	if (technology_==Technology_DDraw && !technologyInstance_)
		{
		SetTechnology(technology_);
		}
*/	
	windowedWidth_=width;
	windowedHeight_=height;

	if (!fullscreen_ && technologyInstance_)
		{
		delete technologyInstance_;
		technologyInstance_=0;

		SetTechnology(technology_);
		}

	disableOnWmSize_--;
	}



//*** OnMinimize ***

void Platform_Win32_3D::OnMinimize()
	{
/*	DebugPrint(("OnMinimize\n"));
	if (technologyInstance_)
		{
		delete technologyInstance_;
		technologyInstance_=0;
		}
*/	}


//*** OnLoseFocus ***

void Platform_Win32_3D::OnLoseFocus()
	{
	disableOnWmSize_++;

	if (fullscreen_ && technologyInstance_)
		{
		disableOnWmSize_++;
		DebugPrint(("OnLoseFocus\n"));
		delete technologyInstance_;
		technologyInstance_=0;
		disableOnWmSize_--;
		}
	}


//*** OnGainFocus ***

void Platform_Win32_3D::OnGainFocus()
	{
	if (fullscreen_ && !technologyInstance_)
		{
		disableOnWmSize_++;
		DebugPrint(("OnGainFocus\n"));
		SetTechnology(technology_);
		disableOnWmSize_--;
		}

	disableOnWmSize_--;
	}


//*** SetWindowSize ***

void Platform_Win32_3D::SetWindowSize()
	{
	DebugPrint(("SetWindowSize\n"));

	// Get the whole window area
	RECT rect1;
	BOOL ret=GetWindowRect(windowHandle_,&rect1);
	Assert(ret,"Couldn't get Window Rect of main window");
	rect1.right-=rect1.left;
	rect1.bottom-=rect1.top;

	// Get the client area
	RECT rect2;
	ret=GetClientRect(windowHandle_,&rect2);
	Assert(ret,"Couldn't get Client Rect of main window");
	rect2.right-=rect2.left;
	rect2.bottom-=rect2.top;

	// Calculate the size of the windows borders/title bar etc.
	int sx=rect1.right-rect2.right;
	int sy=rect1.bottom-rect2.bottom;

	// Set the window to the required dimensions
	MoveWindow(windowHandle_,windowedX_,windowedY_,windowedWidth_+sx,windowedHeight_+sy,TRUE);
	}


//*** IsRunningOnWine ***

bool Platform_Win32_3D::IsRunningOnWine()
	{
    HMODULE module=LoadLibrary("ntdll");
    if (!module)
		{
        return false;
		}

	FARPROC proc=GetProcAddress(module, "wine_get_unix_file_name");
    FreeLibrary(module);

    return proc!=0;
	}


//*** VertexBufferBound ***

void Platform_Win32_3D::VertexBufferBound(Platform_Win32_3D_VertexBuffer* vertexBuffer)
	{
	lastBoundVertexBuffer_=vertexBuffer;
	}


//*** IndexBufferBound ***

void Platform_Win32_3D::IndexBufferBound(Platform_Win32_3D_IndexBuffer* indexBuffer)
	{
	lastBoundIndexBuffer_=indexBuffer;
	}


//*** TextureBound ***

void Platform_Win32_3D::TextureBound(int stage, Platform_Win32_3D_Texture* texture)
	{
	lastBoundTextures_[stage]=texture;
	}


//*** VertexBufferDeleted ***

void Platform_Win32_3D::VertexBufferDeleted(Platform_Win32_3D_VertexBuffer* vertexBuffer)
	{
	if (lastBoundVertexBuffer_==vertexBuffer)	
		{
		lastBoundVertexBuffer_=0;
		}
	ArrayIterator<Platform_Win32_3D_VertexBuffer*> it(vertexBuffers_);
	if (it.Find(vertexBuffer))
		{
		vertexBuffers_.Remove(it);
		}
	}


//*** IndexBufferDeleted ***

void Platform_Win32_3D::IndexBufferDeleted(Platform_Win32_3D_IndexBuffer* indexBuffer)
	{
	if (lastBoundIndexBuffer_==indexBuffer)	
		{
		lastBoundIndexBuffer_=0;
		}

	ArrayIterator<Platform_Win32_3D_IndexBuffer*> it(indexBuffers_);
	if (it.Find(indexBuffer))
		{
		indexBuffers_.Remove(it);
		}
	}


//*** TextureDeleted ***

void Platform_Win32_3D::TextureDeleted(Platform_Win32_3D_Texture* texture)
	{
	for (int stage=0; stage<8; stage++)
		{
		if (lastBoundTextures_[stage]==texture)
			{
			lastBoundTextures_[stage]=0;
			}
		}

	ArrayIterator<Platform_Win32_3D_Texture*> it(textures_);
	if (it.Find(texture))
		{
		textures_.Remove(it);
		}
	}

