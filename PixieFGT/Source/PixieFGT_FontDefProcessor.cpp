
#include "PixieFGT.h"

#include <assert.h>
#include <windows.h>
#include <stdio.h>
#include <cstdarg>

void ProgressMessage(const char* text, ...);

void PixieFGT::FGTFontDefProcessor(const Array<pixietools::FGTFontDef>& fonts, unsigned fontcount)
{
    if (fontcount == 0)
    {
        return;
    }

    if (fonts.Get(0).scalex != 0)
    {
#if 0
        // generate scaled fonts
        // Generating Font: Courier New 32px Anti-Aliased at scale 100% x 200% ... 100%
        for (unsigned i = 0; i < fontcount; i++)
        {
            pixietools::FGTFontDef& fontdef = fonts.Get(i);
            printf("***Generating Font %d of %d\n", i + 1, fontcount);
            this->FGTScaleFontDefProcessor(fontdef);
        }
#endif
        printf("\n\n*** Whoops! ScaleFont not yet implemented. Sorry!\n\n");
        return;
    }

    for (unsigned i = 0; i < fontcount; i++)
    {
        pixietools::FGTFontDef& fontdef = fonts.Get(i);
        printf("***Generating Font %d of %d\n", i + 1, fontcount);
        this->FGTFontDefProcessor(fontdef);
    }
}

void PixieFGT::FGTFontDefProcessor(pixietools::FGTFontDef& fontdef)
{
    StringId fontname(fontdef.face);

    // Ensure that the font is available
    if (!this->ValidateRequestedFont(fontname.GetString()))
    {
        printf("ERROR: invalid font name specified: %s\n", fontname.GetString());
        return;
    }

    // validate font size
	if (fontdef.size <= 0)
	{
        printf("ERROR: invalid font size specified: %d\n", fontdef.size);
	    return;
	}

    // generate a new filename (strip spaces from font name and concatenate font size)
	int maxLen = StrLen(fontname.GetString()) + 10;
	char* filename = new char[maxLen];
	char* copyfrom = (char*)fontname.GetString();
	char* copyto = filename;
	while (*copyfrom != 0)
	{
	    if (*copyfrom != ' ')
	    {
	        *copyto = *copyfrom;
	        copyto++;
	    }
	    copyfrom++;
	}
	SNPrintF(copyto, maxLen, "%d", fontdef.size);

    char* xmlFilename = new char[StrLen(filename) + 5];
	StrCpy(xmlFilename, filename);
	StrCat(xmlFilename, ".xml");

    printf(
        "Proceeding with the following settings:\n"
        "    fontname  : %s\n"
        "    fontsize  : %d\n"
        "    spacing   : %d\n"
        "    antialias : %s\n"
        "    rendering : %s to %s.tga|%s\n\n",
        fontname.GetString(),
        fontdef.size,
        fontdef.spacing,
        (fontdef.antialias ? "YES" : "NO"),
        ((fontdef.technique == pixietools::FGT_TEXTURE) ? "RenderTexture" : "RenderStrip"),
        filename, xmlFilename);

    // generate font glyphs
    if (fontdef.antialias)
    {
        this->AA_CalculateMaximumGlyphSize(fontname.GetString(), fontdef.size);
        this->AA_BuildGlyphTable(fontname.GetString(), fontdef.size);
    }
    else
    {
        this->NOAA_CalculateMaximumGlyphSize(fontname.GetString(), fontdef.size);
        this->NOAA_BuildGlyphTable(fontname.GetString(), fontdef.size);
    }

    // render font glyphs to image file using specified rendering technique
    switch(fontdef.technique)
    {
    case pixietools::FGT_STRIP: 
        { 
            this->RenderStrip(filename, glyphs, fontdef.spacing); 
        } break;

    case pixietools::FGT_TEXTURE: 
        { 
            this->RenderTexture(filename, glyphs, fontdef.spacing); 
        } break;

    default: break;
    }

    // generate xml dataset file
    if (fontdef.antialias)
    {
        this->AA_WriteXML(xmlFilename, filename, fontname.GetString(), fontdef.size);
    }
    else
    {
        this->NOAA_WriteXML(xmlFilename, filename, fontname.GetString(), fontdef.size);
    }

    delete [] xmlFilename;
	delete [] filename;
}

// helper function for writing inline progress messages to stdout
void ProgressMessage(const char* text, ...)
{
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    GetConsoleScreenBufferInfo(consoleHandle, &consoleInfo);
    COORD consoleCursorPosition = consoleInfo.dwCursorPosition;
    consoleCursorPosition.Y--;
    SetConsoleCursorPosition(consoleHandle, consoleCursorPosition);

    char buffer[4096];
    va_list va;
    va_start(va, text);
    vsnprintf(buffer, 4095, text, va);
    va_end(va);
    
    printf("\n%s", buffer);
}
