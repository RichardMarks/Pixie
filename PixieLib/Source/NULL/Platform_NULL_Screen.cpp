//*** Platform_NULL_Screen.cpp ***

#include "Platform_NULL_Screen.h"


//*** Constructor ***

Platform_NULL_Screen::Platform_NULL_Screen()
	{
	}


//*** Present ***

void Platform_NULL_Screen::Present(unsigned short* bitmapData, int bitmapWidth, int bitmapHeight, unsigned short modulate, unsigned short backgroundColor)
	{
	}


//*** SetInterpolationMode *** 

void Platform_NULL_Screen::SetInterpolationMode(bool enabled)
	{
	}


//*** GetInterpolationMode *** 

bool Platform_NULL_Screen::GetInterpolationMode()
	{
	return true;
	}


//*** SetFullscreen ***

void Platform_NULL_Screen::SetFullscreen(bool fullscreen)
	{
	}


//*** GetFullscreen ***

bool Platform_NULL_Screen::GetFullscreen()
	{
	return false;
	}

		
//*** SetSize ***

void Platform_NULL_Screen::SetSize(int width, int height)
	{
	}


//*** GetWidth ***

int Platform_NULL_Screen::GetWidth()
	{
	return 0;
	}


//*** GetHeight ***

int Platform_NULL_Screen::GetHeight()
	{
	return 0;
	}


//*** TransformCursorCoordinates ***

void Platform_NULL_Screen::TransformCursorCoordinates(float& x, float& y)
	{
	}


