/**
 * \class	Bitmap
 * 
 * \ingroup	graphics
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 * A Bitmap is simply a collection of 16-bit color values with a width and a height.
 * It is typically used as blit target, and is also what you pass on to the Present
 * method of Platform_Screen to display stuff.
 * 
 * Bitmaps can be blitted onto other bitmaps, but this is quite slow as the code is
 * not yet optimized. It is recommended that you use just one bitmap (for your back
 * buffer) and make all your other art of the type Bitmap_RLE8, as this can be blitted
 * onto a Bitmap in the same way, but is much, much faster.
 *
 * \todo	Update Clip code to handle offset/pitch. Might be fixed, but needs cleaning up (transforms and such)
 * \todo	Additive blitting
 * \todo	Make TransformCoordinates non-inline
 */

#ifndef __Bitmap_H__
#define __Bitmap_H__

// Includes
#include "StringId.h"

// Forward declares
class Asset;

// Bitmap
class Bitmap
	{
	public:
		Bitmap();

		/** 
		 * The Bitmap class is an abstract interface, and is implemented by a number of different
		 * bitmap classes. The GetType method retrieves the name of the actual implementation for
		 * the Bitmap instance.
		 *
		 * \returns	A StringId containing the class name of this bitmaps implementation
		 */
		virtual StringId GetType() = 0;


		/**
		 * The Bitmap class has an empty, virtual destructor, so that you can delete a pointer
		 * of type Bitmap, and still have the destructor of it's subtype called properly.
		 */
		virtual ~Bitmap() { };


		/**
		 * Get the raw data pointer to the bitmap data. This should be used with care, as it can 
		 * be quite easy to read outside the valid memory area. There might be Bitmap implementations
		 * which doesn't have color data, so this method can return 0.
		 * 
		 * \returns	A pointer to the pixel data (each pixel is a 16 bit color value, on the form R5G6B5)
		 */
		virtual unsigned short* GetColorData() const;

		/**
		 * Get the raw data pointer to the bitmaps alpha channel data. This should be used with care, 
		 * as it can be quite easy to read outside the valid memory area. There might be Bitmap 
		 * implementations which doesn't have an alpha channel, so this method can return 0.
		 * 
		 * \returns	A pointer to the alpha channel data (each pixel is a 8 bit transparency value, 0=Fully transparent, 255=Fully Opaque
		 */
		virtual unsigned char* GetAlphaData() const;

		virtual int GetHPitch() const;
		
		virtual int GetVPitch() const;

		virtual int GetHOffset() const;
		
		virtual int GetVOffset() const;

		/**
		 * Sets every pixel of the bitmap to black (0). Internally this just uses a memset for
		 * speed. If you want to clear to any other color, you need to use the Fill method, which
		 * is slightly slower. If the bitmap has an alpha channel, it will be cleared too.
		 */
		virtual void Clear();


		/** 
		 * Fills the entire bitmap with the specified color. If an alpha value is specified, the 
		 * fill will be semi-transparent, and what was already in the  bitmap will show through. 
		 * If the bitmap has an alpha channel, it won't be affected by Fill.
		 */
		virtual void Fill(
			unsigned short color,	///< 16 bit color (R5G6B5) to fill the specified area with
			unsigned char alpha = 255 ///< Level of transparency for the fill. 0=Fully transparent, 255=Fully Opaque
			);


		/** 
		 * Fills the specified area of the bitmap with the specified color. If an alpha value is
		 * specified, the filled area will be semi-transparent, and what was already in the 
		 * bitmap will show through. Note that the filled area will include the x2 and y2 pixels
		 * as well (it won't leave the last pixel blank as for instance DirectX dies).
		 *
		 * If the specified area is outside of the bitmap, it will be clipped to the extents of
		 * the bitmap. If the bitmap has an alpha channel, it won't be affected by Fill.
		 */
		virtual void Fill(
			int x1,	///< X-coordinate of the top-left corner of the area to be filled
			int y1,	///< Y-coordinate of the top-left corner of the area to be filled 
			int x2, ///< X-coordinate of the bottom-right corner of the area to be filled
			int y2, ///< Y-coordinate of the bottom-right corner of the area to be filled
			unsigned short color,	///< 16 bit color (R5G6B5) to fill the specified area with
			unsigned char alpha = 255 ///< Level of transparency for the fill. 0=Fully transparent, 255=Fully Opaque
			);
							

		virtual void FillAlpha(
			unsigned char alpha ///< Level of transparency for the fill. 0=Fully transparent, 255=Fully Opaque
			);

		virtual void FillAlpha(
			int x1,	///< X-coordinate of the top-left corner of the area to be filled
			int y1,	///< Y-coordinate of the top-left corner of the area to be filled 
			int x2, ///< X-coordinate of the bottom-right corner of the area to be filled
			int y2, ///< Y-coordinate of the bottom-right corner of the area to be filled
			unsigned char alpha ///< Level of transparency for the fill. 0=Fully transparent, 255=Fully Opaque
			);
				
		/**
		 * Sometimes it is useful to rotate or flip bitmaps. The methods specified below
		 * all takes a Transformation parameter, which will cause the method to function as
		 * if the bitmap was transformed in the manner specified, without actually transforming
		 * any pixel data. This means you can blit an image upside down by just passing
		 * Mirror_Y as the transformation parameter to the blit method. All the transformation
		 * parameters default to untransformed, so just omit the parameter if you don't want to 
		 * think about transformations...
		 */
		enum Transformation
			{
			NoTransformation,	///< The bitmap is not transformed 
			Rotate_90,	///< The bitmap is rotated 90 degrees clockwise
			Rotate_180,	///< The bitmap is rotated 180 degrees
			Rotate_270,	///< The bitmap is rotated 270 degrees clockwise / 90 degrees counter clockwise
			Mirror_X,	///< The bitmap is flipped horizontally
			Mirror_Y,	///< The bitmap is flipped vertically
			Transformations_Count	///< Number of available transformations, handy if you want to pick a random one
			};


		/**
		 * Gets the width of the bitmap
		 *
		 * \returns	The width of the bitmap in pixels
		 */
		virtual int GetWidth(
			Transformation transformation = NoTransformation ///< Specifies the desired transformation to use for this operation
			) const;
		

		/**
		 * Gets the height of the bitmap
		 *
		 * \returns	The height of the bitmap in pixels
		 */
		virtual int GetHeight(
			Transformation transformation = NoTransformation	///< Specifies the desired transformation to use for this operation
			) const;


		/**
		 * Gets the 16 bit (R5G6B5) pixel at the specified coordinate of the bitmap
		 * This version of GetPixel will take an Transformation parameters, so you can get pixels
		 * as if the bitmap was transformed.
		 *
		 * \returns	The pixel at the specified coordinate
		 */
		virtual unsigned short  GetPixelColor(
			int x,	///< X-coordinate of pixel to get
			int y,	///< Y-coordinate of pixel to get
			Transformation transformation  = NoTransformation	///< Specifies the desired transformation to use for this operation
			) const;

		virtual unsigned char  GetPixelAlpha(
			int x,	///< X-coordinate of pixel to get
			int y,	///< Y-coordinate of pixel to get
			Transformation transformation = NoTransformation	///< Specifies the desired transformation to use for this operation
			) const;

		/**
		 * Changes the pixel at the specified coordinate of the bitmap.
		 * This version of SetPixel will take an Transformation parameters, so you can set pixels
		 * as if the bitmap was transformed.
		 */
		virtual void SetPixelColor(
			int x, 	///< X-coordinate of pixel to set
			int y,	///< Y-coordinate of pixel to set
			unsigned short color,	///< 16 bit color (R5G6B5) to set the specified coordinate to
			Transformation transformation = NoTransformation	///< Specifies the desired transformation to use for this operation
			);

		virtual void SetPixelAlpha(
			int x, 	///< X-coordinate of pixel to set
			int y,	///< Y-coordinate of pixel to set
			unsigned char alpha,	
			Transformation transformation = NoTransformation	///< Specifies the desired transformation to use for this operation
			);


		/** 
		 * Blends the pixel at the specified coordinate with the specified color, using the specified
		 * alpha value as a blend factor. 
		 * This version of BlenPixel will take an Transformation parameters, so you can blend pixels
		 * as if the bitmap was transformed.
		 */
		virtual void BlendPixel(
			int x,	///< X-coordinate of pixel to change
			int y,	///< Y-coordinate of pixel to change
			unsigned short color,	///< 16 bit color (R5G6B5) to blend the existing pixel with
			unsigned char alpha,	///< Blend factor. 0=Use the existing pixel color, 255=Use the specified color
			Transformation transformation	= NoTransformation	///< Specifies the desired transformation to use for this operation
			);

		/**
		 * This method will blit this bitmap onto the target bitmap at the specified coordinate. It will blit the entire 
		 * bitmap onto the target, but it will perform clipping on the parts which are outside of the target.
		 */
		virtual void Blit(
			Bitmap& target, ///< Target bitmap to blit this bitmap onto
			int x,	///< X-coordinate in the target bitmap. The top left corner of this bitmap will be drawn at this coordinate on the target bitmap
			int y,	///< Y-coordinate in the target bitmap. The top left corner of this bitmap will be drawn at this coordinate on the target bitmap
			unsigned short modulate = 0xffff,	///< 16 bit color value (R5G6B5) to modulate the source pixels with before they are written. 
			unsigned char alpha = 255,	///< Level of transparency the bitmap will be written with. 0=Fully transparent, 255=Fully Opaque
			Transformation transformation = NoTransformation,	///< Specifies the desired transformation to use for this operation
			bool maxBlit = false
			) const;

		/**
		 * This method will blit a section of this bitmap onto the target bitmap at the specified coordinate. You specify the top-left and 
		 * bottom-right coordinates of a rectangle you want to blit onto the target. Clipping will be performed on the parts which are outside 
		 * of the target.
		 */
		virtual void Blit(
			int x1, ///< X-coordinate for the top-left corner of the section of this bitmap which is to be blitted onto the target
			int y1,	///< Y-coordinate for the top-left corner of the section of this bitmap which is to be blitted onto the target
			int x2,	///< X-coordinate for the bottom-right corner of the section of this bitmap which is to be blitted onto the target 
			int y2,	///< Y-coordinate for the bottom-right corner of the section of this bitmap which is to be blitted onto the target
			Bitmap& target, ///< Target bitmap to blit this bitmap onto
			int x,	///< X-coordinate in the target bitmap. The top left corner of the blitted section will be drawn at this coordinate on the target bitmap
			int y,	///< Y-coordinate in the target bitmap. The top left corner of the blitted section will be drawn at this coordinate on the target bitmap
			unsigned short modulate = 0xffff,///< 16 bit color value (R5G6B5) to modulate the source pixels with before they are written. 
			unsigned char alpha = 255,	///< Level of transparency the bitmap will be written with. 0=Fully transparent, 255=Fully Opaque
			Transformation transformation = NoTransformation, ///< Specifies the desired transformation to use for this operation
			bool maxBlit = false
			) const;

		virtual void Copy(
			Bitmap& target, ///< Target bitmap to blit this bitmap onto
			int x,	///< X-coordinate in the target bitmap. The top left corner of this bitmap will be drawn at this coordinate on the target bitmap
			int y,	///< Y-coordinate in the target bitmap. The top left corner of this bitmap will be drawn at this coordinate on the target bitmap
			unsigned short modulate = 0xffff,///< 16 bit color value (R5G6B5) to modulate the source pixels with before they are written. 
			Transformation transformation = NoTransformation	///< Specifies the desired transformation to use for this operation
			) const;

		virtual void Copy(
			int x1, ///< X-coordinate for the top-left corner of the section of this bitmap which is to be blitted onto the target
			int y1,	///< Y-coordinate for the top-left corner of the section of this bitmap which is to be blitted onto the target
			int x2,	///< X-coordinate for the bottom-right corner of the section of this bitmap which is to be blitted onto the target 
			int y2,	///< Y-coordinate for the bottom-right corner of the section of this bitmap which is to be blitted onto the target
			Bitmap& target, ///< Target bitmap to blit this bitmap onto
			int x,	///< X-coordinate in the target bitmap. The top left corner of the blitted section will be drawn at this coordinate on the target bitmap
			int y,	///< Y-coordinate in the target bitmap. The top left corner of the blitted section will be drawn at this coordinate on the target bitmap
			unsigned short modulate = 0xffff,///< 16 bit color value (R5G6B5) to modulate the source pixels with before they are written. 
			Transformation transformation = NoTransformation ///< Specifies the desired transformation to use for this operation
			) const;

		virtual void Save(Asset& asset) const = 0;
		virtual void Load(const Asset& asset) = 0;

		virtual void WriteToAsset(Asset* asset) const = 0;
		virtual void ReadFromAsset(const Asset* asset) = 0;

	protected:
		/**
		 * Helper method used to transform a specified (x,y) coordinate to the specified transformation
		 */
		inline void TransformCoordinates(
			int& x, ///< X-coordinate to transform
			int& y, ///< Y-coordinate to transform
			Transformation transformation	///< Specifies the desired transformation to use for this operation
			) const
			{
			int xp=x;
			int yp=y;
			switch (transformation)
				{
				case NoTransformation:
					{
					return;
					} break;

				case Rotate_90:
					{
					xp=y;
					yp=(GetHeight()-1)-x;
					} break;

				case Rotate_180:
					{
					xp=(GetWidth()-1)-x;
					yp=(GetHeight()-1)-y;
					} break;

				case Rotate_270:
					{
					xp=(GetWidth()-1)-y;
					yp=x;
					} break;

				case Mirror_X:
					{
					xp=(GetWidth()-1)-x;
					} break;

				case Mirror_Y:
					{
					yp=(GetHeight()-1)-y;
					} break;

				}
			x=xp;
			y=yp;
			}


		bool Clip(int& sourceX, int& sourceY, int& sourceWidth, int& sourceHeight, int& targetX, int& targetY, int targetWidth, int targetHeight, Transformation transformation ) const
			{

			// First, make sure source is in valid range
			
			// Is left side negative?
			if (sourceX<0)
				{
				targetX+=-sourceX;	// Compensate destination position 
				sourceWidth-=-sourceX; // Compensate width
				sourceX=0; // Clamp left side
				}

			// Is top side negative?
			if (sourceY<0) 
				{
				targetY+=-sourceY;	// Compensate destination position 
				sourceHeight-=-sourceY; // Compensate height
				sourceY=0; // Clamp top side
				}

			// Is right side out of range?
			if (sourceX+sourceWidth>GetWidth(transformation)) 
				{
				sourceWidth-=(sourceX+sourceWidth)-GetWidth(transformation); // Truncate width
				}

			// Is bottom  side out of range?
			if (sourceY+sourceHeight>GetHeight(transformation)) 
				{
				sourceHeight-=(sourceY+sourceHeight)-GetHeight(transformation); // Truncate height
				}

			// Next, make sure target is in valid range

			// Is left side negative?
			if (targetX<0)
				{
				sourceX+=-targetX;	// Compensate source position 
				sourceWidth-=-targetX; // Compensate width
				targetX=0; // Clamp left side
				}

			// Is top side negative?
			if (targetY<0)
				{
				sourceY+=-targetY;	// Compensate source position 
				sourceHeight-=-targetY; // Compensate width
				targetY=0; // Clamp left side
				}

			// Is right side out of range?
			if (targetX+sourceWidth>targetWidth)
				{
				sourceWidth-=(targetX+sourceWidth)-targetWidth; // Truncate width
				}

			// Is bottom side out of range?
			if (targetY+sourceHeight>targetHeight) 
				{
				sourceHeight-=(targetY+sourceHeight)-targetHeight; // Truncate height
				}

			// Check if there's something left to draw
			if (sourceWidth<=0 || sourceHeight<=0)
				{
				return false;
				}
			
			return true;
			}

	protected:
		unsigned short* color_;	///< Pixel color data (hPitch*vPitch)
		unsigned char* alpha_;	///< Pixel alpha data (width*height)
		int width_;		///< Width of the bitmap
		int height_;	///< Height of the bitmap
		int hPitch_;	///< Horizontal pitch (dimension) of the bitmap
		int vPitch_;	///< Vertical pitch (dimension) of the bitmap
		int hOffset_;	///< Horizontal offset from bitmap origin to data
		int vOffset_;	///< Vertical offset from bitmap origin to data
	};




#endif /* __Bitmap16_H__ */
	