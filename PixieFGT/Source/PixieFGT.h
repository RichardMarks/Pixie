//*** PixieFGT.h ***

#ifndef __PixieFGT_H__
#define __PixieFGT_H__

#include "Pixie.h"

#include "PixieFGTConfiguration.h"

 
// Structure to hold kerning information
struct Kerning
	{
	char first;
	char second;
	int amount;
	};

// Structure to hold glyph data
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


class PixieFGT
{
public:
    static const char* version;
    PixieFGT();
    ~PixieFGT();

    void Main(const char* commandLineString);

private:

    PixieFGTConfiguration cfgtool;

    Array<Glyph*> glyphs;
    Array<int> glyphWidths;
    Array<Kerning> kerning;

	int glyphWidth;
	int glyphHeight;
    int baseline;

    void DisplayProgramUsage();

    void CalculateMaximumGlyphSize(const char* fontname, int fontsize, bool antialias);

    void BuildGlyphTable(const char* fontname, int fontsize, bool antialias);
        
    // Checks to see if the specified font is available
    bool ValidateRequestedFont(const char* fontname);

    // Retrieves all kerning pairs for the specified font
    void GetKerning(HDC hdc, bool antialias);
    //void GetKerning(const char* fontname, int fontsize, bool antialias);


    // Finds the width and height of the specified character of the specified font
    //void GetGlyphSize(char ascii, const char* fontname, int fontsize, bool antialias, int& width, int& height);

    // Render the specified character
    Image* GenerateGlyphImage(char ascii, HFONT hfont, const char* fontname, int fontsize, bool antialias, int width, int height);
    //Image* GenerateGlyphImage(char ascii, const char* fontname, int fontsize, bool antialias, int width, int height);

    // Render the specified character with the specified scale
    //Image* GenerateScaledGlyphImage(char ascii, const char* fontname, int fontsize, bool antialias, int width, int height, int scalex, int scaley);

    // Extract pixel and glyph data from the specified image
    struct Glyph* CreateGlyph(char ascii, Image* image, bool antialias, int width, int& baseline);

    // Arrange all characters on a strip
    void CreateStrip(const char* filename, Array<struct Glyph*> glyphs, int spacing);

    // Arrange all characters on a power-of-two texture
    void CreateTexture(const char* filename, Array<struct Glyph*> glyphs, int spacing);

    // Writes the XML font definition file
    void WriteXML(const char* xmlFilename, const char* fontFilename, const char* fontname, int fontsize, bool antialias);
};

#endif /* __PixieFGT_H__ */
