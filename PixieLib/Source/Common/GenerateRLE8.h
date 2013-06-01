/**
 * \class	GenerateRLE8
 * 
 * \ingroup	graphics
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 */


#ifndef __GenerateRLE8_H__
#define __GenerateRLE8_H__

// Includes

// Forward declares
class Image;
class Bitmap_RLE8;

// GenerateRLE8
class GenerateRLE8
	{
	public:
		GenerateRLE8(const Image& image, Bitmap_RLE8* bitmap, bool dither);

	private:
		void CreateFromImage(const Image* image, bool dither);
		int GetRLESize(unsigned char* data,unsigned char* mask);
		int	GetRunLength(unsigned char* data, unsigned char* mask,int size);
		int	GetUniquesLength(unsigned char* data, unsigned char* mask,int size);
		int CreateRLE(unsigned char* source, unsigned char* mask, unsigned char* destination);
		int GetRLESize_Alpha(unsigned char* data,unsigned char* mask);
		int	GetRunLength_Alpha(unsigned char* data, unsigned char* mask,int size);
		int	GetUniquesLength_Alpha(unsigned char* data, unsigned char* mask,int size);
		int CreateRLE_Alpha(unsigned char* source, unsigned char* mask, unsigned char* destination);
		void Palettize(const Image* image,unsigned char** data, unsigned char** mask, unsigned short** palette, unsigned char* colorCount, int* alphaCount, bool dither);
		void Crop(const Image* image, int* minX, int* minY, int* maxX, int* maxY);

	private:
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
	};
	
	 
#endif /* __GenerateRLE8_H__ */
