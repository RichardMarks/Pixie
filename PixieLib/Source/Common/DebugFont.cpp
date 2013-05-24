//*** DebugFont.cpp ***

#include "DebugFont.h"
#include "Bitmap.h"

// Forward declare
extern unsigned int DebugFontData[];


//*** Blit Bitmap ***

void DebugFont::Blit(Bitmap& bitmap, int x, int y, const char* text, unsigned short color)
	{
	Blit<unsigned short>(bitmap.GetColorData(),bitmap.GetHPitch(),bitmap.GetVPitch(),x-bitmap.GetHOffset(),y-bitmap.GetVOffset(),text,color);
	}


//*** Blit 8-bit ***

void DebugFont::Blit(unsigned char* colorData, int hPitch, int vPitch, int x, int y, const char* text, unsigned char color )
	{
	Blit<unsigned char>(colorData,hPitch,vPitch,x,y,text,color);
	}


//*** Blit 16-bit ***

void DebugFont::Blit(unsigned short* colorData, int hPitch, int vPitch, int x, int y, const char* text, unsigned short color )
	{
	Blit<unsigned short>(colorData,hPitch,vPitch,x,y,text,color);
	}


//*** Blit 32-bit ***

void DebugFont::Blit(unsigned int* colorData, int hPitch, int vPitch, int x, int y, const char* text, unsigned int color )
	{
	Blit<unsigned int>(colorData,hPitch,vPitch,x,y,text,color);
	}


//*** Blit ***

template <typename TYPE> 
void DebugFont::Blit(TYPE* colorData, int hPitch, int vPitch, int x, int y, const char* text, TYPE color)
	{
	// Don't draw anything if we don't have a valid string
	if (!text)
		{
		return;
		}

	// Draw text
	int xp=x;
	while (*text)
		{
		char c=*text;
		text++;
		if (c>=32 && c<=192)
			{
			// The font starts with space (ascii 32)
			c-=32;

			// Source coordinates
			int sx=((c%14)*9);
			int sy=((c/14)*16);

			// Destination coordinates
			int dx=xp;
			int dy=y;

			// Render
			for (int y=0; y<16; y++)
				{
				for (int x=0; x<8; x++)
					{
					// Calculate the character location within the font data
					int v=(sx+x)/32;
					int u=(sx+x)-(v*32);

					// Get the font-data bit-mask for the current pixel row
					unsigned int h=DebugFontData[v+(sy+y)*4];
					
					// Check if the bit is set for the current pixel column
					if (h & (1<<u))
						{
						// Calculate current target position
						int xp=dx+x;
						int yp=dy+y;

						// Check if the current target position is inside the bitmap
						if (xp>=0 && xp<hPitch && yp>=0 && yp<vPitch)
							{
							// Write pixel color
							colorData[xp+yp*hPitch]=color;
							}
						}
					}
				}	

			// Increase draw position
			xp=xp+8;
			}
		}
	}


//*** DebugFontData ***

