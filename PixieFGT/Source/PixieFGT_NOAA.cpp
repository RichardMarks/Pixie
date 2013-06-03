
#include "PixieFGT.h"

#include <assert.h>
#include <windows.h>
#include <stdio.h>
#include <cstdarg>

using namespace pixie;

void PixieFGT::NOAA_CalculateMaximumGlyphSize(const char* fontname, int fontsize)
{
    HDC hdc = CreateCompatibleDC(0);
    
    SetMapMode(hdc, MM_TEXT);

    HFONT hfont = CreateFont(
	    -fontsize,
	    0,0,0,
	    0,
        FALSE, FALSE, FALSE,
        ANSI_CHARSET, OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        PROOF_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE,
        fontname);

    SelectObject(hdc, hfont);
    
    for (unsigned char ascii = 32; ascii < 128; ascii++)
    {
        char asciichar = ascii;
        SIZE sz;
        GetTextExtentPoint32(hdc, &asciichar, 1, &sz);
        int width = sz.cx;
        int height = sz.cy;
        
        glyphWidths.Add(width);

        if (width > glyphWidth)
        {
            glyphWidth = width;
        }

        if (height > glyphHeight)
        {
            glyphHeight = height;
        }
    }

    printf("\n*** Glyph Width %d x Glyph Height %d\n", glyphWidth, glyphHeight);

    this->NOAA_GetKerning(hdc);
    
    DeleteObject(hfont);
    DeleteDC(hdc);
}

void PixieFGT::NOAA_GetKerning(HDC hdc)
{
    kerning.Clear();

    int kerningCount = GetKerningPairs(hdc, 0, 0);

    KERNINGPAIR* kerningPairs = new KERNINGPAIR[kerningCount];
    GetKerningPairs(hdc, kerningCount, kerningPairs);

    for (int i = 0; i < kerningCount; i++)
    {
        KERNINGPAIR& pair = kerningPairs[i];
        if (
            pair.wFirst == (unsigned char)pair.wFirst && 
            pair.wSecond == (unsigned char)pair.wSecond)
        {
            Kerning k;
            k.first = (unsigned char)pair.wFirst;
            k.second = (unsigned char)pair.wSecond;
            k.amount = pair.iKernAmount;
            
            if (k.amount)
            {
                kerning.Add(k);
            }
        }
    }
    delete[] kerningPairs;
}

void PixieFGT::NOAA_BuildGlyphTable(const char* fontname, int fontsize)
{
    int baseline = 0;

    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;

    HFONT hfont = CreateFont(
        -fontsize,
        0,0,0,
        100,
        FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        NONANTIALIASED_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE,
        fontname);

	for (unsigned char i = 0; i < 96; i++)
    {
	    unsigned char ascii = 32 + i;

        Image* glyphImage = this->NOAA_GenerateGlyphImage(
            ascii,
            hfont,
            fontname,
            glyphWidth + glyphWidth / 2,
            glyphHeight + glyphHeight / 2);
	    
        Glyph* glyph = this->NOAA_CreateGlyph(
            ascii,
            glyphImage,
            glyphWidths.Get(ascii - 32),
            baseline);
	    
        glyphs.Add(glyph);
	    
        delete glyphImage;
        
        int percentageDone = (int)(100.0f * ((float)i) / 95.0f);
        GetConsoleScreenBufferInfo(consoleHandle, &consoleInfo);
        COORD consoleCursorPosition = consoleInfo.dwCursorPosition;
        consoleCursorPosition.Y--;
        SetConsoleCursorPosition(consoleHandle, consoleCursorPosition);
        printf("\nGenerating characters... %d%% done", percentageDone);
    }

    DeleteObject(hfont);

    printf("\n\n");
}

