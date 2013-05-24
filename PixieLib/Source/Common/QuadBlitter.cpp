//*** QuadBlitter.cpp ***

#include "QuadBlitter.h"
#include "ColorHelper.h"
#include "StandardLibrary.h"
#include "Debug.h"


QuadBlitter::EdgeBuffer QuadBlitter::edgeBuffer_;

/*	// INTERPOLATION
	int offset=(u0>>16)+sourceHPitch*(v0>>16);
	unsigned short c1=sourceColor[offset];
	unsigned short c2=sourceColor[offset+1];
	unsigned short c3=sourceColor[offset+sourceHPitch];
	unsigned short c4=sourceColor[offset+sourceHPitch+1];
	unsigned char uf=(unsigned char)((u0&0xffff)>>8);
	unsigned char vf=(unsigned char)((v0&0xffff)>>8);
	unsigned short fc0=AlphaBlend16(c1,c2,uf);
	unsigned short fc1=AlphaBlend16(c3,c4,uf);
	unsigned short fcx=AlphaBlend16(fc0,fc1,vf);
	*pdata=fcx;
*/


// DstColor fylls med fillColor
void QuadBlitter::Blit(unsigned short fillColor, unsigned short* targetColor, int targetHPitch, int targetVPitch, int targetX1, int targetY1, int targetX2, int targetY2, int targetX3, int targetY3, int targetX4, int targetY4)
	{
	// *** START EDGE SCANNING ***

	// To avoid massive amounts of code duplication, the edge scanning code have been
	// written to cope with multiple scenarios, with features determined by #defines

	#define EDGE_TYPE EdgeX
	
	#include "QuadBlitter_scanning.inl"
	
	#undef EDGE_TYPE

	// *** END EDGE SCANNING ***


	// Draw rows
	unsigned short* data=targetColor+p0y*targetHPitch;
	EdgeX* edgePtr=edgeBuffer;
	for (int y=p0y; y<p3y; y++)
		{
		int xstart=(*edgePtr++).x;
		int width=(*edgePtr++).x-xstart;

		unsigned short* pdata=data+xstart;
		for (int x=0; x<width; x++)
			{
			*pdata=fillColor;
			pdata++;
			}

		// Advance to next rows
		data+=targetHPitch;
		}

	}

// SrcColor kopieras till DstColor
void QuadBlitter::Blit(unsigned short* sourceColor, int sourceHPitch, int sourceVPitch, unsigned short* targetColor, int targetHPitch, int targetVPitch, int sourceX1, int sourceY1, int sourceX2, int sourceY2, int sourceX3, int sourceY3, int sourceX4, int sourceY4, int targetX1, int targetY1, int targetX2, int targetY2, int targetX3, int targetY3, int targetX4, int targetY4)
	{
	sourceX2++;
	sourceX3++;
	sourceY3++;
	sourceY4++;

	// *** START EDGE SCANNING ***

	// To avoid massive amounts of code duplication, the edge scanning code have been
	// written to cope with multiple scenarios, with features determined by #defines

	#define EDGE_TYPE EdgeXUV
	#define USE_UV
	
	#include "QuadBlitter_scanning.inl"
	
	#undef USE_UV
	#undef EDGE_TYPE

	// *** END EDGE SCANNING ***


	// Draw rows
	unsigned short* data=targetColor+p0y*targetHPitch;
	EdgeXUV* edgePtr=static_cast<EdgeXUV*>(edgeBuffer_.buffer_);
	for (int y=p0y; y<p3y; y++)
		{
		int xstart=(*edgePtr).x;
		int u0=(*edgePtr).u;
		int v0=(*edgePtr).v;
		edgePtr++;

		int xend=(*edgePtr).x;
		int u1=(*edgePtr).u;
		int v1=(*edgePtr).v;
		edgePtr++;

		
		int width=xend-xstart+1;

		int stepU=(u1-u0)/width;
		int stepV=(v1-v0)/width;

		unsigned short* pdata=data+xstart;
		for (int x=0; x<width; x++)
			{
			*pdata=sourceColor[(u0>>16)+sourceHPitch*(v0>>16)];
			pdata++;
			u0+=stepU;
			v0+=stepV;
			}

		// Advance to next rows
		data+=targetHPitch;
		}

	}

