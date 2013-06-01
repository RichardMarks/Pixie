/**
 * \file	ColorHelper.h
 * 
 * \ingroup	graphics
 * \author	Mattias Gustavsson	
 * 
 * The ColorHelper.h file defines a bunch of functions which can be used to
 * perform common color operations, such as calculating 32 or 16 bit 
 * color values given separate r, g and b components, or blend two colors
 * using a specified blend factor.
 */

#ifndef __ColorHelper_H__
#define __ColorHelper_H__

#include "StandardLibrary.h"

/**
 * Helper function for alpha blending two colors in the 16 bit R5G6B5 format. 
 */
__forceinline unsigned short AlphaBlend16(
	unsigned short color1, 
	unsigned short color2, 
	unsigned char alpha
	)	
	{
	// These checks are needed to avoid the 16-bit related artefacts we get otherwise
	if (alpha<3)
		{
		return color1;
		}
	if (alpha>252)
		{
		return color2;
		}


	unsigned char invAlpha=255-alpha;
	return 
		((unsigned short)(
			((((((unsigned int)color1 & 0xf800)>>11)*invAlpha)+(((unsigned int)color2 & 0xf800)>>11)*alpha)>>8) <<11 |
			((((((unsigned int)color1 & 0x07e0)>>5 )*invAlpha)+(((unsigned int)color2 & 0x07e0)>>5 )*alpha)>>8) <<5  |
			((((((unsigned int)color1 & 0x001f)    )*invAlpha)+(((unsigned int)color2 & 0x001f)    )*alpha)>>8)
		));
	}

/// Helper function for alpha blending two colors in the 32 bit X8R8G8B8 format. 
__forceinline unsigned int AlphaBlend32(
	unsigned int color1, 
	unsigned int color2, 
	unsigned char alpha
	)
	{
	unsigned char invAlpha=255-alpha;
	return
		(((((color1 & 0x00ff0000)>>16)*invAlpha)+((color2 & 0xff0000)>>16)*alpha)>>8)<<16 |
		(((((color1 & 0x0000ff00)>>8 )*invAlpha)+((color2 & 0x00ff00)>>8 )*alpha)>>8)<<8  |
		(((((color1 & 0x000000ff)    )*invAlpha)+((color2 & 0x0000ff)    )*alpha)>>8)	 |
		(0xff000000);
	}

/// Helper function for multiplying two colors in the 16 bit R5G6B5 format
__forceinline unsigned short RGBModulate16(
	unsigned short color1, 
	unsigned short color2
	)
	{
	return
		((unsigned short)(
			((((((unsigned int)color1 & 0xf800)>>11)*(((unsigned int)color2 & 0xf800)>>11)))>>5) <<11 |
			((((((unsigned int)color1 & 0x07e0)>>5 )*(((unsigned int)color2 & 0x07e0)>>5 )))>>6) <<5  |
			((((((unsigned int)color1 & 0x001f)    )*(((unsigned int)color2 & 0x001f)    )))>>5)
		));
	}

__forceinline unsigned short RGBAdd16(
	unsigned short color1, 
	unsigned short color2
	)
	{
	#define MIN(x,y) (x<y?x:y)
	return
		((unsigned short)(
			MIN((((unsigned int)color1 & 0xf800)>>11)+(((unsigned int)color2 & 0xf800)>>11),0x1f) <<11 |
			MIN((((unsigned int)color1 & 0x07e0)>>5 )+(((unsigned int)color2 & 0x07e0)>>5 ),0x3f) <<5  |
			MIN((((unsigned int)color1 & 0x001f)    )+(((unsigned int)color2 & 0x001f)    ),0x1f)
		));
	}

__forceinline unsigned short RGBSubtract16(
	unsigned short color1, 
	unsigned short color2
	)
	{
	#define MAX(x,y) (x>y?x:y)
	return
		((unsigned short)(
			MAX((((int)color1 & 0xf800)>>11)-(((int)color2 & 0xf800)>>11),0) <<11 |
			MAX((((int)color1 & 0x07e0)>>5 )-(((int)color2 & 0x07e0)>>5 ),0) <<5  |
			MAX((((int)color1 & 0x001f)    )-(((int)color2 & 0x001f)    ),0)
		));
	}





