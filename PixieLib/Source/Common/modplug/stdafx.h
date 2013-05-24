/*
 * This source code is public domain.
 *
 * Authors: Rani Assaf <rani@magic.metawire.com>,
 *          Olivier Lapicque <olivierl@jps.net>,
 *          Adam Goode       <adam@evdebs.org> (endian and char fixes for PPC)
*/

#ifndef _STDAFX_H_
#define _STDAFX_H_

// MG: Make sure this uses my standard library wrappers
#include "StandardLibrary.h"

#pragma warning (disable:4201)
#pragma warning (disable:4514)
#pragma warning (disable:4244)
#pragma warning (disable:4800)
#pragma warning (disable:4702)

#ifdef MSC_VER

#define WIN32_LEAN_AND_MEAN 
#define VC_EXTRALEAN
#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include <stdio.h>

inline void ProcessPlugins(int n) {}

#else

#define WIN32_LEAN_AND_MEAN 
#define VC_EXTRALEAN
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef signed char CHAR;
typedef unsigned char UCHAR;
typedef unsigned char* PUCHAR;
typedef unsigned short USHORT;
#if defined(__x86_64__)
typedef unsigned int ULONG;
typedef unsigned int UINT;
typedef unsigned int DWORD;
typedef int LONG;
typedef long LONGLONG;
typedef int * LPLONG;
typedef unsigned int * LPDWORD;
#else
typedef unsigned long ULONG;
typedef unsigned long UINT;
typedef unsigned long DWORD;
typedef long LONG;
typedef long long LONGLONG;
typedef long * LPLONG;
typedef unsigned long * LPDWORD;
#endif
typedef unsigned short WORD;
typedef unsigned char BYTE;
typedef unsigned char * LPBYTE;
typedef bool BOOL;
typedef char * LPSTR;
typedef void *  LPVOID;
typedef unsigned short * LPWORD;
typedef const char * LPCSTR;
typedef void * PVOID;
typedef void VOID;


inline LONG MulDiv (long a, long b, long c)
{
  // if (!c) return 0;
  return (LONG)(((unsigned long long) a * (unsigned long long) b ) / c);
}

#define MODPLUG_NO_FILESAVE
#define NO_AGC
#define LPCTSTR LPCSTR
#define lstrcpyn StrNCpy
#define lstrcpy StrCpy
#define lstrcmp StrCmp
#define WAVE_FORMAT_PCM 1
//#define ENABLE_EQ

#define  GHND   0

inline signed char * GlobalAllocPtr(unsigned int, size_t size)
{
  signed char * p = (signed char *) malloc(size);

  if (p != NULL) memset(p, 0, (unsigned int)size);
  return p;
}

inline void ProcessPlugins(int n) {}

#define GlobalFreePtr(p) free((void *)(p))

//#define strnicmp(a,b,c)		strncasecmp(a,b,c)
#define wsprintf			sprintf

#ifndef FALSE
#define FALSE	false
#endif

#ifndef TRUE
#define TRUE	true
#endif

#endif // MSC_VER

#endif



