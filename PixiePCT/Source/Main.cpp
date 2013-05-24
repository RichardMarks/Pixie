//*** Main.cpp ***

#include "PixiePCT.h"


// Win32 Platform Includes
#include "../Win32/Platform_Win32_OS.h"
#include "../Win32/Platform_Win32_FileSystem.h"

// Need to have memory-leak checking
#include <crtdbg.h>

#include <string.h>
#include <windows.h>

//*** main ***

int main(int argc, const char* argv[])
	{
	// Enable windows memory leak detection
	#ifdef _DEBUG
		int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG); // Get current flag
		flag |= _CRTDBG_LEAK_CHECK_DF; // Turn on leak-checking bit
		_CrtSetDbgFlag(flag); // Set flag to the new value

//		_CrtSetBreakAlloc(117); // Can be manually commented back in to break at a certain allocation
	#endif
	
	// Platform abstraction systems - This is where we choose which platform components we want to use
	Platform::SetPlatform_OS(new Platform_Win32_OS(0,0));
	Platform::SetPlatform_FileSystem(new Platform_Win32_FileSystem());

	// Convert command line to single string
	size_t l=argc+1;
	for (int i=0; i<argc; i++)
		{
		l+=strlen(argv[i]);
		}
	char* commandLine=new char[l];
	strcpy(commandLine,"");
	for (int i=0; i<argc; i++)
		{
		strcat(commandLine,argv[i]);
		strcat(commandLine," ");
		}

	// Run the converter
	PixiePCTMain(commandLine);
	

	// Exit
	delete[] commandLine;
	return 0;
	}

