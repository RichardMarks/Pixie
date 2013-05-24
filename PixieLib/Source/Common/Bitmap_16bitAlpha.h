/**
 * \class	Bitmap_16bitAlpha
 * 
 * \ingroup	graphics
 * \brief	Uncompressed 16-bit color buffer with alpha channel
 * \author	Mattias Gustavsson	
 * 
 * An Bitmap_16bitAlpha is simply a collection of 16-bit color values with a width and a 
 * height, plus a collection of 8-bit alpha values with the same dimesnsions.
 * 
 */

#ifndef __Bitmap_16bitAlpha_H__
#define __Bitmap_16bitAlpha_H__

// Includes
#include "Bitmap.h"

// Forward declares
class Image;


// Bitmap_16bitAlpha
class Bitmap_16bitAlpha:public Bitmap
	{
	public:
		Bitmap_16bitAlpha();

		Bitmap_16bitAlpha(
			const Asset& asset
			);

		/**
		 * Creates an empty bitmap with the specified dimensions. All pixels will be
		 * set to black (0) at creation.
		 */
		Bitmap_16bitAlpha(
			int width,	///< The width of the bitmap, in pixels
			int height	///< The height of the bitmap, in pixels
			);

		/**
		 * Creates a bitmap from an Image. The width and the height of the bitmap will
		 * be taken from the image, the colors reduced from 32 to 16 bits, and the
		 * alpha channel is dropped.
		 */
		Bitmap_16bitAlpha(
			const Image& image,	///< Image to create bitmap from
			bool dither = false
			);

		~Bitmap_16bitAlpha();

		virtual StringId GetType();

		virtual void Save(Asset& asset) const;
		virtual void Load(const Asset& asset);

		virtual void WriteToAsset(Asset* asset) const;
		virtual void ReadFromAsset(const Asset* asset);

	};




#endif /* __Bitmap_16bitAlpha_H__ */
	