__forceinline unsigned int RGBModulate32(
	unsigned int color1, 
	unsigned int color2
	)
	{
	return
		((unsigned int)(
			((((((unsigned int)color1 & 0xff000000)>>24)*(((unsigned int)color2 & 0xff000000)>>24)))>>8) <<24 |
			((((((unsigned int)color1 & 0x00ff0000)>>16)*(((unsigned int)color2 & 0x00ff0000)>>16)))>>8) <<16 |
			((((((unsigned int)color1 & 0x0000ff00)>>8 )*(((unsigned int)color2 & 0x0000ff00)>>8 )))>>8) <<8  |
			((((((unsigned int)color1 & 0x000000ff)    )*(((unsigned int)color2 & 0x000000ff)    )))>>8)
		));
	}

__forceinline unsigned int RGBAdd32(
	unsigned int color1, 
	unsigned int color2
	)
	{
	#define MIN(x,y) (x<y?x:y)
	return
		((unsigned int)(
			MIN((((unsigned int)color1 & 0xff000000)>>24)+(((unsigned int)color2 & 0xff000000)>>24),0xff) <<24 |
			MIN((((unsigned int)color1 & 0x00ff0000)>>16)+(((unsigned int)color2 & 0x00ff0000)>>16),0xff) <<16 |
			MIN((((unsigned int)color1 & 0x0000ff00)>>8 )+(((unsigned int)color2 & 0x0000ff00)>>8 ),0xff) <<8  |
			MIN((((unsigned int)color1 & 0x000000ff)    )+(((unsigned int)color2 & 0x000000ff)    ),0xff)
		));
	}

__forceinline unsigned int RGBSubtract32(
	unsigned int color1, 
	unsigned int color2
	)
	{
	#define MAX(x,y) (x>y?x:y)
	return
		((unsigned int)(
		MAX((((unsigned int)color1 & 0xff000000)>>24)+(((unsigned int)color2 & 0xff000000)>>24),0) <<24 |
		MAX((((unsigned int)color1 & 0x00ff0000)>>16)+(((unsigned int)color2 & 0x00ff0000)>>16),0) <<16 |
		MAX((((unsigned int)color1 & 0x0000ff00)>>8 )+(((unsigned int)color2 & 0x0000ff00)>>8 ),0) <<8  |
		MAX((((unsigned int)color1 & 0x000000ff)    )+(((unsigned int)color2 & 0x000000ff)    ),0)
		));
	}




__forceinline unsigned short RGB16(
	unsigned char r, 
	unsigned char g, 
	unsigned char b
	)
	{
	return 
		((unsigned short) (
			((((unsigned short)r) & 0xf8)<<8) |
			((((unsigned short)g) & 0xfc)<<3) |
			((((unsigned short)b) & 0xf8)>>3)
		));
	}


__forceinline unsigned int RGB32(
	unsigned char r, 
	unsigned char g, 
	unsigned char b
	)
	{
	return
		((unsigned int) (
			0xff000000	|
			(((unsigned int)r)<<16) |
			(((unsigned int)g)<<8) |
			(((unsigned int)b))
		));
	}


__forceinline unsigned int RGBA32(
	unsigned char r, 
	unsigned char g, 
	unsigned char b, 
	unsigned char a
	)
	{
	return
		((unsigned int) (
			(((unsigned int)a)<<24) |
			(((unsigned int)r)<<16) |
			(((unsigned int)g)<<8) |
			(((unsigned int)b))
		));
	}


// Helper function for converting color values from 32 bit X8R8G8B8 to 16 bit R5G6B5
__forceinline unsigned short RGB32TO16(
	unsigned int color
	)
	{
	return
		((unsigned short) (
			((color & 0x00f80000)>>8) |
			((color & 0x0000fc00)>>5) |
			((color & 0x000000f8)>>3)
		));
	}

// Helper function for converting color values from 16 bit R5G6B5 to 32 bit X8R8G8B8
__forceinline unsigned int RGB16TO32(
	unsigned short color
	)
	{
	return 
		(((unsigned int)color & 0xf800)<<8) |
		(((unsigned int)color & 0x07e0)<<5) |
		(((unsigned int)color & 0x001f)<<3) |
		(0xff000000);
	}

