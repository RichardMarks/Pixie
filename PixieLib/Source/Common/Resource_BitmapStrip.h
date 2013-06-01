/**
 * \class	Resource_BitmapStrip
 * 
 * \ingroup	resources
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 *
 */
 
#ifndef __Resource_BitmapStrip_H__
#define __Resource_BitmapStrip_H__

// Includes
#include "StringId.h"
#include "Bitmap.h"
#include "Filename.h"

// Forward declares
class BitmapStrip;
class Image;


// Resource_BitmapStrip
class Resource_BitmapStrip
	{
	public:
		Resource_BitmapStrip();

		Resource_BitmapStrip(const BitmapStrip* bitmapStrip);
		
		Resource_BitmapStrip(const Bitmap* bitmap);

		Resource_BitmapStrip(const Filename& filename, int celCount = 1 );

		Resource_BitmapStrip(const char* filename, int celCount = 1 );

		Resource_BitmapStrip(StringId filename, int celCount = 1 );

		Resource_BitmapStrip(const Image& image);
		
		~Resource_BitmapStrip();

		Resource_BitmapStrip(const Resource_BitmapStrip& resource);

		const Resource_BitmapStrip& operator =(const Resource_BitmapStrip& resource);

		const BitmapStrip& GetBitmapStrip() const;

		const Bitmap& GetBitmap() const;

		operator const BitmapStrip&() const;

		operator const Bitmap&() const;

		int GetWidth(int cel = 0) const;
	
		int GetHeight(int cel = 0) const;

		int GetCelCount() const;

		const Bitmap& GetCel(int index) const;

		/**
		 * This method will blit this bitmap onto the target bitmap at the specified coordinate. It will blit the entire 
		 * bitmap onto the target, but it will perform clipping on the parts which are outside of the target.
		 */
		void Blit(
			Bitmap& target, ///< Target bitmap to blit this bitmap onto
			int x,	///< X-coordinate in the target bitmap. The top left corner of this bitmap will be drawn at this coordinate on the target bitmap
			int y,	///< Y-coordinate in the target bitmap. The top left corner of this bitmap will be drawn at this coordinate on the target bitmap
			unsigned short modulate = 0xffff,	///< 16 bit color value (R5G6B5) to modulate the source pixels with before they are written. 
			unsigned char alpha = 255,	///< Level of transparency the bitmap will be written with. 0=Fully transparent, 255=Fully Opaque
			Bitmap::Transformation transformation = Bitmap::NoTransformation	///< Specifies the desired transformation to use for this operation
			) const;

		/**
		 * This method will blit a section of this bitmap onto the target bitmap at the specified coordinate. You specify the top-left and 
		 * bottom-right coordinates of a rectangle you want to blit onto the target. Clipping will be performed on the parts which are outside 
		 * of the target.
		 */
		void Blit(
			int x1, ///< X-coordinate for the top-left corner of the section of this bitmap which is to be blitted onto the target
			int y1,	///< Y-coordinate for the top-left corner of the section of this bitmap which is to be blitted onto the target
			int x2,	///< X-coordinate for the bottom-right corner of the section of this bitmap which is to be blitted onto the target 
			int y2,	///< Y-coordinate for the bottom-right corner of the section of this bitmap which is to be blitted onto the target
			Bitmap& target, ///< Target bitmap to blit this bitmap onto
			int x,	///< X-coordinate in the target bitmap. The top left corner of the blitted section will be drawn at this coordinate on the target bitmap
			int y,	///< Y-coordinate in the target bitmap. The top left corner of the blitted section will be drawn at this coordinate on the target bitmap
			unsigned short modulate = 0xffff,///< 16 bit color value (R5G6B5) to modulate the source pixels with before they are written. 
			unsigned char alpha = 255,	///< Level of transparency the bitmap will be written with. 0=Fully transparent, 255=Fully Opaque
			Bitmap::Transformation transformation = Bitmap::NoTransformation ///< Specifies the desired transformation to use for this operation
			) const;

		void Copy(
			Bitmap& target, ///< Target bitmap to blit this bitmap onto
			int x,	///< X-coordinate in the target bitmap. The top left corner of this bitmap will be drawn at this coordinate on the target bitmap
			int y,	///< Y-coordinate in the target bitmap. The top left corner of this bitmap will be drawn at this coordinate on the target bitmap
			unsigned short modulate = 0xffff,///< 16 bit color value (R5G6B5) to modulate the source pixels with before they are written. 
			Bitmap::Transformation transformation = Bitmap::NoTransformation	///< Specifies the desired transformation to use for this operation
			) const;

		void Copy(
			int x1, ///< X-coordinate for the top-left corner of the section of this bitmap which is to be blitted onto the target
			int y1,	///< Y-coordinate for the top-left corner of the section of this bitmap which is to be blitted onto the target
			int x2,	///< X-coordinate for the bottom-right corner of the section of this bitmap which is to be blitted onto the target 
			int y2,	///< Y-coordinate for the bottom-right corner of the section of this bitmap which is to be blitted onto the target
			Bitmap& target, ///< Target bitmap to blit this bitmap onto
			int x,	///< X-coordinate in the target bitmap. The top left corner of the blitted section will be drawn at this coordinate on the target bitmap
			int y,	///< Y-coordinate in the target bitmap. The top left corner of the blitted section will be drawn at this coordinate on the target bitmap
			unsigned short modulate = 0xffff,///< 16 bit color value (R5G6B5) to modulate the source pixels with before they are written. 
			Bitmap::Transformation transformation = Bitmap::NoTransformation ///< Specifies the desired transformation to use for this operation
			) const;


		/**
		 * This method will blit this bitmap onto the target bitmap at the specified coordinate. It will blit the entire 
		 * bitmap onto the target, but it will perform clipping on the parts which are outside of the target.
		 */
		void Blit(
			int cel,
			Bitmap& target, ///< Target bitmap to blit this bitmap onto
			int x,	///< X-coordinate in the target bitmap. The top left corner of this bitmap will be drawn at this coordinate on the target bitmap
			int y,	///< Y-coordinate in the target bitmap. The top left corner of this bitmap will be drawn at this coordinate on the target bitmap
			unsigned short modulate = 0xffff,	///< 16 bit color value (R5G6B5) to modulate the source pixels with before they are written. 
			unsigned char alpha = 255,	///< Level of transparency the bitmap will be written with. 0=Fully transparent, 255=Fully Opaque
			Bitmap::Transformation transformation = Bitmap::NoTransformation	///< Specifies the desired transformation to use for this operation
			) const;

		/**
		 * This method will blit a section of this bitmap onto the target bitmap at the specified coordinate. You specify the top-left and 
		 * bottom-right coordinates of a rectangle you want to blit onto the target. Clipping will be performed on the parts which are outside 
		 * of the target.
		 */
		void Blit(
			int cel,
			int x1, ///< X-coordinate for the top-left corner of the section of this bitmap which is to be blitted onto the target
			int y1,	///< Y-coordinate for the top-left corner of the section of this bitmap which is to be blitted onto the target
			int x2,	///< X-coordinate for the bottom-right corner of the section of this bitmap which is to be blitted onto the target 
			int y2,	///< Y-coordinate for the bottom-right corner of the section of this bitmap which is to be blitted onto the target
			Bitmap& target, ///< Target bitmap to blit this bitmap onto
			int x,	///< X-coordinate in the target bitmap. The top left corner of the blitted section will be drawn at this coordinate on the target bitmap
			int y,	///< Y-coordinate in the target bitmap. The top left corner of the blitted section will be drawn at this coordinate on the target bitmap
			unsigned short modulate = 0xffff,///< 16 bit color value (R5G6B5) to modulate the source pixels with before they are written. 
			unsigned char alpha = 255,	///< Level of transparency the bitmap will be written with. 0=Fully transparent, 255=Fully Opaque
			Bitmap::Transformation transformation = Bitmap::NoTransformation ///< Specifies the desired transformation to use for this operation
			) const;

		void Copy(
			int cel,
			Bitmap& target, ///< Target bitmap to blit this bitmap onto
			int x,	///< X-coordinate in the target bitmap. The top left corner of this bitmap will be drawn at this coordinate on the target bitmap
			int y,	///< Y-coordinate in the target bitmap. The top left corner of this bitmap will be drawn at this coordinate on the target bitmap
			unsigned short modulate = 0xffff,///< 16 bit color value (R5G6B5) to modulate the source pixels with before they are written. 
			Bitmap::Transformation transformation = Bitmap::NoTransformation	///< Specifies the desired transformation to use for this operation
			) const;

		void Copy(
			int cel,
			int x1, ///< X-coordinate for the top-left corner of the section of this bitmap which is to be blitted onto the target
			int y1,	///< Y-coordinate for the top-left corner of the section of this bitmap which is to be blitted onto the target
			int x2,	///< X-coordinate for the bottom-right corner of the section of this bitmap which is to be blitted onto the target 
			int y2,	///< Y-coordinate for the bottom-right corner of the section of this bitmap which is to be blitted onto the target
			Bitmap& target, ///< Target bitmap to blit this bitmap onto
			int x,	///< X-coordinate in the target bitmap. The top left corner of the blitted section will be drawn at this coordinate on the target bitmap
			int y,	///< Y-coordinate in the target bitmap. The top left corner of the blitted section will be drawn at this coordinate on the target bitmap
			unsigned short modulate = 0xffff,///< 16 bit color value (R5G6B5) to modulate the source pixels with before they are written. 
			Bitmap::Transformation transformation = Bitmap::NoTransformation ///< Specifies the desired transformation to use for this operation
			) const;

	private:
		const BitmapStrip* bitmapStrip_;
	};

#endif /* __Resource_BitmapStrip_H__ */

