//*** Platform_Win32_OS ***

#include "Platform_Win32_OS.h"

#include "Platform_Win32_Input.h"
#include "Platform_Win32_Input_KeyboardDevice.h"
#include "Platform_Screen.h"
#include "Platform_3D.h"

#define WIN32_LEAN_AND_MEAN 
#define VC_EXTRALEAN
#include <crtdbg.h>
#include <stdlib.h>
#include <stdio.h>

//*** Constructor ***

Platform_Win32_OS::Platform_Win32_OS(HINSTANCE hInstance, const char* commandLine):
	hInstance_(hInstance_),
	applicationName_(0),
	executablePath_(0),
	commandLine_(0),
	windowHandle_(0),
	applicationHasFocus_(false),
	cursor_(0),
	exitRequested_(false)
	{
	Platform::RegisterEventListener(this);

	// Store a copy of the command line
	if (commandLine)
		{
//		commandLine_=strdup(commandLine);
		commandLine_=_strdup(commandLine);
		}

	// Retrieve the path of our executable
    char path[MAX_PATH + 1];
    GetModuleFileName(0, path, MAX_PATH );
	char* end=strrchr(path,'\\');
	if (!end)
		{
		strcpy(path,"");
		}
	else
		{
		*end='\0';
		}
//	executablePath_=strdup(path);
	executablePath_=_strdup(path);
	
	// Setup the main application window
	RegisterWindowClass(hInstance,"Pixie");
	windowHandle_=CreateWindowInstance(hInstance,"Pixie","Pixie Game Engine");

	// Store "this" pointer with window, so we can get back to the Platform_Win32_OS instance
	#pragma warning (push)
	#pragma warning( disable: 4244)
	SetWindowLongPtr(windowHandle_, GWLP_USERDATA, (LONG_PTR)this);
	#pragma warning (pop)

	// Load a default Arrow cursor
	cursor_=LoadCursor(0,IDC_ARROW);

	}


//*** Destructor ***

Platform_Win32_OS::~Platform_Win32_OS()
	{
	Platform::UnregisterEventListener(this);

	// Release the executable path
	if (executablePath_)
		{
		free(executablePath_);
		}

	// Release command line data
	if (commandLine_)
		{
		free(commandLine_);
		}

	// Release application name
	if (applicationName_)
		{
		free(applicationName_);
		}
	}


//*** OsYield ***

void Platform_Win32_OS::OsYield()
	{
	Platform::SendEvent_OsYield();
	if (applicationHasFocus_)
		{
		Sleep(0);	// Yield timeslice
		}
	else
		{
		Sleep(16); // Pause for a bit... we're not in a rush here
		}
	
	ProcessMessages(); // Windows message handling
	}


//*** SetApplicationName ***

void Platform_Win32_OS::SetApplicationName(const char* applicationName)
	{
	if (applicationName)
		{
		SetWindowText(windowHandle_,applicationName);
		}
	else
		{
		SetWindowText(windowHandle_,"Pixie Game Engine");
		}
	}


//*** GetCommandLineString ***

const char* Platform_Win32_OS::GetCommandLineString()
	{
	return commandLine_;	
	}


//*** GetExecutablePath ***

const char* Platform_Win32_OS::GetExecutablePath()
	{
	return executablePath_;	
	}


//*** HasFocus ***

bool Platform_Win32_OS::HasFocus()
	{
	return applicationHasFocus_;	
	}


//*** GetWindowHandle ***

HWND Platform_Win32_OS::GetWindowHandle()
	{
	return windowHandle_;
	}


//*** RegisterWindowClass ***

