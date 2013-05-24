/**
 * \class	Bitmap_16bitAlphaCrop
 * 
 * \ingroup	graphics
 * \brief	Uncompressed 16-bit color buffer with alpha channel
 * \author	Mattias Gustavsson	
 * 
 */

#ifndef __Bitmap_16bitAlphaCrop_H__
#define __Bitmap_16bitAlphaCrop_H__

// Includes
#include "Bitmap.h"


// Forward declares
class Image;

// Bitmap_16bitAlphaCrop
class Bitmap_16bitAlphaCrop:public Bitmap
	{
	public:
		Bitmap_16bitAlphaCrop();

		Bitmap_16bitAlphaCrop(const Asset& asset);

		Bitmap_16bitAlphaCrop(const Image& image, bool dither = false);

		Bitmap_16bitAlphaCrop(const Image& color, const Image& alpha, bool dither = false);

		~Bitmap_16bitAlphaCrop();

		virtual StringId GetType();

		virtual void Save(Asset& asset) const;
		virtual void Load(const Asset& asset);

		virtual void WriteToAsset(Asset* asset) const;
		virtual void ReadFromAsset(const Asset* asset);
		void ReadFromAsset_Old(const Asset* asset);
	};

#endif /* __Bitmap_16bitAlphaCrop_H__ */
	