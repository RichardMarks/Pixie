
/**
 * \class	MaxBlitterRLE8
 * 
 * \ingroup	graphics
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 */

#ifndef __MaxBlitterRLE8_H__
#define __MaxBlitterRLE8_H__

// Includes

// Forward declares


// MaxBlitterRLE8
class MaxBlitterRLE8
	{
	public:
		__forceinline static void Opaque_Unclipped_Unmasked(unsigned char* opaqueData, int activeWidth, int activeHeight, unsigned short* palette, unsigned short* data, int backBufferDelta,int x, int y);
		__forceinline static void Opaque_Unclipped_Masked(unsigned char* opaqueData, int activeWidth, int activeHeight, unsigned short* palette, unsigned short* data, int backBufferDelta,int x, int y);
		__forceinline static void Opaque_Unclipped_Unmasked_Transparent(unsigned char* opaqueData, int activeWidth, int activeHeight, unsigned short* palette, unsigned short* data, int backBufferDelta,int x, int y, unsigned char alpha);
		__forceinline static void Opaque_Unclipped_Masked_Transparent(unsigned char* opaqueData, int activeWidth, int activeHeight, unsigned short* palette, unsigned short* data, int backBufferDelta,int x, int y, unsigned char alpha);
		
		__forceinline static void Opaque_Clipped_Unmasked(unsigned char* opaqueData, int activeWidth, int activeHeight, unsigned short* palette, unsigned short* data, int backBufferDelta,int x, int y, int xStart, int yStart, int xEnd, int yEnd);
		__forceinline static void Opaque_Clipped_Masked(unsigned char* opaqueData, int activeWidth, int activeHeight, unsigned short* palette, unsigned short* data, int backBufferDelta,int x, int y, int xStart, int yStart, int xEnd, int yEnd);
		__forceinline static void Opaque_Clipped_Unmasked_Transparent(unsigned char* opaqueData, int activeWidth, int activeHeight, unsigned short* palette, unsigned short* data, int backBufferDelta,int x, int y, int xStart, int yStart, int xEnd, int yEnd, unsigned char alpha);
		__forceinline static void Opaque_Clipped_Masked_Transparent(unsigned char* opaqueData, int activeWidth, int activeHeight, unsigned short* palette, unsigned short* data, int backBufferDelta,int x, int y, int xStart, int yStart, int xEnd, int yEnd, unsigned char alpha);

		__forceinline static void Alpha_Unclipped(unsigned char* alphaData, int activeWidth, int activeHeight, unsigned short* palette, unsigned short* data, int backBufferDelta,int x, int y);
		__forceinline static void Alpha_Clipped(unsigned char* alphaData, int activeWidth, int activeHeight, unsigned short* palette, unsigned short* data, int backBufferDelta,int x, int y, int xStart, int yStart, int xEnd, int yEnd);
		__forceinline static void Alpha_Unclipped_Transparent(unsigned char* alphaData, int activeWidth, int activeHeight, unsigned short* palette, unsigned short* data, int backBufferDelta,int x, int y, unsigned char alpha);
		__forceinline static void Alpha_Clipped_Transparent(unsigned char* alphaData, int activeWidth, int activeHeight, unsigned short* palette, unsigned short* data, int backBufferDelta,int x, int y, int xStart, int yStart, int xEnd, int yEnd, unsigned char alpha);


	private:

		__forceinline static void Fill(unsigned short* data, unsigned short color, int len);
//		__forceinline static void Blend(unsigned short* destination, unsigned int color, unsigned int alpha);
		__forceinline static void FillTransparent(unsigned short* data, unsigned short color, int len, unsigned char alpha);
		__forceinline static void BurstFill(unsigned short* data, unsigned short color, int len);

		__forceinline static void RunLength_Opaque_Unclipped_Unmasked(int len,unsigned short* data, unsigned char** source, unsigned short* palette);
		__forceinline static void RunLength_Opaque_Unclipped_Masked(int len,unsigned short* data, unsigned char** source, unsigned short* palette);
		__forceinline static void RunLength_Opaque_Unclipped_Unmasked_Transparent(int len,unsigned short* data, unsigned char** source, unsigned short* palette, unsigned char alpha);
		__forceinline static void RunLength_Opaque_Unclipped_MaskedTransparent(int len,unsigned short* data, unsigned char** source, unsigned short* palette, unsigned char alpha);

		__forceinline static void RunLength_Opaque_Clipped_Masked(int len,unsigned short* data, unsigned char** source, unsigned short* palette, int clipStart, int clipEnd);
		__forceinline static void RunLength_Opaque_Clipped_Masked_Transparent(int len,unsigned short* data, unsigned char** source, unsigned short* palette, int clipStart, int clipEnd, unsigned char alpha);

		__forceinline static void IgnoreOpaque(int len, unsigned char** source);
		__forceinline static void IgnoreAlpha(int len, unsigned char** source);

	};

#include "MaxBlitterRLE8.inl"

#endif /* __MaxBlitterRLE8_H__ */
	