void PixieFGT::NOAA_WriteXML(const char* xmlFilename, const char* fontFilename, const char* fontname, int fontsize)
{
    printf("Saving %s...\n", xmlFilename);

	int glyphCount = glyphs.GetItemCount();

    FILE* fp = fopen(xmlFilename, "w");
    if (fp == NULL)
    {
        printf("ERROR: Writing to %s failed.\n", xmlFilename);
        return;
    }

	fprintf(fp, "<PixieFont version=\"0\" name=\"%s\" size=\"%d\" glyphCount=\"%d\" glyphHeight=\"%d\" baseline=\"%d\">\n", 
        fontname, 
        fontsize, 
        glyphCount, 
        glyphHeight, 
        baseline);

	fprintf(fp, "    <Layer filename=\"%s.tga\"/>\n", 
        fontFilename);

	for (int i = 0; i < glyphs.GetItemCount(); i++)
	{
		Glyph& glyph = *glyphs.Get(i);

        fprintf(fp, "    <Glyph ascii=\"%d\" width=\"%d\" x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" xoffset=\"%d\" yoffset=\"%d\" />\n",
            glyph.ascii,
            glyph.width,
            glyph.x1,
            glyph.y1,
            glyph.x2,
            glyph.y2,
            glyph.xoffset,
            glyph.yoffset);
		}
	
	for (int i = 0; i < kerning.GetItemCount(); i++)
	{
        Kerning& k = kerning.Get(i);

        fprintf(fp, "    <Kerning first=\"%d\" second=\"%d\" amount=\"%d\" />\n", 
            (unsigned char)k.first, 
            (unsigned char)k.second, 
            k.amount);
	}

	fprintf(fp, "</PixieFont>\n");
    fclose(fp);
}

Image* PixieFGT::NOAA_GenerateGlyphImage(char ascii, HFONT hfont, const char* fontname, int width, int height)
{
    HDC hdc = CreateCompatibleDC(0);

    BITMAPINFO bmi;
    ZeroMemory(&bmi.bmiHeader, sizeof(BITMAPINFOHEADER));

    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = width;
    bmi.bmiHeader.biHeight = -height;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    unsigned int* pixelData;
    HBITMAP hbmp = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, (void**)(&pixelData), NULL, 0);

    RECT fullrect = { 0, 0, width, height };

    SetMapMode(hdc, MM_TEXT);
    
    SelectObject(hdc, hbmp);
    SelectObject(hdc, hfont);

    FillRect(hdc, &fullrect, (HBRUSH)GetStockObject(GRAY_BRUSH));

    SetBkColor(hdc, RGB(0x00, 0x00, 0x00)); 
    SetTextAlign(hdc, TA_BASELINE | TA_LEFT);
    SetTextColor(hdc, RGB(0xFF, 0xFF, 0xFF));

    int xpos = (width / 6);
    int ypos = height-(height/3);
    
    TextOut(hdc, xpos, ypos, &ascii, 1);

    Image* image = new Image(width, height);
    for (int y = 0; y < height; y++)
    {
        int y2 = y * width;
        for (int x = 0; x < width; x++)
        {
            image->SetPixel(x, y, 0xff000000 | pixelData[x + y2]);
        }
    }

    DeleteObject(hbmp);
    DeleteDC(hdc);

    return image;
}

Glyph* PixieFGT::NOAA_CreateGlyph(char ascii, Image* image, int width, int& baseline)
{
    Glyph* glyph = new Glyph();
	MemSet(glyph, 0, sizeof(Glyph));

	glyph->ascii = ascii;
	glyph->width = width;

	// Crop image
	int minX = image->GetWidth();
	int maxX = 0;
	int minY = image->GetHeight();
	int maxY = 0;
	for (int y = 0; y < image->GetHeight(); y++)
	{
		for (int x = 0; x < image->GetWidth(); x++)
		{
			if (image->GetPixel(x, y) == 0xff000000)
			{
				if (x < minX) { minX = x; }
				if (x > maxX) { maxX = x; }				
				if (y < minY) { minY = y; }
				if (y > maxY) { maxY = y; }				
			}
		}
	}

	int xpos = (image->GetWidth() / 6);
	int ypos = (image->GetHeight()) - ((image->GetHeight()) / 3);

    baseline = (ypos - minY);

	glyph->xoffset = (minX - xpos);
	glyph->yoffset = 0;
	
	int hpitch = maxX - minX + 1;
	int vpitch = maxY - minY + 1;

	glyph->hpitch = hpitch;
	glyph->vpitch = vpitch;

	glyph->data = new unsigned char[hpitch * vpitch];
	MemSet(glyph->data, 0, hpitch * vpitch);

	for (int y = minY; y <= maxY; y++)
	{
        int y2 = (y - minY);
        int y3 = y2 * hpitch;
		for (int x = minX; x <= maxX; x++)
		{
			if (image->GetPixel(x, y) == 0xffffffff)
			{
                int x2 = (x - minX);

				assert(x2 >= 0 && x2 < hpitch);
				assert(y2 >= 0 && y2 < vpitch);

				glyph->data[x2 + y3] = 0xff;
			}
		}
	}

	return glyph;
}
