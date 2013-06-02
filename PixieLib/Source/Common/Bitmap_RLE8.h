/**
 * \class	Bitmap_RLE8
 * 
 * \ingroup	graphics
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 * \todo	Refactor and clean up
 * \todo	Add methods to conform to Bitmap interface (Copy, transformations etc)
 */


#ifndef __Bitmap_RLE8_H__
#define __Bitmap_RLE8_H__

// Includes
#include "Bitmap.h"

// Forward declares
class Image;
class Bitmap_16bit;
class Bitmap_16bitAlpha;
class Asset;

// Bitmap_RLE8
class Bitmap_RLE8:public Bitmap
	{
	public:
		Bitmap_RLE8();
		Bitmap_RLE8(const Asset& asset);
		Bitmap_RLE8(const Image& image, bool dither = false);
		~Bitmap_RLE8();

		virtual StringId GetType();

		virtual unsigned short* GetColorData();

		virtual unsigned char* GetAlphaData();

		virtual void Clear();
		
		virtual void Fill(int x1, int y1, int x2, int y2, unsigned short color,	unsigned char alpha = 255 );							
		virtual void Fill(unsigned short color, unsigned char alpha = 255);

        virtual int GetHPitch() const;
        virtual int GetVPitch() const;
        virtual int GetHOffset() const;
        virtual int GetVOffset() const;

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

		int GetX1() const;
		int GetY1() const;
		int GetX2() const;
		int GetY2() const;

		unsigned int GetOpaqueSize() const;
		unsigned char* GetOpaqueData() const;	

	private:
		void BlitRLE(Bitmap_16bit* target,int x, int y, unsigned short modulate=0xffff, unsigned char alpha = 255) const;
		void BlitRLE(int x1, int y1, int x2, int y2, Bitmap_16bit* target,int x, int y, unsigned short modulate=0xffff, unsigned char alpha = 255) const;
		void MaxBlitRLE(int x1, int y1, int x2, int y2, Bitmap_16bit* target,int x, int y, unsigned short modulate=0xffff, unsigned char alpha = 255) const;

		void BlitRLE(Bitmap_16bitAlpha* target,int x, int y, unsigned short modulate) const;
		void BlitRLE(int x1, int y1, int x2, int y2, Bitmap_16bitAlpha* target,int x, int y, unsigned short modulate) const;

		unsigned char RLEGetPixelAlpha(int x, int y) const;
		unsigned short RLEGetPixelColor(int x, int y) const;



	private:
		friend class GenerateRLE8;
		friend class GenerateRLE8old;
		unsigned char usesMask_;
		unsigned short xOffset_;
		unsigned short yOffset_;
		unsigned short activeWidth_;	// Width of sprite    
		unsigned short activeHeight_;	// Height of sprite
		unsigned short width_;	// Width of sprite    
		unsigned short height_;	// Height of sprite
		unsigned int opaqueSize_;
		unsigned char* opaqueData_;	
		unsigned int alphaSize_;
		unsigned char* alphaData_;	
		unsigned char colorCount_;	// Number of colors in palette
		unsigned short* palette_; // Maximum of 256 colors
		mutable unsigned short* modulatedPalette_; // palette used when doing modulated rendering
		mutable unsigned short* currentPalette_; // palette currently in use
	};
	
	 
#endif /* __Bitmap_RLE8_H__ */
