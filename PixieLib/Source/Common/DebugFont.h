/**
 * \class	DebugFont
 * 
 * \ingroup	core
 * \brief	Debug font rendering
 * \author	Mattias Gustavsson	
 * 
 * It is sometimes useful to be able to display text on the screen for debugging purposes. This class provides very
 * basic functionality for writing text, and is completely self-contained - it even includes the font data. It can be
 * used to draw onto any type of bitmap, and handles clipping of any part of the text that would end up outside of the 
 * bitmap.
 */

#ifndef __DebugFont_H__
#define __DebugFont_H__

// Includes

// Forward declares
class Bitmap;

// DebugFont
class DebugFont
	{
	public:
		/**
		 * Render text onto a bitmap
		 */
		void Blit(
			Bitmap& bitmap,	///< Bitmap to render onto
			int x,	///< X-position of text
			int y,	///< Y-position of text
			const char* text,	///< Zero-terminated string to render
			unsigned short color = 0xffff	///< 16-bit color to use for drawing the text. Defaults to white
			);

		/**
		 * Render text onto a 8-bit bitmap buffer
		 */
		void Blit(
			unsigned char* colorData,	///< Pointer to the pixel data of the target bitmap 
			int hPitch,	///< Width of the bitmap, in pixels
			int vPitch,	///< Height of the bitmap, in pixels
			int x,	///< X-position of text
			int y,	///< Y-position of text
			const char* text,	///< Zero-terminated string to render
			unsigned char color = 0xff	///< Color to use for rendering font
			);

		/**
		 * Render text onto a 16-bit bitmap buffer
		 */
		void Blit(
			unsigned short* colorData,	///< Pointer to the pixel data of the target bitmap 
			int hPitch,	///< Width of the bitmap, in pixels
			int vPitch,	///< Height of the bitmap, in pixels
			int x,	///< X-position of text
			int y,	///< Y-position of text
			const char* text,	///< Zero-terminated string to render
			unsigned short color = 0xffff	///< Color to use for rendering font
			);

		/**
		 * Render text onto a 32-bit bitmap buffer
		 */
		void Blit(
			unsigned int* colorData,	///< Pointer to the pixel data of the target bitmap 
			int hPitch,	///< Width of the bitmap, in pixels
			int vPitch,	///< Height of the bitmap, in pixels
			int x,	///< X-position of text
			int y,	///< Y-position of text
			const char* text,	///< Zero-terminated string to render
			unsigned int color = 0xffffffff	///< Color to use for rendering font
			);

	private:
		/**
		 * All the different blit methods have the same implementation, with only the type differing, so it makes sense 
		 * to use a template method to avoid duplicating the code. This method will draw the specified text by writing 
		 * the color value to each non-blank pixel as specified by the font-data.
		 */
		template <typename TYPE> void Blit(
			TYPE* colorData,	///< Pointer to the pixel data of the target bitmap 
			int hPitch,	///< Width of the bitmap, in pixels
			int vPitch,	///< Height of the bitmap, in pixels
			int x,	///< X-position of text
			int y,	///< Y-position of text
			const char* text,	///< Zero-terminated string to render
			TYPE color	///< Color to use for rendering font
			);
	};


#endif /* __DebugFont_H__ */
