/**
 * \class	ImageFormat_PNG
 * 
 * \ingroup	graphics
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 *
 */
 
#ifndef __ImageFormat_PNG_H__
#define __ImageFormat_PNG_H__

// Includes
#include "ImageFormat.h"

// Forward declares


// ImageFormat_PNG
class ImageFormat_PNG:public ImageFormat
	{
	public:
		static void Register();
		static bool TestAsset(const Asset& asset);
		static ImageFormat* Create(const Asset& asset);

		/**
		 * Constructor
		 */
		ImageFormat_PNG(
			const Asset& asset
			);
		
		ImageFormat_PNG(
			int width,
			int height,
			void* data
			);

		/**
		 * Destructor
		 */
		virtual ~ImageFormat_PNG();

		virtual int GetWidth();

		virtual int GetHeight();

		virtual int GetCelCount();

		virtual void CopyPixels(unsigned int* destination);

		virtual float GetCelDelay(int celIndex);

		static void Save(const Filename& filename, int width, int height, void* data);
	private:
		void* png_ptr_;
		void* info_ptr_;
	};

#endif /* __ImageFormat_PNG_H__ */
