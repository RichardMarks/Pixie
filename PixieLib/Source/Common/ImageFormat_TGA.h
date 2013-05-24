/**
 * \class	ImageFormat_TGA
 * 
 * \ingroup	graphics
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 *
 */
 
#ifndef __ImageFormat_TGA_H__
#define __ImageFormat_TGA_H__

// Includes
#include "ImageFormat.h"

// Forward declares


// ImageFormat_TGA
class ImageFormat_TGA:public ImageFormat
	{
	public:
		static void Register();
		static bool TestAsset(const Asset& asset);
		static ImageFormat* Create(const Asset& asset);

		/**
		 * Constructor
		 */
		ImageFormat_TGA(
			const Asset& asset
			);
		
		ImageFormat_TGA(
			int width,
			int height,
			void* data
			);

		/**
		 * Destructor
		 */
		virtual ~ImageFormat_TGA();

		virtual int GetWidth();

		virtual int GetHeight();

		virtual int GetCelCount();

		virtual void CopyPixels(unsigned int* destination);

		virtual float GetCelDelay(int celIndex);

		static void Save(const Filename& filename, int width, int height, void* data);
	private:
		void* image_;
	};

#endif /* __ImageFormat_TGA_H__ */