// SrcColor blendas (SrcAlpha) med DstColor
void QuadBlitter::Blit(unsigned short* sourceColor, unsigned char* sourceAlpha, int sourceHPitch, int sourceVPitch, unsigned short* targetColor, int targetHPitch, int targetVPitch, int sourceX1, int sourceY1, int sourceX2, int sourceY2, int sourceX3, int sourceY3, int sourceX4, int sourceY4, int targetX1, int targetY1, int targetX2, int targetY2, int targetX3, int targetY3, int targetX4, int targetY4)
	{
	sourceX2++;
	sourceX3++;
	sourceY3++;
	sourceY4++;

	// *** START EDGE SCANNING ***

	// To avoid massive amounts of code duplication, the edge scanning code have been
	// written to cope with multiple scenarios, with features determined by #defines

	#define EDGE_TYPE EdgeXUV
	#define USE_UV
	
	#include "QuadBlitter_scanning.inl"
	
	#undef USE_UV
	#undef EDGE_TYPE

	// *** END EDGE SCANNING ***


	// Draw rows
	unsigned short* data=targetColor+p0y*targetHPitch;
	EdgeXUV* edgePtr=static_cast<EdgeXUV*>(edgeBuffer_.buffer_);
	for (int y=p0y; y<p3y; y++)
		{
		int xstart=(*edgePtr).x;
		int u0=(*edgePtr).u;
		int v0=(*edgePtr).v;
		edgePtr++;

		int xend=(*edgePtr).x;
		int u1=(*edgePtr).u;
		int v1=(*edgePtr).v;
		edgePtr++;

		
		int width=xend-xstart+1;

		int stepU=(u1-u0)/width;
		int stepV=(v1-v0)/width;

		unsigned short* pdata=data+xstart;
		for (int x=0; x<width; x++)
			{
			int offset=(u0>>16)+sourceHPitch*(v0>>16);
			unsigned char srcalpha=sourceAlpha[offset];
			if (srcalpha>0)
				{
				*pdata=AlphaBlend16(*pdata,sourceColor[offset],srcalpha);
				}

			pdata++;
			u0+=stepU;
			v0+=stepV;
			}

		// Advance to next rows
		data+=targetHPitch;
		}

	}


// SrcColor blendas (SrcAlpha*BlitAlpha) med DstColor
void QuadBlitter::Blit(unsigned char alpha, unsigned short* sourceColor, unsigned char* sourceAlpha, int sourceHPitch, int sourceVPitch, unsigned short* targetColor, int targetHPitch, int targetVPitch, int sourceX1, int sourceY1, int sourceX2, int sourceY2, int sourceX3, int sourceY3, int sourceX4, int sourceY4, int targetX1, int targetY1, int targetX2, int targetY2, int targetX3, int targetY3, int targetX4, int targetY4)
	{
	sourceX2++;
	sourceX3++;
	sourceY3++;
	sourceY4++;

	// *** START EDGE SCANNING ***

	// To avoid massive amounts of code duplication, the edge scanning code have been
	// written to cope with multiple scenarios, with features determined by #defines

	#define EDGE_TYPE EdgeXUV
	#define USE_UV
	
	#include "QuadBlitter_scanning.inl"
	
	#undef USE_UV
	#undef EDGE_TYPE

	// *** END EDGE SCANNING ***


	// Draw rows
	unsigned short* data=targetColor+p0y*targetHPitch;
	EdgeXUV* edgePtr=static_cast<EdgeXUV*>(edgeBuffer_.buffer_);
	for (int y=p0y; y<p3y; y++)
		{
		int xstart=(*edgePtr).x;
		int u0=(*edgePtr).u;
		int v0=(*edgePtr).v;
		edgePtr++;

		int xend=(*edgePtr).x;
		int u1=(*edgePtr).u;
		int v1=(*edgePtr).v;
		edgePtr++;

		
		int width=xend-xstart+1;

		int stepU=(u1-u0)/width;
		int stepV=(v1-v0)/width;

		unsigned short* pdata=data+xstart;
		for (int x=0; x<width; x++)
			{
			int offset=(u0>>16)+sourceHPitch*(v0>>16);
			unsigned char srcalpha=sourceAlpha[offset];
			if (srcalpha>0)
				{
				unsigned char outalpha=(srcalpha*alpha)>>8;
				*pdata=AlphaBlend16(*pdata,sourceColor[offset],outalpha);
				}

			pdata++;
			u0+=stepU;
			v0+=stepV;
			}

		// Advance to next rows
		data+=targetHPitch;
		}

	}
