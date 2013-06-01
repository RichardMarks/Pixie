/**
 * \class	BitmapStrip
 * 
 * \ingroup	graphics
 * \brief	Collection of Bitmaps (used for animation)
 * \author	Mattias Gustavsson	
 * 
 * A bitmap strip is a collection of individual Bitmaps which are meant to be used together to form an
 * animation sequence. A bitmap strip can either be loaded directly from a binary asset as created with
 * the PixiePCT utility, or created from an image containing one or more cels.
 *
 */
 
#ifndef __BitmapStrip_H__
#define __BitmapStrip_H__

// Includes
#include "Array.h"

// Forward declares
class Bitmap;
class Image;
class Asset;


// BitmapStrip
class BitmapStrip
	{
	public:
		BitmapStrip();

		/**
		 * Creates a bitmap strip from a binary .rle file created by the Palettizer tool.
		 * This is a fast way of creating a bitmap strip, as the data in an .rle file is
		 * organized so that it can mostly just be read straight into memory with very
		 * little additional operations.
		 */
		BitmapStrip(
			const Asset& asset	///< Asset to create the bitmap strip from.
			);
		
		/**
		 * Creates a bitmap strip from an image with one or more cels.
		 * This is a slow way of creating a bitmap strip, as the data is copied several
		 * times and the image might also need to be palettized before becoming an RLE,
		 * but it can still be useful to use this method during development. Before you
		 * ship your game, you should definitely change everything over to use RLEs though,
		 * as this will practically eliminate loading times.
		 */
		BitmapStrip(
			const Image& image	///< Image to create the bitmap strip from
			);

		BitmapStrip(
			const Bitmap* bitmap
			);

		/**
		 * Destructor
		 */
		~BitmapStrip();

		/**
		 * Get the number of cels in this bitmap strip
		 *
		 * \return	Number of cels in the strip
		 */
		int GetCelCount() const;

		/**
		 * Get the cel at the specified index of of the bitmap strip
		 *
		 * \returns The specified cel 
		 */
		const Bitmap& GetCel(
			int index	///< Index of cel to get, in the range 0 to GetCelCount()-1
			) const;


		operator const Bitmap&() const;

		void ReadFromAsset(const Asset* asset);

	private:
		Array<const Bitmap*> cels_;	///< Collection of Bitmaps making up the strip
	};

#endif /* __BitmapStrip_H__ */

