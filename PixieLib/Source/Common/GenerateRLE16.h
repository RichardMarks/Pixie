/**
 * \class	GenerateRLE16
 * 
 * \ingroup	graphics
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 */


#ifndef __GenerateRLE16_H__
#define __GenerateRLE16_H__

// Includes

// Forward declares
class Image;
class Bitmap_RLE16;

// GenerateRLE16
class GenerateRLE16
	{
	public:
		GenerateRLE16(const Image& image, Bitmap_RLE16* bitmap, bool dither);

	private:
		void CreateFromImage(const Image* image, bool dither);
		void Crop(const Image* image, int* minX, int* minY, int* maxX, int* maxY);
		void Palettize(const Image* image,unsigned short** data, unsigned char** mask, bool dither);
		int GetRLESize(unsigned short* data,unsigned char* mask);
		int CreateRLE(unsigned short* source, unsigned char* mask, unsigned char* destination);
		int GetRLESize_Alpha(unsigned short* data,unsigned char* mask);
		int CreateRLE_Alpha(unsigned short* source, unsigned char* mask, unsigned char* destination);
		int	GetRunLength(unsigned short* data, unsigned char* mask,int size);
		int	GetUniquesLength(unsigned short* data, unsigned char* mask,int size);
		int	GetRunLength_Alpha(unsigned short* data, unsigned char* mask,int size);
		int	GetUniquesLength_Alpha(unsigned short* data, unsigned char* mask,int size);


	private:
		unsigned int opaqueSize_;
		unsigned char* opaqueData_;	
		unsigned int alphaSize_;
		unsigned char* alphaData_;	
		int hPitch_;
		int vPitch_;
		int hOffset_;
		int vOffset_;
	};
	
	 
#endif /* __GenerateRLE16_H__ */
