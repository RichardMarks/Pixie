
/*
    <PixieFGT>
        <Font face="Courier New" size="32" spacing="1" aa="false" technique="texture" />
        <ScaleFont face="Impact" size="32" spacing="1" aa="true" technique="texture" scalex="1" scaley="2" />
    </PixieFGT>


    $ PixieFGT pixiecfg.xml -cfg
    Pixie Font Generation Tool 1.1

    Reading Configuration File: pixiecfg.xml
    Number of Font Definitions: 1
    Number of ScaleFont Definitions: 1

    Generating Font: Courier New 32px Aliased ... 100%
    Writing CourierNew32_texture.png

    Generating Font: Courier New 32px Anti-Aliased at scale 100% x 200% ... 100%
    Writing CourierNew32_aa_texture.png

    Done
    $


*/

#include "PixieFGTConfiguration.h"
#include "tinyxml2.h"

using namespace pixie;

PixieFGTConfiguration::PixieFGTConfiguration()
{
}

PixieFGTConfiguration::~PixieFGTConfiguration()
{
    fonts.Clear();
    scalefonts.Clear();
}

bool PixieFGTConfiguration::Load(const char* filename)
{
    fonts.Clear();
    scalefonts.Clear();

    printf("Reading Configuration File: %s\n", filename);

    tinyxml2::XMLDocument doc;
    doc.LoadFile(filename);

    tinyxml2::XMLElement* root = doc.FirstChildElement("PixieFGT");

    // Read Font definitions
    for (tinyxml2::XMLElement* fontnode = root->FirstChildElement("Font");
        fontnode != NULL;
        fontnode = fontnode->NextSiblingElement("Font"))
    {
        pixietools::FGTFontDef fontdef;
        fontdef.scalex = 0;
        fontdef.scaley = 0;
        ParseFontNode(fontnode, fontdef);
        fonts.Add(fontdef);
    }


    // Read ScaleFont definitions
    for (tinyxml2::XMLElement* fontnode = root->FirstChildElement("ScaleFont");
        fontnode != NULL;
        fontnode = fontnode->NextSiblingElement("ScaleFont"))
    {
        pixietools::FGTFontDef fontdef;
        fontdef.scalex = 0;
        fontdef.scaley = 0;
        ParseScaleFontNode(fontnode, fontdef);
        scalefonts.Add(fontdef);
    }

    printf("Number of Font Definitions: %d\nNumber of ScaleFont Definitions: %d\n\n", fonts.GetItemCount(), scalefonts.GetItemCount());

    return true;
}

const Array<pixietools::FGTFontDef>& PixieFGTConfiguration::GetFontArray() const
{
    return fonts;
}

const Array<pixietools::FGTFontDef>& PixieFGTConfiguration::GetScaleFontArray() const
{
    return scalefonts;
}

unsigned PixieFGTConfiguration::GetFontCount() const
{
    return fonts.GetItemCount();
}

unsigned PixieFGTConfiguration::GetScaleFontCount() const
{
    return scalefonts.GetItemCount();
}

bool PixieFGTConfiguration::ParseFontNode(tinyxml2::XMLElement* fontnode, pixietools::FGTFontDef& fontdef)
{
    if (fontnode->Attribute("face") != NULL)
    {
        StrCpy(fontdef.face, fontnode->Attribute("face"));
    }
    else
    {
        printf("Error: Missing expected \"face\" attribute in Font definition.\n");
        return false;
    }
    
    fontdef.size = fontnode->IntAttribute("size");

    if (fontdef.size == 0)
    {
        printf("Warning: Missing expected \"size\" attribute in Font definition. Using a default of 16.\n");
        fontdef.size = 16;
    }

    fontdef.spacing = fontnode->IntAttribute("spacing");
    
    fontdef.antialias = false;
    if (fontnode->Attribute("aa", "true") != NULL)
    {
        fontdef.antialias = true;
    }

    fontdef.technique = pixietools::FGT_STRIP;
    if (fontnode->Attribute("technique", "texture") != NULL)
    {
        fontdef.technique = pixietools::FGT_TEXTURE;
    }

    return true;
}

bool PixieFGTConfiguration::ParseScaleFontNode(tinyxml2::XMLElement* fontnode, pixietools::FGTFontDef& fontdef)
{
    if (!ParseFontNode(fontnode, fontdef))
    {
        return false;
    }

    fontdef.scalex = fontnode->IntAttribute("scalex");
    if (fontdef.scalex == 0)
    {
        printf("Warning: Missing expected \"scalex\" attribute in Font definition. Using a default of 1.\n");
        fontdef.scalex = 1;
    }

    fontdef.scaley = fontnode->IntAttribute("scaley");
    if (fontdef.scaley == 0)
    {
        printf("Warning: Missing expected \"scaley\" attribute in Font definition. Using a default of 1.\n");
        fontdef.scaley = 1;
    }

    return true;
}
