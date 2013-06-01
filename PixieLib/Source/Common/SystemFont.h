/**
 * \file	SystemFont.h
 * 
 * \brief	Basic font rendering using default fonts
 * \author	Mattias Gustavsson	
 * 
 * It is sometimes useful to be able to display text on the screen for debugging purposes. This 
 * class provides very basic functionality for drawing text, and is completely self-contained - it 
 * even includes the font data. It can be used to draw onto any type of bitmap, and handles clipping 
 * of any part of the text that would end up outside of the bitmap.
 */

#ifndef __SystemFont_H__
#define __SystemFont_H__

/**
 * Render text onto a 8-bit bitmap buffer using 9x16 font
 */
void SystemFont9x16(
	unsigned char* colorData,	///< Pointer to the pixel data of the target bitmap 
	int hPitch,	///< Width of the bitmap, in pixels
	int vPitch,	///< Height of the bitmap, in pixels
	int x,	///< X-position of text
	int y,	///< Y-position of text
	const char* text,	///< Zero-terminated string to render
	unsigned char color = 0xff	///< Color to use for rendering font
	);

/**
 * Render text onto a 16-bit bitmap buffer using 9x16 font
 */
void SystemFont9x16(
	unsigned short* colorData,	///< Pointer to the pixel data of the target bitmap 
	int hPitch,	///< Width of the bitmap, in pixels
	int vPitch,	///< Height of the bitmap, in pixels
	int x,	///< X-position of text
	int y,	///< Y-position of text
	const char* text,	///< Zero-terminated string to render
	unsigned short color = 0xffff	///< Color to use for rendering font
	);

/**
 * Render text onto a 32-bit bitmap buffer using 9x16 font
 */
void SystemFont9x16(
	unsigned int* colorData,	///< Pointer to the pixel data of the target bitmap 
	int hPitch,	///< Width of the bitmap, in pixels
	int vPitch,	///< Height of the bitmap, in pixels
	int x,	///< X-position of text
	int y,	///< Y-position of text
	const char* text,	///< Zero-terminated string to render
	unsigned int color = 0xffffffff	///< Color to use for rendering font
	);

/**
 * Render text onto a 8-bit bitmap buffer using 8x8 font
 */
void SystemFont8x8(
	unsigned char* colorData,	///< Pointer to the pixel data of the target bitmap 
	int hPitch,	///< Width of the bitmap, in pixels
	int vPitch,	///< Height of the bitmap, in pixels
	int x,	///< X-position of text
	int y,	///< Y-position of text
	const char* text,	///< Zero-terminated string to render
	unsigned char color = 0xff	///< Color to use for rendering font
	);

/**
 * Render text onto a 16-bit bitmap buffer using 8x8 font
 */
void SystemFont8x8(
	unsigned short* colorData,	///< Pointer to the pixel data of the target bitmap 
	int hPitch,	///< Width of the bitmap, in pixels
	int vPitch,	///< Height of the bitmap, in pixels
	int x,	///< X-position of text
	int y,	///< Y-position of text
	const char* text,	///< Zero-terminated string to render
	unsigned short color = 0xffff	///< Color to use for rendering font
	);

/**
 * Render text onto a 32-bit bitmap buffer using 8x8 font
 */
void SystemFont8x8(
	unsigned int* colorData,	///< Pointer to the pixel data of the target bitmap 
	int hPitch,	///< Width of the bitmap, in pixels
	int vPitch,	///< Height of the bitmap, in pixels
	int x,	///< X-position of text
	int y,	///< Y-position of text
	const char* text,	///< Zero-terminated string to render
	unsigned int color = 0xffffffff	///< Color to use for rendering font
	);


#endif /* __SystemFont_H__ */
