//*** PixieFGT.h ***

#ifndef __PixieFGT_H__
#define __PixieFGT_H__

#include "Array.h"
#include "StringId.h"
#include "Image.h"

// Main function, called from Main.cpp
void PixieFGTMain(const char* commandLine);
 
// Structure to hold kerning information
struct Kerning
	{
	char first;
	char second;
	int amount;
	};

// Retrieves all kerning pairs for the specified font
void GetKerning(const char* fontname, int fontsize, bool antialias, Array<Kerning>& kerning);

// Finds the width and height of the specified character of the specified font
void GetGlyphSize(char ascii, const char* fontname, int fontsize, bool antialias, int& width, int& height);

// Render the specified character
Image* GenerateGlyphImage(char ascii, const char* fontname, int fontsize, bool antialias, int width, int height);

// Extract pixel and glyoh data from the specified image
struct Glyph* CreateGlyph(char ascii, Image* image, bool antialias, int width, int& baseline);

// Arrange all characters on a strip
void CreateStrip(const char* filename, Array<struct Glyph*> glyphs, int spacing);

// Arrange all characters on a power-of-two texture
void CreateTexture(const char* filename, Array<struct Glyph*> glyphs, int spacing);

// Structure to hold glyoh data
struct Glyph
	{
	char ascii;
	int width;
	int xoffset;
	int yoffset;
	int hpitch;
	int vpitch;
	unsigned char* data;
	int x1;
	int y1;
	int x2;
	int y2;
	};

#endif /* __PixieFGT_H__ */
