/**
 * \class	Resource_Font
 * 
 * \ingroup	resources
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 *
 */
 
#ifndef __Resource_Font_H__
#define __Resource_Font_H__

// Includes
#include "StringId.h"
#include "Filename.h"

// Forward declares
class Font;
class Filename;
class Bitmap;

// Resource_Font
class Resource_Font
	{
	public:
		Resource_Font();

		Resource_Font(const Filename& filename);

		Resource_Font(const char* filename);

		Resource_Font(StringId filename);

		~Resource_Font();

		Resource_Font(const Resource_Font& resource);

		const Resource_Font& operator =(const Resource_Font& resource);

		const Font& GetFont() const;

		operator const Font&() const;

		void Blit(
			Bitmap& bitmap, 
			int x, 
			int y, 
			const char* text, 
			int spacing = 0, 
			unsigned short modulate = 0xffff,
			unsigned char alpha = 255
			) const;

		void BlitWrap(
			Bitmap& bitmap, 
			int x, 
			int y, 
			const char* text, 
			int width,
			int hspacing = 0, 
			int vspacing = 0, 
			unsigned short modulate = 0xffff,
			unsigned char alpha = 255
			) const;

	private:
		const Font* font_;
	};

#endif /* __Resource_Font_H__ */

