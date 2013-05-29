
#include "PixieFGT.h"

void PixieFGT::RenderStrip(const char* filename, Array<struct Glyph*> glyphs, int spacing)
{
    printf("Creating Strip %s...\n", filename);

    int hpitch = 0;
    int vpitch = 0;

    for (int i=0; i<glyphs.GetItemCount(); i++)
	{
	    hpitch += glyphs.Get(i)->hpitch + spacing * 2;
	    if (glyphs.Get(i)->vpitch + spacing * 2 > vpitch)
		{
		    vpitch = glyphs.Get(i)->vpitch + spacing * 2;
		}
	}

    Image image(hpitch, vpitch);
    int xp = 0;

    for (int i = 0; i < glyphs.GetItemCount(); i++)
    {
	    Glyph* glyph = glyphs.Get(i);

	    for (int y = 0; y < glyph->vpitch; y++)
		{
		    for (int x = 0; x < glyph->hpitch; x++)
			{
			    unsigned int c = 0x00ffffff | (glyph->data[x + y * glyph->hpitch] << 24);
			    image.SetPixel(spacing + xp + x, spacing + y, c);
            }
		}

	    glyph->xoffset -= spacing;
	    glyph->yoffset -= spacing;
	    glyph->x1 = xp;
	    glyph->x2 = xp + glyph->hpitch - 1 + spacing * 2;
	    glyph->y1 = 0;
	    glyph->y2 = glyph->vpitch - 1 + spacing * 2;

	    xp += glyph->hpitch + spacing * 2;
	}

    char* imageFilename = new char[StrLen(filename) + 5];
    
    StrCpy(imageFilename, filename);
    StrCat(imageFilename, ".tga");
    
    image.Save<ImageFormat_TGA>(imageFilename);

    delete [] imageFilename;
}
