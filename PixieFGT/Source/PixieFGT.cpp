//*** PixieFGT.cpp ***

#include "PixieFGT.h"

#include <assert.h>
#include <windows.h>
#include <stdio.h>
#include <cstdarg>

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

void GenerateFonts(const Array<pixietools::FGTFontDef>& fonts, unsigned fontcount)
{
    if (fontcount == 0)
    {
        return;
    }

    if (fonts.Get(0).scalex != 0)
    {
        // generate scaled fonts
        // Generating Font: Courier New 32px Anti-Aliased at scale 100% x 200% ... 100%
        for (unsigned i = 0; i < fontcount; i++)
        {
            pixietools::FGTFontDef& fontdef = fonts.Get(i);

            for (unsigned char i = 0; i < 96; i++)
	        {
                int percentageDone = (int)(100.0f * ((float)i) / 95.0f);
                ProgressMessage("Generating Font: %s %dpx %s at scale %d%% x %d%% ... %d%%", 
                    fontdef.face, fontdef.size, (fontdef.antialias ? "Anti-aliased":"Aliased"),
                    fontdef.scalex * 100, fontdef.scaley * 100, percentageDone);
                Sleep(200);
            }
            printf("\n");
        }
        return;
    }

    // generate non scaled font
    // Generating Font: Courier New 32px Aliased ... 100%
    for (unsigned i = 0; i < fontcount; i++)
    {
        pixietools::FGTFontDef& fontdef = fonts.Get(i);

        printf("Generating Font: %s %dpx %s ... \n", fontdef.face, fontdef.size, fontdef.antialias ? "Anti-aliased":"Aliased");
    }
}


const char* PixieFGT::version = "2.0";

PixieFGT::PixieFGT() 
{
    baseline = 0;
    glyphWidth = 0;
    glyphHeight = 0;
}

PixieFGT::~PixieFGT() 
{
    for (int i = 0; i < glyphs.GetItemCount(); i++)
	{
	    Glyph* glyph = glyphs.Get(i);
	    delete[] glyph->data;
	    delete glyph;
	}
}

void PixieFGT::Main(const char *commandLineString)
{
    printf("Pixie Font Generation Tool %s\n\n", PixieFGT::version);

    // handle the command line
    CommandLine cmd(commandLineString);

    // PixieFGT -cfg cfgfile.xml
    if (cmd.IsCommandLineFlagSet("cfg"))
    {
        if (cmd.GetCommandLineStringCount() < 2)
        {
            DisplayProgramUsage();
            return;
        }

        if (cfgtool.Load(cmd.GetCommandLineString(1).GetString()))
        {
            GenerateFonts(cfgtool.GetFontArray(), cfgtool.GetFontCount());
            GenerateFonts(cfgtool.GetScaleFontArray(), cfgtool.GetScaleFontCount());
        }
        printf("\nDone.\n");

        return;
    }

    // PixieFGT fontname fontsize [spacing] [-antialias] [-texture]
	if (cmd.GetCommandLineStringCount() < 3)
	{
        DisplayProgramUsage();
	    return;
	}

	// get runtime flags
	bool antialias = cmd.IsCommandLineFlagSet("antialias");
    pixietools::FGTTechnique renderingTechnique = pixietools::FGT_STRIP;
	if (cmd.IsCommandLineFlagSet("texture"))
    {
        renderingTechnique = pixietools::FGT_TEXTURE;
    }

    // get parameters
    StringId fontname = cmd.GetCommandLineString(1);
    int fontsize = StringToInt(cmd.GetCommandLineString(2).GetString());
    int spacing = 0;
	if (cmd.GetCommandLineStringCount() >= 4)
	{
	    spacing = StringToInt(cmd.GetCommandLineString(3).GetString());
	}
	
    // Ensure that the font is available
    if (!ValidateRequestedFont(fontname.GetString()))
    {
        printf("ERROR: invalid font name specified: %s\n", fontname.GetString());
        return;
    }

    // validate font size
	if (fontsize <= 0)
	{
        printf("ERROR: invalid font size specified: %d\n", fontsize);
	    return;
	}

	printf(
        "Proceeding with the following settings:\n"
        "    fontname  : %s\n"
        "    fontsize  : %d\n"
        "    spacing   : %d\n"
        "    antialias : %s\n"
        "    rendering : %s\n\n",
        fontname.GetString(),
        fontsize,
        spacing,
        (antialias ? "YES" : "NO"),
        (renderingTechnique == pixietools::FGT_TEXTURE) ? "RenderTexture" : "RenderStrip");


	CalculateMaximumGlyphSize(fontname.GetString(), fontsize, antialias);
	//GetKerning(fontname.GetString(), fontsize, antialias);
    BuildGlyphTable(fontname.GetString(), fontsize, antialias);

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
	SNPrintF(copyto, maxLen, "%d", fontsize);

    // render
    switch(renderingTechnique)
    {
    case pixietools::FGT_STRIP:
        {
            printf("Creating Strip %s...\n", filename);
		    CreateStrip(filename, glyphs, spacing);
        } break;

    case pixietools::FGT_TEXTURE:
        {
            printf("Creating Texture %s...\n", filename);
		    CreateTexture(filename, glyphs, spacing);
        } break;
    default: break;
    }

	char* xmlFilename = new char[StrLen(filename) + 5];
	StrCpy(xmlFilename, filename);
	StrCat(xmlFilename, ".xml");

    WriteXML(xmlFilename, filename, fontname.GetString(), fontsize, antialias);

	// Cleanup
    delete[] xmlFilename;
	delete[] filename;

	printf("Done!\n\n\n");
}

