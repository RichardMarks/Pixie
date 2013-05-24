/**
 * \class	ImageFormat_JPG
 * 
 * \ingroup	graphics
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 *
 */
 
#ifndef __ImageFormat_JPG_H__
#define __ImageFormat_JPG_H__

// Includes
#include "ImageFormat.h"

// Forward declares


// ImageFormat_JPG
class ImageFormat_JPG:public ImageFormat
	{
	public:
		static void Register();
		static bool TestAsset(const Asset& asset);
		static ImageFormat* Create(const Asset& asset);

		/**
		 * Constructor
		 */
		ImageFormat_JPG(
			const Asset& asset
			);
		
		ImageFormat_JPG(
			int width,
			int height,
			void* data
			);

		/**
		 * Destructor
		 */
		virtual ~ImageFormat_JPG();

		virtual int GetWidth();

		virtual int GetHeight();

		virtual int GetCelCount();

		virtual void CopyPixels(unsigned int* destination);

		virtual float GetCelDelay(int celIndex);

		static void Save(const Filename& filename, int width, int height, void* data);
	private:
		unsigned int* image_;
		int width_;
		int height_;
	};

#endif /* __ImageFormat_JPG_H__ */