__forceinline unsigned int RGB16TO32(
	unsigned short color, 
	unsigned char alpha
	)
	{
	return 
		(((unsigned int)color & 0xf800)<<8) |
		(((unsigned int)color & 0x07e0)<<5) |
		(((unsigned int)color & 0x001f)<<3) |
		(((unsigned int)alpha) <<24);
	}

// Clamp function used by colorhelper functions to clamp input values
__forceinline float CH_CLAMP(float i) 
	{ 
	if (i<0.0f)
		{
		i=0.0f;
		}

	if (i>1.0f) 
		{
		i=1.0f;
		}

	return i;
	}

// Helper function for converting from float alpha value in the range 0 to 1 to an 8 bit value in the range 0 to 255
__forceinline unsigned char FLOATTO8(
	float a
	)
	{
	return ((unsigned char)(255.0f*CH_CLAMP(a)));
	}


// Helper function for converting from r, g and b float values in the range 0 to 1 to 16 bit R5G6B5
__forceinline unsigned short FLOATTORGB16(
	float r, 
	float g, 
	float b
	)
	{	
	return
		(((unsigned short)Round(31.0f*CH_CLAMP(r)))<<11) |
		(((unsigned short)Round(63.0f*CH_CLAMP(g)))<<5) |
		(((unsigned short)Round(31.0f*CH_CLAMP(b))));
	}

// Helper function for converting from r, g and b float values in the range 0 to 1 to 32 bit X8R8G8B8
__forceinline unsigned int FLOATTORGB32(
	float r, 
	float g, 
	float b
	)
	{
	return
		(((unsigned int)Round(255.0f*CH_CLAMP(r)))<<16) |
		(((unsigned int)Round(255.0f*CH_CLAMP(g)))<<8) |
		(((unsigned int)Round(255.0f*CH_CLAMP(b)))) |
		0xff000000;
	}

// Helper function for converting from r, g, b and a float values in the range 0 to 1 to 32 bit A8R8G8B8
__forceinline unsigned int FLOATTORGBA32(
	float r, 
	float g, 
	float b, 
	float a
	)
	{
	return 
		(((unsigned int)(255.0f*CH_CLAMP(a)))<<24) |
		(((unsigned int)(255.0f*CH_CLAMP(r)))<<16) |
		(((unsigned int)(255.0f*CH_CLAMP(g)))<<8) |
		(((unsigned int)(255.0f*CH_CLAMP(b))));
	}

__forceinline float RGB16TOFLOATR(
	unsigned short color
	)								
	{
	return (((float)((color & 0xf800)>>11))/((float)0x1f));	
	}

__forceinline float RGB16TOFLOATG(
	unsigned short color
	)								
	{
	return (((float)((color & 0x07e0)>>5))/((float)0x3f));
	}

__forceinline float RGB16TOFLOATB(
	unsigned short color
	)								
	{
	return (((float)((color & 0x001f)))/((float)0x1f));
	}

__forceinline float RGB32TOFLOATA(
	unsigned int color
	)								
	{
	return (((float)((color & 0xff000000)>>24))/((float)0xff));
	}


__forceinline float RGB32TOFLOATR(
	unsigned int color
	)								
	{
	return (((float)((color & 0x00ff0000)>>16))/((float)0xff));
	}

__forceinline float RGB32TOFLOATG(
	unsigned int color
	)								
	{
	return (((float)((color & 0x0000ff00)>>8))/((float)0xff));
	}

__forceinline float RGB32TOFLOATB(
	unsigned int color
	)
	{
	return (((float)((color & 0x000000ff)))/((float)0xff));
	}


__forceinline unsigned short RGBMin16(
	unsigned short color1, 
	unsigned short color2
	)	
	{
	#define MIN(x,y) (x<y?x:y)
	unsigned int r1 = (((unsigned int)color1 & 0xf800)>>11);
	unsigned int g1 = (((unsigned int)color1 & 0x07e0)>>5 );
	unsigned int b1 = (((unsigned int)color1 & 0x001f)    );
	unsigned int r2 = (((unsigned int)color2 & 0xf800)>>11);
	unsigned int g2 = (((unsigned int)color2 & 0x07e0)>>5 );
	unsigned int b2 = (((unsigned int)color2 & 0x001f)    );
	return (unsigned short)
		( ( MIN(r1,r2) << 11 ) | ( MIN(g1,g2) << 5 ) | ( MIN(b1,b2) ) );

	}

#endif /* __ColorHelper_H__ */