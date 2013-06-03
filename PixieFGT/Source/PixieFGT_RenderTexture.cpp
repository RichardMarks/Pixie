
#include "PixieFGT.h"

using namespace pixie;

void PixieFGT::RenderTexture(const char* filename, Array<struct Glyph*> glyphs, int spacing)
{
    printf("Rendering Texture...\n");

    // calculate total size
    int size = 0;
    for (int i = 0; i < glyphs.GetItemCount(); i++)
    {
        size += (glyphs.Get(i)->hpitch + spacing * 2) * (glyphs.Get(i)->vpitch + spacing * 2);
    }

    // Find smallest power of two
    int dimensionX = 1;
    int dimensionY = 1;
    while (dimensionX * dimensionY < size)
    {
        dimensionY *= 2;
        if (dimensionX * dimensionY < size)
        {
            dimensionX *= 2;
        }
    }

    bool tryForSize = true;
    Image* image = 0;
    while (tryForSize)
    {
        tryForSize = false;
        image = new Image(dimensionX, dimensionY);
        int xp = 0;
        int yp = 0;
        for (int i = 0; i < glyphs.GetItemCount(); i++)
        {
            Glyph* glyph = glyphs.Get(i);
            if (xp + glyph->hpitch >= dimensionX)
            {
                yp += glyph->vpitch;
                xp = 0;
            }

            if (yp + glyph->vpitch >= dimensionY)
            {
                if (dimensionX < dimensionY)
                {
                    dimensionX *= 2;
                }
                else
                {
                    dimensionY *= 2;
                }
                tryForSize = true;
                delete image;
                image = 0;
                break;
            }

            for (int y = 0; y < glyph->vpitch; y++)
            {
                for (int x = 0; x < glyph->hpitch; x++)
                {
                    unsigned int c = 0x00ffffff | (glyph->data[x + y * glyph->hpitch] << 24);
                    image->SetPixel(xp + x, yp + y, c);
                }
            }

            glyph->x1 = xp;
            glyph->x2 = xp + glyph->hpitch - 1;
            glyph->y1 = yp;
            glyph->y2 = yp + glyph->vpitch - 1;

            xp += glyph->hpitch;
        }
    }

    char* imageFilename = new char[StrLen(filename) + 5];
    
    StrCpy(imageFilename, filename);
    StrCat(imageFilename, ".tga");
    
    printf("Saving %s...\n", imageFilename);

    image->Save<ImageFormat_TGA>(imageFilename);
    
    delete [] imageFilename;
    delete image;
}
