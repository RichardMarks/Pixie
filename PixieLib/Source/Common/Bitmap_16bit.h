/**
 * \class	Bitmap_16bit
 * 
 * \ingroup	graphics
 * \brief	Uncompressed 16-bit color buffer without alpha channel
 * \author	Mattias Gustavsson	
 * 
 *
 */

#ifndef __Bitmap_16bit_H__
#define __Bitmap_16bit_H__

// Includes
#include "Bitmap.h"

// Forward declares
class Image;


// Bitmap_16bit
class Bitmap_16bit:public Bitmap
	{
	public:
		Bitmap_16bit();

		Bitmap_16bit(const Asset& asset);

		/**
		 * Creates an empty bitmap with the specified dimensions. 
		 */
		Bitmap_16bit(
			int width,	///< The width of the bitmap, in pixels
			int height	///< The height of the bitmap, in pixels
			);

		/**
		 * Creates a bitmap from an Image. The width and the height of the bitmap will
		 * be taken from the image, the colors reduced from 32 to 16 bits, and the
		 * alpha channel is dropped.
		 */
		Bitmap_16bit(
			const Image& image,	///< Image to create bitmap from
			bool dither = false
			);

		~Bitmap_16bit();

		virtual StringId GetType();

		virtual void Save(Asset& asset) const;
		virtual void Load(const Asset& asset);

		virtual void WriteToAsset(Asset* asset) const;
		virtual void ReadFromAsset(const Asset* asset);

	};




#endif /* __Bitmap_16bit_H__ */
	