/**
 * \file	Debug.h
 * 
 * \ingroup	core
 * \brief	Useful debug functions
 * \author	Mattias Gustavsson	
 * 
 * This file contains a few macros that are handy to make your code more easy to debug, like Assert and DebugPrint.
 * They are really just wrappers to make it easier to have the functions compile out on certain configurations, and
 * the actual implementation of their functionality is implemented elsewhere, specific for the current platform.
 */

#ifndef __Debug_H__
#define __Debug_H__

// Includes
#include "Platform_OS.h"

// These can be useful to turn on when debugging release builds - when enabled, asserts and debug print will be 
// included even in release builds
//#define FORCE_DEBUGPRINT 
//#define FORCE_ASSERT 

// We want to be able to do Assert(false,"message"), so we will disable this warning for all files that include Debug.h
#pragma warning( disable : 4127) // conditional expression is constant


/**
 * Assert
 */
#if defined(_DEBUG) || defined(FORCE_ASSERT)

	#define Assert(expression,message)																	\
		if (!(expression) && Platform::GetPlatform_OS())												\
			{																							\
			Platform::GetPlatform_OS()->DisplayAssertMessage(#expression, message, __FILE__, __LINE__);	\
			}																							\

#else

	#define Assert(expression,message)

#endif


/**
 * Fatal Error
 */
#define FatalError(message)															\
	if (Platform::GetPlatform_OS())													\
		{																			\
		Platform::GetPlatform_OS()->ReportFatalError(message, __FILE__, __LINE__);	\
		}																			\
	

/**
 * Debug Print
 */
#if defined(_DEBUG) || defined(FORCE_DEBUGPRINT)

	#define DebugPrint(x)									\
		if (Platform::GetPlatform_OS())						\
			{												\
			Platform::GetPlatform_OS()->OutputDebugText x;	\
			}												\

#else

	#define DebugPrint(x) 

#endif

#define CTASTR2(pre,post) pre ## post
#define CTASTR(pre,post) CTASTR2(pre,post)
#define StaticAssert(cond,msg) \
    typedef struct CTASTR(StaticAssert_,__COUNTER__) { int msg : !!(cond); } \
        CTASTR(static_assertion_failed_,__COUNTER__)


#endif /* __Debug_H__ */
