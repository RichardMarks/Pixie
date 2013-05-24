/**
 * \class	ImageFormat_GIF
 * 
 * \ingroup	graphics
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 *
 */
 
#ifndef __ImageFormat_GIF_H__
#define __ImageFormat_GIF_H__

// Includes
#include "ImageFormat.h"

// Forward declares
class GifLoader;


// ImageFormat_GIF
class ImageFormat_GIF:public ImageFormat
	{
	public:
		static void Register();
		static bool TestAsset(const Asset& asset);
		static ImageFormat* Create(const Asset& asset);

		/**
		 * Constructor
		 */
		ImageFormat_GIF(
			const Asset& asset
			);
		
		/**
		 * Destructor
		 */
		virtual ~ImageFormat_GIF();

		virtual int GetWidth();

		virtual int GetHeight();

		virtual int GetCelCount();

		virtual void CopyPixels(unsigned int* destination);

		virtual float GetCelDelay(int celIndex);

		static void Save(const Filename& filename, int width, int height, void* data);
	private:
		GifLoader* image_;
	};

#endif /* __ImageFormat_GIF_H__ */
