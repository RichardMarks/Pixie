/**
 * \class	ImageFormat
 * 
 * \ingroup	graphics
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 *
 */
 
#ifndef __ImageFormat_H__
#define __ImageFormat_H__

// Includes
#include "Array.h"

// Forward declares
class Asset;
class Filename;

// ImageFormat
class ImageFormat
	{
	public:
		/**
		 * Destructor
		 */
		virtual ~ImageFormat() { } 

		virtual int GetWidth() = 0;

		virtual int GetHeight() = 0;

		virtual int GetCelCount() = 0;

		virtual float GetCelDelay(int celIndex) = 0;

		virtual void CopyPixels(unsigned int* destination) = 0;

		static ImageFormat* CreateImageFormat(const Asset& asset);

	protected:
		static void RegisterImageFormat(bool (*TestFunction)(const Asset&),ImageFormat* (*CreateFunction)(const Asset&));

	private:
		struct ImageFormatEntry
			{
			bool (*TestFunction)(const Asset&);
			ImageFormat* (*CreateFunction)(const Asset&);
			};
		static Array<ImageFormatEntry> imageFormats_;

	};

#endif /* __ImageFormat_H__ */