void PixieFGT::DisplayProgramUsage()
{
    printf(
        "Usage Syntax:\n"
        "    PixieFGT can be run in two modes of operation:\n\n"
        "    Inline Mode:\n"
        "    Font information is passed the PixieFGT through the command line."
        "    PixieFGT fontname fontsize [spacing] [-antialias] [-texture|-strip]\n\n"
        "        fontname   : Specifies the name of the font to use (must be installed in the system)\n"
        "        fontsize   : Specifies the size of the font to generate (larger sizes will take longer)\n"
        "        spacing    : Specifies the spacing to use between each character glyph. Defaults to zero.\n"
        "        -antialias : Specifies to use Anti-Aliased (Non-Jagged Edges) Rendering of the font\n"
        "        -texture   : Specifies to output the rendered font to a power-of-two texture\n"
        "        -strip     : (Deprecated flag, now the default. specifies to output the rendered font to a horizontal image strip)\n\n"
        "    Configuration Mode:\n"
        "    PixieFGT -cfg cfgfile\n\n"
        "        -cfg       : Specifies that we want to load a configuration file\n"
        "        cfgfile    : Specifies the name of the configuration file.\n\n");
}

void PixieFGT::CalculateMaximumGlyphSize(const char* fontname, int fontsize, bool antialias)
{
    HDC hdc = CreateCompatibleDC(0);
    SetMapMode(hdc, MM_TEXT);

    if (antialias)
    {
        fontsize *= 16;
    }
    
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

    GetKerning(hdc, antialias);
    
    DeleteObject(hfont);
    DeleteDC(hdc);
    
#if 0
    for (unsigned char ascii = 32; ascii < 128; ascii++)
    {
        int width = 0;
        int height = 0;
        
        GetGlyphSize(ascii, fontname, fontsize, antialias, width, height);
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
#endif

}

void PixieFGT::BuildGlyphTable(const char* fontname, int fontsize, bool antialias)
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

        Image* glyphImage = GenerateGlyphImage(
            ascii,
            hfont,
            fontname,
            fontsize,
            antialias,
            glyphWidth + glyphWidth / 2,
            glyphHeight + glyphHeight / 2);
	    
        Glyph* glyph = CreateGlyph(
            ascii,
            glyphImage,
            antialias,
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

//*** ValidateFont ***

Array<int> fontValidationArray;
int CALLBACK EnumFontFamExProcCallback(ENUMLOGFONTEX* logfont, NEWTEXTMETRICEX* metric, int fontType, LPARAM lParam)
{
    fontValidationArray.Add(0);
    return 1;
}

bool PixieFGT::ValidateRequestedFont(const char* fontname)
{
    fontValidationArray.Clear();

    if (StrLen(fontname) >= LF_FACESIZE)
    {
        printf("ERROR: Font name \"%s\" contains too many characters.\n", fontname);
        return false;
    }

    LOGFONT logfont;
    MemSet(&logfont, 0, sizeof(LOGFONT));

    logfont.lfCharSet = DEFAULT_CHARSET;
    StrCpy(logfont.lfFaceName, fontname);

    HDC hdc = GetDC(NULL);
    EnumFontFamiliesExA(hdc, &logfont, (FONTENUMPROC)EnumFontFamExProcCallback, 0, 0);
    ReleaseDC(NULL, hdc);

    if (fontValidationArray.GetItemCount() == 0)
    {
        printf("ERROR: Font name \"%s\" not found.\n", fontname);
        return false;
    }

    return true;
}

//*** GetKerningPairs ***

void PixieFGT::GetKerning(HDC hdc, bool antialias)
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
            if (antialias)
			{
			    k.amount = (int)((((float)k.amount) / 16.0f) + 0.5f);
			}
            if (k.amount)
            {
                kerning.Add(k);
            }
        }
    }
    delete[] kerningPairs;
}

