/**
 * \class	Image
 * 
 * \ingroup	graphics
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 * \todo	More transformations/manipulation (contrast, saturation, hue, blur, sharpen, resize...)
 * \todo	Get rid of orientation stuff - it goes in the bitmap instead
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
		enum Orientation
			{
			Rotate_0,
			Rotate_90,
			Rotate_180,
			Rotate_270,
			Mirror_X,
			Mirror_Y,
			Orientations_Count
			};

		Image();
		Image(const Asset& asset, Orientation orientation = Rotate_0);
		Image(const Asset& asset, int celCount, Orientation orientation = Rotate_0);
		Image(int width, int height);
		Image(int width, int height, int celCount);
		Image(const Image& image);
		Image(const Bitmap& bitmap);
		
		~Image();

		const Image& operator=(const Image& image);
		

		void SetOrientation(Orientation orientation);

		int GetWidth() const;

		int GetHeight() const;


		unsigned int GetPixel(int x, int y) const;

		unsigned int GetPixel(int cel, int x, int y) const;
		
		void SetPixel(int x, int y, unsigned int color);

		void SetPixel(int cel, int x, int y, unsigned int color);

		int GetCelCount() const;

		float GetCelDelay(int cel) const;

		void AdjustBrightness(float redMultiplier, float greenMultiplier, float blueMultiplier);

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
		Orientation orientation_;
	};


#endif /* __Image_H__ */