unsigned int DebugFontData[] = 
	{
	0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000180,0x00000000,0x00000000,0x00000000,0x0001c180,0x00000000,0x00000000,0x61983000,0x070363c3,0x0018300c,0x00000000,
	0x61987800,0x0d8b6663,0x0030180c,0x00000000,0xf1987800,0x0d8dc067,0xb030180c,0x000000c1,0x60007800,0x070600c3,0xe0600c00,0x000000c0,0x60003000,0x01830183,0xf8600c00,0x0fc003f3,
	0x60003000,0x3d818303,0xe0600c00,0x000000c0,0xf0000000,0x198ec607,0xb0600c00,0x000000c1,0x60003000,0x199b4663,0x00600c00,0x00038000,0x60003000,0x371b03c3,0x00301800,0x00038000,
	0x00000000,0x000e0180,0x00301800,0x00030000,0x00000000,0x00000180,0x00183000,0x00018000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x81e0c000,0x030783c1,0xf0fc383f,0x000001e0,
	0xc330c000,0x030cc661,0x98c01803,0x00000331,0xf3b06000,0x1b0cc661,0x98600c03,0x07038331,0x83b06000,0x1b0c0601,0xb8603e03,0x07038331,0x83303000,0x1b070301,0xf030661f,0x00000330,
	0x83703000,0x198c0181,0xd8306630,0x000003e1,0x83701800,0x3f8cc0c1,0x98186630,0x00000181,0x83301838,0x180cc061,0x98186618,0x070380c1,0x81e00c38,0x180787e1,0xf0183c0f,0x070380e0,
	0x00000c00,0x00000000,0x00000000,0x06000000,0x00000000,0x00000000,0x00000000,0x03000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0xe0180060,0x0f8307e1,0xf8fc1e1e,0x078661e1,
	0x30300030,0x19878c33,0x180c3633,0x03066330,0x30600018,0x198ccc33,0x180c6633,0x03066330,0x80c0fc0c,0x198ccf31,0x180c6603,0x03066030,0xc1800006,0x0f8ccdb0,0xf87c6603,0x0307e030,
	0xc0c0fc0c,0x198fcdb0,0x180c6603,0x030663b0,0x00600018,0x198ccf30,0x180c6633,0x03066330,0xc0300030,0x198cc030,0x180c3633,0x03066330,0xc0180060,0x0f8ccfe0,0x18fc1e1e,0x078663e0,
	0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x3018cc60,0x0f878c66,0xf8783e1e,0x18c66331,
	0x3018cc60,0x198ccc66,0x60cc6633,0x18c66330,0x70186c60,0x198ccce7,0x600c6633,0x18c66330,0xb0186c60,0x198ccde6,0x60186633,0x1ac66330,0xb0183c60,0x0f8ccf66,0x60303e33,0x1ac66330,
	0xb0186c60,0x018cce66,0x60603633,0x1ac66330,0x30186c66,0x018ccc66,0x60c06633,0x0d866330,0x3018cc66,0x018ccc66,0x60cc6633,0x0d83c330,0x31f8cc3c,0x01878c66,0x6078661e,0x0d8181e0,
	0x00000000,0x00000000,0x00000018,0x00000000,0x00000000,0x00000000,0x00000030,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x06000000,0x00001c00,0x00000000,0x00000000,0x0f000000,0x00001800,0x00000000,0xe1f8cc66,0x19878061,0x18003000,0x00060000,
	0x6180cc66,0x00060060,0x18000000,0x00060000,0x6180cc2c,0x000600c0,0xf8780000,0x0787c1e0,0x60c0cc18,0x000600c0,0x98c00000,0x0cc66331,0x60607818,0x00060180,0x98c00000,0x0cc66031,
	0x60303034,0x00060180,0x98f80000,0x0fc66031,0x60183066,0x00060300,0x98cc0000,0x00c66031,0x60183066,0x00060300,0x98cc0000,0x00c66331,0x61f83066,0x00060600,0xf8f80000,0x0787c1e0,
	0x60000000,0x00060600,0x00000000,0x00000000,0x60000000,0x00060000,0x00000000,0x00000000,0xe0000000,0x80078001,0x0000007f,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0xc0000000,0x00000300,0x00000000,0x00000000,0xc0180078,0x0780c300,0x00000000,0x00000000,
	0x0018000c,0x0600c000,0x00000000,0x00000000,0xf0f8f80c,0x060cc3c0,0xf8783e3f,0x0f8663e0,0xc198cc0c,0x060cc300,0x98cc666b,0x00c76331,0xc198cc7e,0x0606c300,0x98cc666b,0x00c0e331,
	0xc198cc0c,0x0603c300,0x98cc666b,0x07806331,0xc198cc0c,0x0606c300,0x98cc666b,0x0c006331,0xc198cc0c,0x060cc300,0x98cc666b,0x0c006331,0xf198f80c,0x1f8cc303,0xf8786663,0x07c063e0,
	0x0000c000,0x00000300,0x18000000,0x00000300,0x0000c000,0x00000300,0x18000000,0x00000300,0x00007c00,0x000001e0,0x18000000,0x00000300,0x00000000,0x00000000,0x00000000,0x00000000,
	0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x0000000c,0x00000000,0x38181818,0x000003f2,
	0x0000000c,0x00000000,0x6c30180c,0x000003f3,0x3198cc7e,0x1f8cc666,0xc430180c,0x000003f1,0xb198cc0c,0x180cc666,0x0030180c,0x000003f0,0xb198cc0c,0x0c0cc3c6,0x00601806,0x000003f0,
	0xb198cc0c,0x060cc186,0x00c01803,0x000003f0,0xb198cc0c,0x030cc3c6,0x00601806,0x000003f0,0x60f0cc0c,0x018cc663,0x0030180c,0x000003f0,0x6060f878,0x1f878663,0x0030180c,0x000003f0,
	0x00000000,0x00060000,0x0030180c,0x00000000,0x00000000,0x00030000,0x00181818,0x00000000,0x00000000,0x0001e000,0x00001800,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	};