#if 0
void PixieFGT::GetKerning(const char* fontname, int fontsize, bool antialias)
	{
	if (antialias)
		{
		fontsize*=16;
		}

	HDC hdc = CreateCompatibleDC(0);

	SetMapMode(hdc,MM_TEXT);
	HFONT hfnt = CreateFont(
		-fontsize,
		0,0,0,
		0,
        FALSE, FALSE, FALSE,
        ANSI_CHARSET, OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        PROOF_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE,
        fontname);
	SelectObject(hdc, hfnt);

	int kerningCount=GetKerningPairs(hdc,0,0);
	KERNINGPAIR* kerningPairs=new KERNINGPAIR[kerningCount];
	GetKerningPairs(hdc,kerningCount,kerningPairs);

	for (int i=0; i<kerningCount; i++)
		{
		if (kerningPairs[i].wFirst==(unsigned char)kerningPairs[i].wFirst && kerningPairs[i].wSecond==(unsigned char)kerningPairs[i].wSecond )
			{
			Kerning k;
			k.first=(unsigned char)kerningPairs[i].wFirst;
			k.second=(unsigned char)kerningPairs[i].wSecond;
			k.amount=kerningPairs[i].iKernAmount;
			if (antialias)
				{
				k.amount=(int)((((float)k.amount)/16.0f)+0.5f);
				}
			if (k.amount)
				{
				kerning.Add(k);
				}
			}
		}
	delete[] kerningPairs;

    DeleteObject(hfnt);
    DeleteDC(hdc);
	}
#endif


//*** GetGlyphSize ***

#if 0
void PixieFGT::GetGlyphSize(char ascii, const char* fontname, int fontsize, bool antialias, int& width, int& height)
{
    if (antialias)
    {
        fontsize *= 16;
    }

    HDC hdc = CreateCompatibleDC(0);

    SetMapMode(hdc, MM_TEXT);

    HFONT hfnt = CreateFont(
	    -fontsize,
	    0,0,0,
	    0,
        FALSE, FALSE, FALSE,
        ANSI_CHARSET, OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        PROOF_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE,
        fontname);
    SelectObject(hdc, hfnt);

    SIZE sz;
    GetTextExtentPoint32(hdc, &ascii, 1, &sz);

    width = sz.cx;
    height = sz.cy;

    DeleteObject(hfnt);
    DeleteDC(hdc);
}
#endif


