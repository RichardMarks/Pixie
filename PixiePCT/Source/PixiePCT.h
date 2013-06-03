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
    pixie::Image image;
    pixie::StringId filename;
	};

struct FontFile
	{
    pixie::ImageFont* font;
    pixie::StringId filename;
	};

template <class Type> void Convert(pixie::Array<ImageFile>& images,bool strip, bool dither, const char* header);

template <class Type> void Convert(pixie::Array<FontFile>& fonts, bool dither, const char* header);

pixie::StringId GetPixieFilename(pixie::StringId filename, bool stripNumbers = false, const char* extension=".pix");

void DitherImage(pixie::Image& image);	

void LoadFile(const char* filename,	pixie::Array<ImageFile>& images, pixie::Array<FontFile>& fonts, bool convertImmediately, bool dither);

void LoadDirectory(const char* filename, pixie::Array<ImageFile>& images, pixie::Array<FontFile>& fonts, bool convertImmediately, bool dither);

#endif /* __PixiePCT_H__ */
