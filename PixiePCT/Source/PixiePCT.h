//*** PixiePCT.h ***

#ifndef __PixiePCT_H__
#define __PixiePCT_H__

#include "Array.h"
#include "StringId.h"
#include "Image.h"
#include "ImageFont.h"

void PixiePCTMain(const char* commandLine);

struct ImageFile
	{
	Image image;
	StringId filename;
	};

struct FontFile
	{
	ImageFont* font;
	StringId filename;
	};

template <class Type> void Convert(Array<ImageFile>& images,bool strip, bool dither, const char* header);

template <class Type> void Convert(Array<FontFile>& fonts, bool dither, const char* header);

StringId GetPixieFilename(StringId filename, bool stripNumbers = false, const char* extension=".pix");

void DitherImage(Image& image);	

void LoadFile(const char* filename,	Array<ImageFile>& images, Array<FontFile>& fonts, bool convertImmediately, bool dither);

void LoadDirectory(const char* filename, Array<ImageFile>& images, Array<FontFile>& fonts, bool convertImmediately, bool dither);

#endif /* __PixiePCT_H__ */
