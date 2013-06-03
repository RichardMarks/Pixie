//*** PixieFGT.cpp ***

#include "PixieFGT.h"

#include <windows.h>

using namespace pixie;

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
            this->FGTFontDefProcessor(cfgtool.GetFontArray(), cfgtool.GetFontCount());
            this->FGTFontDefProcessor(cfgtool.GetScaleFontArray(), cfgtool.GetScaleFontCount());
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
	bool antialias = cmd.IsCommandLineFlagSet("antialias") || cmd.IsCommandLineFlagSet("aa");
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

    // set up the font definition structure with the passed params
    pixietools::FGTFontDef fontdef;
    StrCpy(fontdef.face, fontname.GetString());
    fontdef.size = fontsize;
    fontdef.spacing = spacing;
    fontdef.antialias = antialias;
    fontdef.technique = renderingTechnique;
    fontdef.scalex = 0;
    fontdef.scaley = 0;

    // go time!
    this->FGTFontDefProcessor(fontdef);
	printf("\nDone.\n");
}

void PixieFGT::DisplayProgramUsage()
{
    printf(
        "Usage Syntax:\n"
        "    PixieFGT can be run in two modes of operation:\n\n"
        "    Inline Mode:\n"
        "    Font information is passed to PixieFGT through the command line.\n"
        "    PixieFGT fontname fontsize [spacing] [-antialias] [-texture|-strip]\n\n"
        "        fontname   : Specifies the name of the font to use (must be installed in the system)\n"
        "        fontsize   : Specifies the size of the font to generate (larger sizes will take longer)\n"
        "        spacing    : Specifies the spacing to use between each character glyph. Defaults to zero.\n"
        "        -antialias : Specifies to use Anti-Aliased (Non-Jagged Edges) Rendering of the font\n"
        "        -aa        : Same as above\n"
        "        -texture   : Specifies to output the rendered font to a power-of-two texture\n"
        "        -strip     : (Deprecated flag, now the default. specifies to output the rendered font to a horizontal image strip)\n\n"
        "    Configuration Mode:\n"
        "    Font information is passed to PixieFGT through an XML configuration file.\n"
        "    PixieFGT -cfg cfgfile\n\n"
        "        -cfg       : Specifies that we want to load a configuration file\n"
        "        cfgfile    : Specifies the name of the configuration file.\n\n");
}

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
