//*** PixieFGTConfiguration.h ***

#ifndef __PixieFGTConfiguration_H__
#define __PixieFGTConfiguration_H__

#include "Pixie.h"
#include <windows.h>
#include <cstdio>

namespace pixietools
{
    enum FGTTechnique
    {
        FGT_STRIP,
        FGT_TEXTURE
    };

    struct FGTFontDef
    {
        char face[LF_FACESIZE];
        unsigned size;
        unsigned spacing;
        unsigned scalex;
        unsigned scaley;
        bool antialias;
        FGTTechnique technique; 

        void Print()
        {
            printf("FGTFontDef {face=%s, size=%d, spacing=%d, scalex=%d, scaley=%d,antialias=%s}\n", face, size, spacing, scalex, scaley, antialias?"true":"false");
        }
    };
};

namespace tinyxml2 { class XMLElement; };

class PixieFGTConfiguration
{
public:
    PixieFGTConfiguration();
    ~PixieFGTConfiguration();
    bool Load(const char* filename);
    const pixie::Array<pixietools::FGTFontDef>& GetFontArray() const;
    const pixie::Array<pixietools::FGTFontDef>& GetScaleFontArray() const;
    unsigned GetFontCount() const;
    unsigned GetScaleFontCount() const;

private:
    pixie::Array<pixietools::FGTFontDef> fonts;
    pixie::Array<pixietools::FGTFontDef> scalefonts;

    bool ParseFontNode(tinyxml2::XMLElement* fontnode, pixietools::FGTFontDef& fontdef);
    bool ParseScaleFontNode(tinyxml2::XMLElement* fontnode, pixietools::FGTFontDef& fontdef);
};

#endif /* __PixieFGTConfiguration_H__ */
