/**
 * \class	Bitmap_RLE16
 * 
 * \ingroup	graphics
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 * \todo	Clipping 
 * \todo	Optimize for masked/unmasked/clipped/unclipped
 * \todo	Add methods to conform to Bitmap interface (Copy, transformations etc)
 * \todo	Move blit implementations to BlitterRLE16/CopperRLE16
 * \todo	Rendering bug when hPitch!=width. Rewrite the renderer to make it work properly. Row-by-row storage?
 */

#ifndef __Bitmap_RLE16_H__
#define __Bitmap_RLE16_H__

// Includes
#include "Bitmap.h"

// Forward declares
class Image;


// Bitmap_RLE16
class Bitmap_RLE16:public Bitmap
	{
	public:
		Bitmap_RLE16();

		Bitmap_RLE16(const Asset& asset);

		Bitmap_RLE16(
			const Image& image,	///< Image to create bitmap from
			bool dither = false
			);

		~Bitmap_RLE16();

		virtual StringId GetType();

		virtual unsigned short* GetColorData();

		virtual unsigned char* GetAlphaData();

		virtual void Clear();
		
		virtual void Fill(int x1, int y1, int x2, int y2, unsigned short color,	unsigned char alpha = 255 );							
		virtual void Fill(unsigned short color, unsigned char alpha = 255);

		virtual int GetWidth(Transformation transformation = NoTransformation) const;		
		virtual int GetHeight(Transformation transformation = NoTransformation) const;

		virtual unsigned short  GetPixelColor(int x, int y, Transformation transformation = NoTransformation) const;
		virtual unsigned char GetPixelAlpha(int x, int y, Transformation transformation = NoTransformation) const;

		virtual void SetPixelColor(int x, int y, unsigned short color, Transformation transformation = NoTransformation);
		virtual void SetPixelAlpha(int x, int y, unsigned char alpha, Transformation transformation = NoTransformation);

		virtual void BlendPixel(int x, int y, unsigned short color, unsigned char alpha, Transformation transformation = NoTransformation);

		virtual void Blit(Bitmap& target, int x, int y, unsigned short modulate = 0xffff, unsigned char alpha = 255, Transformation transformation = NoTransformation, bool maxBlit = false ) const;
		virtual void Blit(int x1, int y1, int x2, int y2, Bitmap& target, int x, int y, unsigned short modulate = 0xffff, unsigned char alpha = 255, Transformation transformation = NoTransformation, bool maxBlit = false ) const;

		virtual void Copy(Bitmap& target, int x, int y, unsigned short modulate = 0xffff, Transformation transformation = NoTransformation) const;
		virtual void Copy(int x1, int y1, int x2, int y2, Bitmap& target, int x, int y, unsigned short modulate = 0xffff, Transformation transformation = NoTransformation) const;

		virtual void Save(Asset& asset) const;
		virtual void Load(const Asset& asset);

		virtual void WriteToAsset(Asset* asset) const;
		virtual void ReadFromAsset(const Asset* asset);

	private:

		unsigned char RLEGetPixelAlpha(int x, int y) const;
		void BlitRLE(Bitmap& target, int xp, int yp) const;

	private:
		friend class GenerateRLE16;
		unsigned int opaqueSize_;
		unsigned char* opaqueData_;	
		unsigned int alphaSize_;
		unsigned char* alphaData_;	
};




#endif /* __Bitmap_RLE16_H__ */
	