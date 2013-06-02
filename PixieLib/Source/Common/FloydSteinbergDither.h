/**
 * \file	FloydSteinbergDither
 *
 * \ingroup	graphics
 * \brief
 * \author	Mattias Gustavsson
 *
 * \todo	Refactor implementation
 */


#ifndef __FloydSteinbergDither_H__
#define __FloydSteinbergDither_H__

// Includes

namespace pixie {
// Forward declares

// FloydSteinbergDither
class FloydSteinbergDither
	{
	public:
		/**
		 *
		 */
		static void DitherImage(
			unsigned int* imageData,
			int imageWidth,
			int imageHeight,
			unsigned int* palette,
			int paletteCount,
			unsigned char* outputData
			);

		static void DitherImage(
			unsigned int* imageData,
			int imageWidth,
			int imageHeight,
			unsigned short* outputData
			);
	};

}; // namespace pixie

#endif /* __FloydSteinbergDither_H__ */
