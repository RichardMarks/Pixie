//*** Screen.cpp **

#include "Screen.h"
#include "Bitmap_16Bit.h"
#include "Platform_Screen.h"


//*** Constructor ***

Screen::Screen(int width, int height):
	backBuffer_(0),
	backgroundColor_(0)
	{
	backBuffer_=new Bitmap_16bit(width,height);
	backBuffer_->Clear();
	}


//*** Destructor ***

Screen::~Screen()
	{
	delete backBuffer_;
	}


//*** GetBackbuffer ***

Bitmap& Screen::GetBackBuffer()
	{
	return *backBuffer_;
	}


//*** Present ***

void Screen::Present(unsigned short modulate)
	{
	if (Platform::GetPlatform_Screen())
		{
		Platform::GetPlatform_Screen()->Present(backBuffer_->GetColorData(),backBuffer_->GetHPitch(),backBuffer_->GetVPitch(),modulate,backgroundColor_);
		}
	}


//*** SetBackgroundColor ***

void Screen::SetBackgroundColor(unsigned short color)
	{
	backgroundColor_=color;
	}


//*** GetBackgroundColor ***

unsigned short Screen::GetBackgroundColor()
	{
	return backgroundColor_;
	}


//*** SetFullscreen ***

void Screen::SetFullscreen(bool fullscreen)
	{
	if (Platform::GetPlatform_Screen())
		{
		Platform::GetPlatform_Screen()->SetFullscreen(fullscreen);
		}
	}


//*** GetFullscreen ***

bool Screen::GetFullscreen()
	{
	if (Platform::GetPlatform_Screen())
		{
		return Platform::GetPlatform_Screen()->GetFullscreen();
		}

	return false;
	}


//*** SetInterpolationMode ***

void Screen::SetInterpolationMode(bool InterpolationMode)
	{
	if (Platform::GetPlatform_Screen())
		{
		Platform::GetPlatform_Screen()->SetInterpolationMode(InterpolationMode);
		}
	}


//*** GetInterpolationMode ***

bool Screen::GetInterpolationMode()
	{
	if (Platform::GetPlatform_Screen())
		{
		return Platform::GetPlatform_Screen()->GetInterpolationMode();
		}

	return false;
	}


//*** SetWindowSize ***

void Screen::SetWindowSize(int width, int height)
	{
	if (Platform::GetPlatform_Screen())
		{
		Platform::GetPlatform_Screen()->SetSize(width,height);
		}
	}


//*** GetWindowWidth ***

int Screen::GetWindowWidth()
	{
	if (Platform::GetPlatform_Screen())
		{
		return Platform::GetPlatform_Screen()->GetWidth();
		}

	return 0;
	}


//*** GetWindowHeight ***

int Screen::GetWindowHeight()
	{
	if (Platform::GetPlatform_Screen())
		{
		return Platform::GetPlatform_Screen()->GetHeight();
		}

	return 0;
	}


//*** MatchWindowToScreenSize ***

void Screen::MatchWindowToScreenSize()
	{
	if (Platform::GetPlatform_Screen())
		{
		Platform::GetPlatform_Screen()->SetSize(backBuffer_->GetWidth(),backBuffer_->GetHeight());
		}
	}