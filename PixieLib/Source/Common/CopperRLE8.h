/**
 * \class	CopperRLE8
 * 
 * \ingroup	graphics
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 */

#ifndef __CopperRLE8_H__
#define __CopperRLE8_H__

// Includes

// Forward declares


// CopperRLE8
class CopperRLE8
	{
	public:
		static void Opaque_Unclipped_Unmasked(unsigned char* opaqueData, int activeWidth, int activeHeight, unsigned short* palette, unsigned short* colorData, unsigned char* alphaData, int backBufferDelta,int x, int y);
		static void Opaque_Unclipped_Masked(unsigned char* opaqueData, int activeWidth, int activeHeight, unsigned short* palette, unsigned short* colorData, unsigned char* alphaData, int backBufferDelta,int x, int y);
		static void Opaque_Clipped_Unmasked(unsigned char* opaqueData, int activeWidth, int activeHeight, unsigned short* palette, unsigned short* colorData, unsigned char* alphaData, int backBufferDelta,int x, int y, int xStart, int yStart, int xEnd, int yEnd);
		static void Opaque_Clipped_Masked(unsigned char* opaqueData, int activeWidth, int activeHeight, unsigned short* palette, unsigned short* colorData, unsigned char* alphaData, int backBufferDelta,int x, int y, int xStart, int yStart, int xEnd, int yEnd);
		static void Alpha_Unclipped(unsigned char* alphaRLEData, int activeWidth, int activeHeight, unsigned short* palette, unsigned short* colorData, unsigned char* alphaData, int backBufferDelta,int x, int y);
		static void Alpha_Clipped(unsigned char* alphaRLEData, int activeWidth, int activeHeight, unsigned short* palette, unsigned short* colorData, unsigned char* alphaData, int backBufferDelta,int x, int y, int xStart, int yStart, int xEnd, int yEnd);
	private:
	
		static void RunLength_Opaque_Unclipped_Unmasked(int len,unsigned short* colorData, unsigned char* alphaData, unsigned char** source, unsigned short* palette);
		static void RunLength_Opaque_Clipped_Masked(int len,unsigned short* colorData, unsigned char* alphaData, unsigned char** source, unsigned short* palette, int clipStart, int clipEnd);
		static void RunLength_Opaque_Unclipped_Masked(int len,unsigned short* colorData, unsigned char* alphaData, unsigned char** source, unsigned short* palette);

		static void Fill(unsigned short* data, unsigned short color, int len);
		static void BurstFill(unsigned short* data, unsigned short color, int len);
		static void AlphaFill(unsigned char*data, int len);
		static void AlphaBurstFill(unsigned char* data, int len);
		static void IgnoreOpaque(int len, unsigned char** source);
		static void IgnoreAlpha(int len, unsigned char** source);
	

	};

#endif /* __CopperRLE8_H__ */
	