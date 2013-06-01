/**
 * \file	TColor
 * \author	Mattias Gustavsson	
 */

#ifndef __TColor_H__
#define __TColor_H__

// TColorName
enum TColor
	{
	TBlack,
	TBlue,
	TGreen,
	TCyan,
	TRed,
	TMagenta,
	TBrown,
	TLightGrey,
	TGrey,
	TLightBlue,
	TLightGreen,
	TLightCyan,
	TLightRed,
	TLightMagenta,
	TYellow,
	TWhite,
	TUndefined,
	TColorCount = TUndefined
	};

//*** TColorToRGB565 ***

static inline unsigned short TColorToRGB565(TColor color)
	{
	static unsigned short colors[TColorCount] = 
		{
		0x0000,0x1093,0x14c2,0x14d3,0x9882,0x9893,0x9aa2,0xad55,
		0x52aa,0x633d,0x676d,0x677d,0xeb2c,0xeb3d,0xef6c,0xffff,
		};
	
	unsigned int index = (unsigned int)color;
	if (index>=TColorCount)
		{
		index = 0;
		}

	return colors[index];
	}

#endif /* __TColor_H__ */
