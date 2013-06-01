/**
 * \class	Image
 * 
 * \ingroup	graphics
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 * \todo	More transformations/manipulation (contrast, saturation, hue, blur, sharpen, resize...)
 */

#ifndef __Image_H__
#define __Image_H__

// Includes
#include "Array.h"

// Forward declares
class Asset;
class Filename;
class Bitmap;

// Image
class Image
	{
	public: 
		Image();
		Image(const Asset& asset);
		Image(const Asset& asset, int celCount);
		Image(int width, int height);
		Image(int width, int height, int celCount);
		Image(const Image& image);
		Image(const Bitmap& bitmap);
		
		~Image();

		const Image& operator=(const Image& image);
		

		int GetWidth() const;

		int GetHeight() const;


		unsigned int GetPixel(int x, int y) const;

		unsigned int GetPixel(int cel, int x, int y) const;
		
		void SetPixel(int x, int y, unsigned int color);

		void SetPixel(int cel, int x, int y, unsigned int color);

		int GetCelCount() const;

		float GetCelDelay(int cel) const;

		void BilinearSample( int c, float x, float y, float& r, float& g, float& b, float& a ) const;

		void Brightness(float value);
		void Contrast(float value);
		void Saturation(float value);
		void Hue(float value);
		void Sharpen( float radius = 0.1f );

		void Bleed( int iterations );

		unsigned int* GetPointer() const;

		template <class ImageFormat> void Save(const Filename& filename)
			{
			ImageFormat::Save(filename, width_,height_,data_);
			}
	private:
		int width_;
		int height_;
		unsigned int* data_;
		int celCount_;
		Array<float> celDelay_;
	};


#endif /* __Image_H__ */