//*** GenerateGlyphImage ***

Image* PixieFGT::GenerateGlyphImage(char ascii, HFONT hfont, const char* fontname, int fontsize, bool antialias, int width, int height)
{
    HDC hdc = CreateCompatibleDC(0);

    if (antialias)
    {
        fontsize *= 16;
    }

    BITMAPINFO bmi;
    unsigned int* pixelData;
    ZeroMemory(&bmi.bmiHeader, sizeof(BITMAPINFOHEADER));
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = width;
    bmi.bmiHeader.biHeight = -height;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    HBITMAP hbmp = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, (void**)(&pixelData), NULL, 0);
    SelectObject(hdc, hbmp);
    RECT fullrect = { 0, 0, width, height };
    FillRect(hdc, &fullrect, (HBRUSH)GetStockObject(GRAY_BRUSH));

    SetMapMode(hdc, MM_TEXT);

    SelectObject(hdc, hfont);


    SetBkColor(hdc, RGB(0x00, 0x00, 0x00)); 
    SetTextAlign(hdc,TA_BASELINE|TA_LEFT);
    SetTextColor(hdc, RGB(0xFF, 0xFF, 0xFF));
    int xpos=(width/6);
    int ypos=height-(height/3);
    if (antialias)
    {
    xpos&=~0xf;
    ypos&=~0xf;
    }
    TextOut(hdc,xpos,ypos,&ascii, 1);


    Image* image=new Image(width,height);
    for (int y=0; y<height; y++)
    {
    for (int x=0; x<width; x++)
    {
    image->SetPixel(x,y,0xff000000 | pixelData[x+y*width]);
    }
    }

    
    DeleteObject(hbmp);
    DeleteDC(hdc);

    return image;
}

#if 0
Image* PixieFGT::GenerateGlyphImage(char ascii, const char* fontname, int fontsize, bool antialias, int width, int height)
	{	
	if (antialias)
		{
		fontsize*=16;
		}


	HDC hdc = CreateCompatibleDC(0);

	BITMAPINFO bmi;
    unsigned int* pixelData;
    ZeroMemory(&bmi.bmiHeader, sizeof(BITMAPINFOHEADER));
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = width;
    bmi.bmiHeader.biHeight = -height;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    HBITMAP hbmp = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, (void**)(&pixelData), NULL, 0);
	SelectObject(hdc,hbmp);
	RECT fullrect={ 0, 0, width, height};
    FillRect(hdc, &fullrect, (HBRUSH)GetStockObject(GRAY_BRUSH));

	SetMapMode(hdc,MM_TEXT);
	HFONT hfnt = CreateFont(
		-fontsize,
		0,0,0,
		100,
        FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        NONANTIALIASED_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE,
        fontname);
	SelectObject(hdc, hfnt);
	

    SetBkColor(hdc, RGB(0x00, 0x00, 0x00)); 
	SetTextAlign(hdc,TA_BASELINE|TA_LEFT);
    SetTextColor(hdc, RGB(0xFF, 0xFF, 0xFF));
	int xpos=(width/6);
	int ypos=height-(height/3);
	if (antialias)
		{
		xpos&=~0xf;
		ypos&=~0xf;
		}
	TextOut(hdc,xpos,ypos,&ascii, 1);


	Image* image=new Image(width,height);
	for (int y=0; y<height; y++)
		{
		for (int x=0; x<width; x++)
			{
			image->SetPixel(x,y,0xff000000 | pixelData[x+y*width]);
			}
		}

    DeleteObject(hfnt);
    DeleteObject(hbmp);
    DeleteDC(hdc);

	return image;
	}
#endif

