/**
 * \class	Bitmap_Alpha
 * 
 * \ingroup	graphics
 * \brief	Uncompressed 8-bit alpha channel
 * \author	Mattias Gustavsson	
 * 
 * A Bitmap_Alpha is simply a collection of 8-bit alpha values with a width and a 
 * height.
 * 
 */

#ifndef __Bitmap_Alpha_H__
#define __Bitmap_Alpha_H__

// Includes
#include "Bitmap.h"

// Forward declares
class Image;

// Bitmap
class Bitmap_Alpha:public Bitmap
	{
	public:

		Bitmap_Alpha();

		Bitmap_Alpha(
			const Asset& asset
			);

		Bitmap_Alpha(
			int width,
			int height
			);

		Bitmap_Alpha(
			const Image& image,	///< Image to create bitmap from
			bool useGrayscaleInsteadOfAlpha = false
			);

		~Bitmap_Alpha();

		virtual StringId GetType();

		virtual void Save(Asset& asset) const;
		virtual void Load(const Asset& asset);

		virtual void WriteToAsset(Asset* asset) const;
		virtual void ReadFromAsset(const Asset* asset);

	};




#endif /* __Bitmap_Alpha_H__ */
	