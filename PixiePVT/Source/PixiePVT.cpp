//*** PixiePVT.cpp ***

#include "PixiePVT.h"

#include "CommandLine.h"
#include "Screen.h"
#include "ImageFormat_TGA.h"
#include "ImageFormat_GIF.h"
#include "ImageFormat_PNG.h"
#include "ImageFormat_JPG.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "Resource_BitmapStrip.h"
#include "Platform_OS.h"
#include "Platform_Screen.h"
#include "Platform_FileSystem.h"
#include "Platform_FileSystem_File.h"
#include "Platform_FileSystem_Directory.h"
#include "StandardLibrary.h"
#include "ColorHelper.h"
#include "FrameTime.h"

StringId GetPath(StringId filename);
StringId GetFilename(StringId filename);
bool VerifyFilename(const char* name);
Screen* LoadImage(Screen* screen, StringId filename,Resource_BitmapStrip& bitmap, bool fullscreen);
void DrawBackground(Bitmap& bitmap);


//*** PixiePVTMain ***

void PixiePVTMain()
	{
	ImageFormat_PNG::Register();
	ImageFormat_JPG::Register();
	ImageFormat_GIF::Register();
	ImageFormat_TGA::Register();

	Platform::GetPlatform_Screen()->SetFullscreen(false);

	ResourceManager resourceManager;
	InputManager inputManager;
	inputManager.HideCursor();

	CommandLine cmd;
	
	StringId filename=cmd.GetCommandLineString(0);

	if (Platform::GetPlatform_FileSystem())
		{	
		Platform_FileSystem_File* file=Platform::GetPlatform_FileSystem()->CreateFileObject(filename.GetString());
		if (!file->Exists())
			{
			return;
			}
		delete file;
		}
	else
		{
		return;
		}

	if (!VerifyFilename(filename.GetString()))
		{
		return;
		}

	bool fullscreen=false;
	int screenWidth=Platform::GetPlatform_Screen()->GetWidth();
	int screenHeight=Platform::GetPlatform_Screen()->GetHeight();
	Array<StringId> files;
	StringId path=GetPath(filename);
	StringId filenameOnly=GetFilename(filename);
	Platform_FileSystem_Directory* directory=Platform::GetPlatform_FileSystem()->CreateDirectoryObject(path.GetString());
	int fileIndex=0;
	for (int i=0; i<directory->GetFileCount(); i++)
		{
		const char* name=directory->GetFile(i);
		if (VerifyFilename(name))
			{
			char buffer[4096];
			SNPrintF(buffer,4096,"%s/%s",path.GetString(),name);	
			files.Add(StringId(buffer));
			if (filenameOnly==StringId(name))
				{
				fileIndex=i;
				}
			}
		}
	delete directory;

	float cel=0;

	Resource_BitmapStrip bitmap;
	Screen* screen=LoadImage(0,filename, bitmap, fullscreen);

	FrameTime frameTime;

	bool exitFlag=false;
	while (!exitFlag)
		{
		cel+=frameTime.Update()*5;
		DrawBackground(screen->GetBackBuffer());
		int x=(screen->GetBackBuffer().GetWidth()-bitmap.GetWidth((int)cel))/2;
		int y=(screen->GetBackBuffer().GetHeight()-bitmap.GetHeight((int)cel))/2;
		if (x>0 || y>0)
			{
			int w=screen->GetBackBuffer().GetWidth();
			int h=screen->GetBackBuffer().GetHeight();
			screen->GetBackBuffer().Fill(0,0,x,h,0);
			screen->GetBackBuffer().Fill(w-x,0,w,h,0);
			screen->GetBackBuffer().Fill(x,0,w-x,y,0);
			screen->GetBackBuffer().Fill(x,h-y,w-x,h,0);
			}
		bitmap.Blit((int)cel,screen->GetBackBuffer(),x,y);
		screen->Present();

		inputManager.Update();
		if (inputManager.WasKeyPressed(KEY_ESCAPE))
			{
			exitFlag=true;
			}
		if (inputManager.WasKeyPressed(KEY_SPACE))
			{
			fileIndex++;
			if (fileIndex>=files.GetItemCount())
				{
				fileIndex=0;
				}
			
			screen=LoadImage(screen, files.Get(fileIndex),bitmap, fullscreen);
			cel=0;
			}
		if (inputManager.WasKeyPressed(KEY_BACK))
			{
			fileIndex--;
			if (fileIndex<0)
				{
				fileIndex=files.GetItemCount()-1;
				}
			screen=LoadImage(screen,files.Get(fileIndex),bitmap,fullscreen);
			cel=0;
			}

		// Toggle between fullscreen and windowed mode on Alt+Enter
		if (inputManager.IsKeyDown(KEY_MENU) && inputManager.WasKeyPressed(KEY_RETURN))
			{
			fullscreen=!fullscreen;
			delete screen;
			screen=new Screen(screenWidth,screenHeight);
			screen->SetFullscreen(fullscreen);
			screen->SetInterpolationMode(false);
			screen=LoadImage(screen, files.Get(fileIndex),bitmap, fullscreen);
			inputManager.RestoreCursor();
			}

		
		if (Platform::GetPlatform_OS())
			{
			Platform::GetPlatform_OS()->OsYield();
			if (Platform::GetPlatform_OS()->ExitRequested())
				{
				exitFlag=true;
				}
			}

		}

	delete screen;
	}