//*** GenerateScaledGlyphImage ***
#if 0
Image* PixieFGT::GenerateScaledGlyphImage(char ascii, const char* fontname, int fontsize, bool antialias, int width, int height, int scalex, int scaley)
    {
    // if the scale is normal, just use the non-scaled function
    if (scalex == 100 && scaley == 100)
        {
        return GenerateGlyphImage(ascii, fontname, fontsize, antialias, width, height);
        }

    if (antialias)
	    {
	    fontsize *= 16;
	    }

	HDC hdc = CreateCompatibleDC(0);

	BITMAPINFO bmi;
    unsigned int* pixelData;
    ZeroMemory(&bmi.bmiHeader, sizeof(BITMAPINFOHEADER));
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = width;
    bmi.bmiHeader.biHeight = -height;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    HBITMAP hbmp = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, (void**)(&pixelData), NULL, 0);
	SelectObject(hdc,hbmp);
	RECT fullrect={ 0, 0, width, height};
    FillRect(hdc, &fullrect, (HBRUSH)GetStockObject(GRAY_BRUSH));

	SetMapMode(hdc,MM_TEXT);
	HFONT hfnt = CreateFont(
		-fontsize,
		0,0,0,
		100,
        FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        NONANTIALIASED_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE,
        fontname);
	SelectObject(hdc, hfnt);
	

    SetBkColor(hdc, RGB(0x00, 0x00, 0x00)); 
	SetTextAlign(hdc,TA_BASELINE|TA_LEFT);
    SetTextColor(hdc, RGB(0xFF, 0xFF, 0xFF));
	int xpos=(width/6);
	int ypos=height-(height/3);
	if (antialias)
		{
		xpos&=~0xf;
		ypos&=~0xf;
		}
	TextOut(hdc,xpos,ypos,&ascii, 1);


	Image* image=new Image(width,height);
	for (int y=0; y<height; y++)
		{
		for (int x=0; x<width; x++)
			{
			image->SetPixel(x,y,0xff000000 | pixelData[x+y*width]);
			}
		}

    DeleteObject(hfnt);
    DeleteObject(hbmp);
    DeleteDC(hdc);

	return image;
    }
#endif

//*** CreateGlyph ***

Glyph* PixieFGT::CreateGlyph(char ascii, Image* image, bool antialias, int width, int& baseline)
{
	Glyph* glyph = new Glyph();
	MemSet(glyph, 0, sizeof(Glyph));
	glyph->ascii = ascii;
	glyph->width = width;

	if (antialias)
	{
		glyph->width = glyph->width / 16;
	}

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
				if (x < minX)
				{
					minX = x;
				}
				if (x > maxX)
				{
					maxX = x;
				}				
				if (y < minY)
				{
					minY = y;
				}
				if (y > maxY)
				{
					maxY = y;
				}				
			}
		}
	}

	if (antialias)
	{
		minX &= ~0xf;
		maxX &= ~0xf;
		maxX += 15;
		minY &= ~0xf;
		maxY &= ~0xf;
		maxY += 15;
	}
	
	int xpos = (image->GetWidth() / 6);
	int ypos = (image->GetHeight()) - ((image->GetHeight()) / 3);

	if (antialias)
	{
		xpos &= ~0xf;
		ypos &= ~0xf;
	}

	glyph->xoffset = (minX - xpos);
	glyph->yoffset = 0;
	baseline = (ypos - minY);

	if (antialias)
	{
		glyph->xoffset /= 16;
		baseline /= 16;
	}

	int hpitch = maxX - minX + 1;
	int vpitch = maxY - minY + 1;

	if (antialias)
	{
		hpitch /= 16;
		vpitch /= 16;
	}

	glyph->hpitch = hpitch;
	glyph->vpitch = vpitch;

	glyph->data = new unsigned char[hpitch * vpitch];
	MemSet(glyph->data, 0, hpitch * vpitch);

	for (int y = minY; y <= maxY; y++)
	{
		for (int x = minX; x <= maxX; x++)
		{
			if (image->GetPixel(x, y) == 0xffffffff)
			{
				if (antialias)
                {
                    assert((x - minX) / 16 >= 0 && (x - minX) / 16 < hpitch);
					assert((y - minY) / 16 >= 0 && (y - minY) / 16 < vpitch);
					
                    if (glyph->data[((x - minX) / 16) + ((y - minY) / 16)* hpitch] < 255)
					{
						glyph->data[((x - minX) / 16) + ((y - minY) / 16) * hpitch]++;
					}
                }
                else
				{
					assert((x - minX) >= 0 && (x - minX) < hpitch);
					assert((y - minY) >= 0 && (y - minY) < vpitch);
					glyph->data[(x - minX) + (y - minY) * hpitch] = 0xff;
				}
			}
		}
	}


	/*
	//*** DEBUG ***
	Image* img=new Image(hpitch,vpitch);
	for (int y=0; y<vpitch; y++)
		{
		for (int x=0; x<hpitch; x++)
			{
			unsigned char c=glyph->data[x+y*hpitch];
			img->SetPixel(x,y,0xff000000 | c<<16 | c<<8 |c);
			}
		}

	// Grid (for debug
	if (antialias)
		{
		for (int y=0; y<image->GetHeight(); y+=16)
			{
			for (int x=0; x<image->GetWidth(); x+=16)
				{
				image->SetPixel(x,y,0xffff0000);
				if (x>=minX && x<=maxX)
					{
					image->SetPixel(x,minY,0xffff00ff);
					image->SetPixel(x,maxY,0xff00ffff);
					}
				}
			image->SetPixel(minX,y,0xff00ff00);
			image->SetPixel(maxX,y,0xffffff00);
			}
		// baseline
		for (int x=0; x<image->GetWidth(); x++)
			{
			image->SetPixel(x,minY+baseline*16,0xffa0f0a0);
			}
		}

	char buf[256];
	//SNPrintF(buf,256,"%d.tga",ascii);
	//img->Save<ImageFormat_TGA>(buf);
	delete img;

	SNPrintF(buf,256,"_%d.tga",ascii);
	image->Save<ImageFormat_TGA>(buf);
	//*** END DEBUG ***
	/**/

	return glyph;
	}


