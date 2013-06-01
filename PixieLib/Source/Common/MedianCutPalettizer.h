/**
 * \file	MedianCutPalettizer
 * 
 * \ingroup	graphics
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 * \todo	Refactor implementation (no STL)
 */


#ifndef __MedianCutPalettizer_H__
#define __MedianCutPalettizer_H__

// Includes

// Forward declares

// MedianCutPalettizer
class MedianCutPalettizer
	{
	public:
		/**
		 * 
		 * \returns	The number of colors used in the palette (will be less than or equal to paletteMaxSize
		 */
		static int GeneratePalette(
			unsigned int* imageData, 
			int imageWidth,
			int imageHeight,
			unsigned int* palette, 
			int paletteMaxCount
			);

		static int GeneratePalette(
			unsigned short* imageData, 
			int imageWidth,
			int imageHeight,
			unsigned short* palette, 
			int paletteMaxCount
			);

		static int GeneratePalette(
			unsigned int* imageData, 
			int imageWidth,
			int imageHeight,
			unsigned short* palette, 
			int paletteMaxCount
			);

		/**
		 * 
		 */
		static void PalettizeImage(
			unsigned int* imageData, 
			int imageWidth,
			int imageHeight,
			unsigned int* palette, 
			int paletteCount,
			unsigned char* outputData
			);

		static void PalettizeImage(
			unsigned short* imageData, 
			int imageWidth,
			int imageHeight,
			unsigned short* palette, 
			int paletteCount,
			unsigned char* outputData
			);
	};
	 
#endif /* __MedianCutPalettizer_H__ */