bool Platform_Win32_OS::RegisterWindowClass(HINSTANCE instance, const char* className)
	{
	WNDCLASS wc;
	wc.style		=	CS_GLOBALCLASS|CS_BYTEALIGNCLIENT;
	wc.lpfnWndProc	=	(WNDPROC) WindowProcedure_Static;
	wc.cbClsExtra	=	0;
	wc.cbWndExtra	=	0;
	wc.hInstance	=	instance;
	wc.hCursor		=	LoadCursor(0, IDC_ARROW);
	wc.hIcon		=	0;
	wc.hbrBackground=	(HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName	=	0;
	wc.lpszClassName=	className;
	
	if(!RegisterClass(&wc))
		{
		MessageBox(0, "Failed to register window class.", "Error", MB_OK|MB_ICONSTOP);
		return false;
		}

	return true;
	}


//*** CreateWindowInstance ***

HWND Platform_Win32_OS::CreateWindowInstance(HINSTANCE instance, const char* className, const char* title)
	{
	DWORD style;
	style=WS_OVERLAPPEDWINDOW;
	HWND hWnd=CreateWindow(className,title,style,0,0,320,200,(HWND)0, (HMENU)0, instance, 0);
	return hWnd;
	}


//*** ProcessMessages ***

void Platform_Win32_OS::ProcessMessages()
	{
	// Store the flag for whether the application has focus
	// or not, so we can call notify Platform_Screen after
	// the message pump
	bool previousFocus=applicationHasFocus_;

	// Standard windows message pump
	MSG	msg;
	while (PeekMessage(&msg, 0, 0,0, PM_REMOVE))
		{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		}
	
	// We need to handle OnGainFocus and OnLoseFocus in Platform_Screen, but
	// not inside the message pump, or D3D will throw a fit (don't know why)
	if (previousFocus!=applicationHasFocus_)
		{
		if (applicationHasFocus_)
			{
			Platform::SendEvent_CustomEvent("OnGainFocus");
			}
		else			
			{
			Platform::SendEvent_CustomEvent("OnLoseFocus");
			}
		}
	}


//*** WindowProcedure_Static ***

LRESULT CALLBACK Platform_Win32_OS::WindowProcedure_Static(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
	{
	#pragma warning (push)
	#pragma warning( disable: 4312)
	Platform_Win32_OS* this_=reinterpret_cast<Platform_Win32_OS*>(GetWindowLongPtr(window, GWLP_USERDATA));
	#pragma warning (pop)
	
	if (this_)
		{
		return this_->WindowProcedure(window,message,wParam,lParam);
		}

	return DefWindowProc(window,message,wParam,lParam);
	}


//*** WindowProcedure ***

LRESULT Platform_Win32_OS::WindowProcedure(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
	{
	static WPARAM storedKeydown;

//	SetCursor(cursor_);

	switch(message)
		{
		case WM_CHAR:
			{
			if (applicationHasFocus_)
				{
				int params[2];
				params[0]=(int)wParam;
				params[1]=(int)storedKeydown;
				Platform::SendEvent_CustomEvent("OnWmChar",params);
				}
			return 0;
			} break;


		case WM_KEYDOWN:
			{
			if (applicationHasFocus_)
				{
				storedKeydown=(int)wParam;
				}
			return 0;
			} break;

		case WM_KEYUP:
			{
			int param=(int)wParam;
			Platform::SendEvent_CustomEvent("OnWmKeyUp",&param);

			return 0;
			} break;


		case WM_SETCURSOR:
			{
			SetCursor(cursor_);
			return 0;
			} break;

		case WM_SIZE:
			{
			return WmSize(window,message,wParam,lParam);
			} break;

		case WM_SYSCOMMAND: 
			{
			return WmSysCommand(window,message,wParam,lParam);
			} break;
		
		case WM_ACTIVATEAPP:
			{
			return WmActivateApp(window,message,wParam,lParam);
			} break;


		case WM_CLOSE:
			{
			return WmClose(window,message,wParam,lParam);
			} break;
			
		}

	return DefWindowProc(window, message, wParam, lParam);
}


//*** WmSize ***

LRESULT Platform_Win32_OS::WmSize(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
	{
	if (applicationHasFocus_)
		{
		if (wParam!=SIZE_MAXHIDE && wParam!=SIZE_MAXSHOW && wParam!=SIZE_MINIMIZED)
			{
			int params[2];
			params[0]=LOWORD(lParam);
			params[1]=HIWORD(lParam);
			Platform::SendEvent_CustomEvent("OnWmSize",params);
			}
		else if (wParam==SIZE_MINIMIZED)
			{
			Platform::SendEvent_CustomEvent("OnMinimize");
			}
		}
	
	return DefWindowProc(window, message, wParam, lParam);
	}


//*** WmSysCommand ***

LRESULT Platform_Win32_OS::WmSysCommand(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
	{
	
	if (wParam==SC_RESTORE)
		{
		Platform::SendEvent_CustomEvent("OnRestore");
		}

	return DefWindowProc(window, message, wParam, lParam);
	}


//*** WmActivateApp ***

LRESULT Platform_Win32_OS::WmActivateApp(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
	{
	if(wParam)
		{
		if (!applicationHasFocus_)
			{
			applicationHasFocus_=true;
			}
		}
	else
		{
		if (applicationHasFocus_)
			{
			applicationHasFocus_=false;
			}
		}

	return 0;
	} 


//*** WmClose ***

LRESULT Platform_Win32_OS::WmClose(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
	{
	exitRequested_=true;
	return 0;
	}


//*** ExitRequested ***

bool Platform_Win32_OS::ExitRequested()
	{
	return exitRequested_;
	}


//*** DisplayAssertMessage ***

void Platform_Win32_OS::DisplayAssertMessage(const char* expression, const char* message, const char* file, int line)
	{
	if (Platform::GetPlatform_Screen())
		{
		Platform::GetPlatform_Screen()->SetFullscreen(false);
		}
	if (Platform::GetPlatform_3D())
		{
		Platform::GetPlatform_3D()->SetFullscreen(false);
		}
	char buf[4096];
	_snprintf(buf,4095,"ASSERTION FAILED!\n\n%s\n\nExpression: %s\nFile: %s\nLine: %d\n",message,expression,file,line);
	OutputDebugString("******************************************\n");
	OutputDebugString(buf);
	OutputDebugString("******************************************\n");
	int result=MessageBox(0,buf,"Pixie Assert",MB_ICONWARNING | MB_ABORTRETRYIGNORE);
	switch (result)
		{
		case IDABORT:
			{
			// Turn off memory leak reports
			#ifndef NDEBUG
				int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG); // Get current flag
				flag ^= _CRTDBG_LEAK_CHECK_DF; // Turn off leak-checking bit
				_CrtSetDbgFlag(flag); // Set flag to the new value
			#endif
			_exit(3);		// Exit application immediately, without calling crt's _atexit
			} break;
		case IDRETRY:
			{
			_asm{int 0x03};	// Break to editor
			} break;
		case IGNORE:		
			{
			return;			// Continue execution
			} break;
		}
	}


//*** OutputDebugText ***

void Platform_Win32_OS::OutputDebugText(const char * formatstr, ...)
	{
	va_list arglist;
	char str[4096];

	va_start(arglist, formatstr);
	_vsnprintf(str, 4095, formatstr, arglist);
	va_end(arglist);

	OutputDebugString(str);
	}


//*** ReportFatalError ***

void Platform_Win32_OS::ReportFatalError(const char* message, const char* file, int line)
	{
	if (Platform::GetPlatform_Screen())
		{
		Platform::GetPlatform_Screen()->SetFullscreen(false);
		}
	if (Platform::GetPlatform_3D())
		{
		Platform::GetPlatform_3D()->SetFullscreen(false);
		}

	INT mbtype=MB_OK;
	if (IsDebuggerPresent())
		{
		mbtype=MB_OKCANCEL;
		}

	char buf[4096];
	_snprintf(buf,4095,"FATAL ERROR!\n\n%s\n\nIt is not possible to recover from this error. The program will now terminate.\n\nFile: %s\nLine: %d\n",message,file,line);
	OutputDebugString("******************************************\n");
	OutputDebugString(buf);
	OutputDebugString("******************************************\n");
	int result=MessageBox(0,buf,"Pixie Fatal Error",MB_ICONSTOP | mbtype);
	switch (result)
		{
		case IDOK:
			{
			// Turn off memory leak reports
			#ifndef NDEBUG
				int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG); // Get current flag
				flag ^= _CRTDBG_LEAK_CHECK_DF; // Turn off leak-checking bit
				_CrtSetDbgFlag(flag); // Set flag to the new value
			#endif
			_exit(3);		// Exit application immediately, without calling crt's _atexit
			} break;

		case IDCANCEL:
			{
			_asm{int 0x03};	// Break to editor
			} break;
		}
	}


//*** OnCustomEvent ***

void Platform_Win32_OS::OnCustomEvent(const char* eventId, void* userData)
	{
//	if (stricmp(eventId,"SetCursor")==0)
	if (_stricmp(eventId,"SetCursor")==0)
		{
		cursor_=(HCURSOR)userData;;
		return;
		}
	}

