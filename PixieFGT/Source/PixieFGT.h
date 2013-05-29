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

    bool ValidateRequestedFont(const char* fontname);

    // FontDef Processor
    void FGTFontDefProcessor(const Array<pixietools::FGTFontDef>& fonts, unsigned fontcount);
    void FGTFontDefProcessor(pixietools::FGTFontDef& fontdef);

    // Aliased
    void NOAA_CalculateMaximumGlyphSize(const char* fontname, int fontsize);
    void NOAA_GetKerning(HDC hdc);
    void NOAA_BuildGlyphTable(const char* fontname, int fontsize);
    void NOAA_WriteXML(const char* xmlFilename, const char* fontFilename, const char* fontname, int fontsize);
    Image* NOAA_GenerateGlyphImage(char ascii, HFONT hfont, const char* fontname, int width, int height);
    Glyph* NOAA_CreateGlyph(char ascii, Image* image, int width, int& baseline);

    // Antialiased
    void AA_CalculateMaximumGlyphSize(const char* fontname, int fontsize);
    void AA_GetKerning(HDC hdc);
    void AA_BuildGlyphTable(const char* fontname, int fontsize);
    void AA_WriteXML(const char* xmlFilename, const char* fontFilename, const char* fontname, int fontsize);
    Image* AA_GenerateGlyphImage(char ascii, HFONT hfont, const char* fontname, int width, int height);
    Glyph* AA_CreateGlyph(char ascii, Image* image, int width, int& baseline);
    
    // Rendering
    void RenderStrip(const char* filename, Array<struct Glyph*> glyphs, int spacing);
    void RenderTexture(const char* filename, Array<struct Glyph*> glyphs, int spacing);
};

#endif /* __PixieFGT_H__ */