//*** CreateStrip ***

void PixieFGT::CreateStrip(const char* filename, Array<struct Glyph*> glyphs, int spacing)
	{
	// Calulate total pitch
	int hpitch=0;
	int vpitch=0;
	for (int i=0; i<glyphs.GetItemCount(); i++)
		{
		hpitch+=glyphs.Get(i)->hpitch+spacing*2;
		if (glyphs.Get(i)->vpitch+spacing*2>vpitch)
			{
			vpitch=glyphs.Get(i)->vpitch+spacing*2;
			}
		}

	Image image(hpitch,vpitch);
	int xp=0;
	for (int i=0; i<glyphs.GetItemCount(); i++)
		{
		Glyph* glyph=glyphs.Get(i);
		for (int y=0; y<glyph->vpitch; y++)
			{
			for (int x=0; x<glyph->hpitch; x++)
				{
				unsigned int c=0x00ffffff | (glyph->data[x+y*glyph->hpitch]<<24);
				image.SetPixel(spacing+xp+x,spacing+y,c);
				}
			}
		glyph->xoffset-=spacing;
		glyph->yoffset-=spacing;
		glyph->x1=xp;
		glyph->x2=xp+glyph->hpitch-1+spacing*2;
		glyph->y1=0;
		glyph->y2=glyph->vpitch-1+spacing*2;
		xp+=glyph->hpitch+spacing*2;
		}

	char* imageFilename=new char[StrLen(filename)+5];
	StrCpy(imageFilename,filename);
	StrCat(imageFilename,".tga");
	image.Save<ImageFormat_TGA>(imageFilename);
	delete[] imageFilename;
	}


//*** CreateTexture ***