//*** GetPath ***

StringId GetPath(StringId filename)
	{
	char* path=StrDup(filename.GetString());
	int len=StrLen(path);

	// Remove trailing '/' or '\' (if any)
	if (path[len-1]=='\\' || path[len-1]=='/')
		{
		path[len-1]=0;
		len--;
		}
	
	// Get the path (strip away the filename)
	const char* fileStart=StrRChr(path,'\\');
	const char* fileStart2=StrRChr(path,'/');
	if (fileStart==0 || (fileStart2 && fileStart2>fileStart))
		{
		fileStart=fileStart2;
		}
	if (fileStart>0)
		{
		fileStart++;
		}
	else
		{
		fileStart=path;
		}
	*(char*)fileStart=0;
	if (StrLen(path)==0)
		{
		Free(path);
		return ".";
		}
	StringId returnValue(path);
	Free(path);
	return returnValue;
	}


//*** GetFilename ***

StringId GetFilename(StringId filename)
	{
	const char* str=filename.GetString();
	const char* lastDelimiter=StrRChr(str,'/');
	const char* lastDelimiter2=StrRChr(str,'\\');


	if (lastDelimiter && lastDelimiter2)
		{
		if (lastDelimiter>lastDelimiter2)
			{
			return StringId(lastDelimiter+1);
			}
		else
			{
			return StringId(lastDelimiter2+1);
			}
		}

	if (lastDelimiter)
		{
		return StringId(lastDelimiter+1);
		}
	if (lastDelimiter2)
		{
		return StringId(lastDelimiter2+1);
		}

	return filename;
	}


//*** VerifyFilename *** 

bool VerifyFilename(const char* name)
	{
	if (StrLen(name)<4)
		{
		return false;
		}
	
	const char* end=name+StrLen(name);
	
	if (StrICmp(end-3,".am")==0)
		{
		return true;
		}

	if (StrICmp(end-3,".bm")==0)
		{
		return true;
		}

	if (StrICmp(end-4,".abm")==0)
		{
		return true;
		}

	if (StrICmp(end-4,".rle")==0)
		{
		return true;
		}

	if (StrICmp(end-4,".pix")==0)
		{
		return true;
		}

	if (StrICmp(end-4,".tga")==0)
		{
		return true;
		}

	if (StrICmp(end-4,".png")==0)
		{
		return true;
		}

	if (StrICmp(end-4,".gif")==0)
		{
		return true;
		}

	if (StrICmp(end-4,".jpg")==0)
		{
		return true;
		}

	return false;
	}


//*** LoadImage ***

Screen* LoadImage(Screen* screen, StringId filename, Resource_BitmapStrip& bitmap, bool fullscreen)
	{
	bitmap=Resource_BitmapStrip(filename);
	if (!fullscreen)
		{
		if (screen)
			{
			delete screen;
			}
		screen=new Screen(bitmap.GetWidth(),bitmap.GetHeight());
		screen->SetFullscreen(fullscreen);
		screen->SetInterpolationMode(false);
		screen->MatchWindowToScreenSize();
		}

	DrawBackground(screen->GetBackBuffer());
	bitmap.Blit(screen->GetBackBuffer(),0,0);

	if (Platform::GetPlatform_OS())
		{
		char buffer[4096];
		if (bitmap.GetCelCount()==1)
			{
			SNPrintF(buffer,4096,"PixView - %s %dx%d",GetFilename(filename).GetString(),bitmap.GetWidth(),bitmap.GetHeight());
			Platform::GetPlatform_OS()->SetApplicationName(buffer);
			}
		else
			{
			SNPrintF(buffer,4096,"PixView - %s %dx%d - %d cels",GetFilename(filename).GetString(),bitmap.GetWidth(),bitmap.GetHeight(),bitmap.GetCelCount());
			Platform::GetPlatform_OS()->SetApplicationName(buffer);
			}
		}


	return screen;
	}


//*** DrawBackground ***

void DrawBackground(Bitmap& bitmap)
	{
	const int gridSize=8;
	unsigned short light=RGB32TO16(0x999999);
	unsigned short dark=RGB32TO16(0x666666);

	for (int y=0; y<bitmap.GetHeight()/gridSize; y++)
		{
		for (int x=0; x<bitmap.GetWidth()/gridSize; x++)
			{
			int c=((x&2)+(y&2))&2;
			unsigned short color=(c?light:dark);
			bitmap.Fill(x*gridSize,y*gridSize,x*gridSize+gridSize,y*gridSize+gridSize,color);
			}
		}
	}