void PixieFGT::CreateTexture(const char* filename, Array<struct Glyph*> glyphs, int spacing)
	{
	// Calulate total size
	int size=0;
	for (int i=0; i<glyphs.GetItemCount(); i++)
		{
		size+=(glyphs.Get(i)->hpitch+spacing*2)*(glyphs.Get(i)->vpitch+spacing*2);
		}

	// Find smallest power of two
	int dimensionX=1;
	int dimensionY=1;
	while (dimensionX*dimensionY<size)
		{
		dimensionY*=2;
		if (dimensionX*dimensionY<size)
			{
			dimensionX*=2;
			}
		}

	bool tryForSize=true;
	Image* image=0;
	while (tryForSize)
		{
		tryForSize=false;
		image=new Image(dimensionX,dimensionY);
		int xp=0;
		int yp=0;
		for (int i=0; i<glyphs.GetItemCount(); i++)
			{
			Glyph* glyph=glyphs.Get(i);
			if (xp+glyph->hpitch>=dimensionX)
				{
				yp+=glyph->vpitch;
				xp=0;
				}
			if (yp+glyph->vpitch>=dimensionY)
				{
				if (dimensionX<dimensionY)
					{
					dimensionX*=2;
					}
				else
					{
					dimensionY*=2;
					}
				tryForSize=true;
				delete image;
				image=0;
				break;
				}
			for (int y=0; y<glyph->vpitch; y++)
				{
				for (int x=0; x<glyph->hpitch; x++)
					{
					unsigned int c=0x00ffffff | (glyph->data[x+y*glyph->hpitch]<<24);
					image->SetPixel(xp+x,yp+y,c);
					}
				}
			glyph->x1=xp;
			glyph->x2=xp+glyph->hpitch-1;
			glyph->y1=yp;
			glyph->y2=yp+glyph->vpitch-1;
			xp+=glyph->hpitch;
			}
		}

	char* imageFilename=new char[StrLen(filename)+5];
	StrCpy(imageFilename,filename);
	StrCat(imageFilename,".tga");
	image->Save<ImageFormat_TGA>(imageFilename);
	delete[] imageFilename;
	delete image;
	}

void PixieFGT::WriteXML(const char* xmlFilename, const char* fontFilename, const char* fontname, int fontsize, bool antialias)
{
    printf("Writing XML %s...\n", xmlFilename);
	FILE* fp=fopen(xmlFilename,"w");
	int lineSpacing=glyphHeight;
	if (antialias)
		{
		lineSpacing=(int)(lineSpacing/16.0f+0.5f);
		}
	int glyphCount=glyphs.GetItemCount();
	fprintf(fp,"<PixieFont version=\"0\" name=\"%s\" size=\"%d\" glyphCount=\"%d\" glyphHeight=\"%d\" baseline=\"%d\">\n",fontname, fontsize, glyphCount, lineSpacing, baseline);
	fprintf(fp,"<Layer filename=\"%s.tga\"/>\n",fontFilename);

	for (int i=0; i<glyphs.GetItemCount(); i++)
		{
		Glyph* glyph=glyphs.Get(i);
		fprintf(fp,"<Glyph");
		fprintf(fp," ascii=\"%d\"",glyph->ascii);
		fprintf(fp," width=\"%d\"",glyph->width);
		fprintf(fp," x1=\"%d\"",glyph->x1);
		fprintf(fp," y1=\"%d\"",glyph->y1);
		fprintf(fp," x2=\"%d\"",glyph->x2);
		fprintf(fp," y2=\"%d\"",glyph->y2);
		fprintf(fp," xoffset=\"%d\"",glyph->xoffset);
		fprintf(fp," yoffset=\"%d\"",glyph->yoffset);
		fprintf(fp,"/>\n");
		}
	
	for (int i=0; i<kerning.GetItemCount(); i++)
		{
		fprintf(fp,"<Kerning");
		fprintf(fp," first=\"%d\"",(unsigned char)kerning.Get(i).first);
		fprintf(fp," second=\"%d\"",(unsigned char)kerning.Get(i).second);
		fprintf(fp," amount=\"%d\"",kerning.Get(i).amount);
		fprintf(fp,"/>\n");
		}

	fprintf(fp,"</PixieFont>\n");
}