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

void QuadBlitter::Blit(unsigned short fillColor, unsigned short* targetColor, int targetHPitch, int targetVPitch, int targetX1, int targetY1, int targetX2, int targetY2, int targetX3, int targetY3, int targetX4, int targetY4, int clipX1, int clipY1, int clipX2, int clipY2)
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

	if ((p0y<clipY1 && p3y<clipY1) || (p0y>clipY2 && p3y>clipY2))
		{
		return;
		}

	if (p0y<clipY1)
		{
		int d=Abs(p0y-clipY1);
		edgePtr+=2*d;
		data+=targetHPitch*d;
		p0y=clipY1;
		}
	if (p3y>clipY2)
		{
		p3y=clipY2;
		}

	for (int y=p0y; y<p3y; y++)
		{
		int xstart=(*edgePtr++).x;
		int xend=(*edgePtr++).x;
		int width=xend-xstart+1;

		if (width==0 || (xstart<clipX1 && xend<clipX1) || (xstart>clipX2 && xend>clipX2))
			{
			data+=targetHPitch;
			continue;
			}

		unsigned short* pdata=data+xstart;
		if (xstart<clipX1)
			{
			int d=Abs(xstart-clipX1);
			width-=d;
			pdata+=d;
			}
		if (xend>clipX2)
			{
			int d=Abs(xend-clipX2);
			width-=d;
			}

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







/////////////////////// CLIPPING /////////////////////////////






void QuadBlitter::Blit(unsigned short* sourceColor, unsigned char* sourceAlpha, int sourceHPitch, int sourceVPitch, unsigned short* targetColor, int targetHPitch, int targetVPitch, int sourceX1, int sourceY1, int sourceX2, int sourceY2, int sourceX3, int sourceY3, int sourceX4, int sourceY4, int targetX1, int targetY1, int targetX2, int targetY2, int targetX3, int targetY3, int targetX4, int targetY4, int clipX1, int clipY1, int clipX2, int clipY2)
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
	if ((p0y<clipY1 && p3y<clipY1) || (p0y>clipY2 && p3y>clipY2))
		{
		return;
		}

	if (p0y<clipY1)
		{
		int d=Abs(p0y-clipY1);
		edgePtr+=2*d;
		data+=targetHPitch*d;
		p0y=clipY1;
		}
	if (p3y>clipY2)
		{
		p3y=clipY2;
		}

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


		if (width==0 || (xstart<clipX1 && xend<clipX1) || (xstart>clipX2 && xend>clipX2))
			{
			data+=targetHPitch;
			continue;
			}

		unsigned short* pdata=data+xstart;

		int stepU=(u1-u0)/width;
		int stepV=(v1-v0)/width;

		if (xstart<clipX1)
			{
			int d=Abs(xstart-clipX1);
			width-=d;
			pdata+=d;
			u0+=d*stepU;
			v0+=d*stepV;
			}
		if (xend>clipX2)
			{
			int d=Abs(xend-clipX2);
			width-=d;
			}

		for (int x=0; x<width; x++)
			{
			int offset=(u0>>16)+sourceHPitch*(v0>>16);
			unsigned char srcalpha=sourceAlpha[offset];
			*pdata=AlphaBlend16(*pdata,sourceColor[offset],srcalpha);
				
				// INTERPOLATION
/*				int offset=(u0>>16)+sourceHPitch*(v0>>16);				
				unsigned short c1=sourceColor[offset];
				unsigned short c2=sourceColor[offset+1];
				unsigned short c3=sourceColor[offset+sourceHPitch];
				unsigned short c4=sourceColor[offset+sourceHPitch+1];
				unsigned char uf=(unsigned char)((u0&0xffff)>>8);
				unsigned char vf=(unsigned char)((v0&0xffff)>>8);
				unsigned short fc0=AlphaBlend16(c1,c2,uf);
				unsigned short fc1=AlphaBlend16(c3,c4,uf);
				unsigned short fcx=AlphaBlend16(fc0,fc1,vf);
				unsigned short a1=sourceAlpha[offset];
				unsigned short a2=sourceAlpha[offset+1];
				unsigned short a3=sourceAlpha[offset+sourceHPitch];
				unsigned short a4=sourceAlpha[offset+sourceHPitch+1];
				unsigned char fa0=(unsigned char)((a1*(255-uf)+a2*uf)>>8);
				unsigned char fa1=(unsigned char)((a3*(255-uf)+a4*uf)>>8);
				unsigned char fax=(unsigned char)((fa0*(255-vf)+fa1*vf)>>8);
				*pdata=AlphaBlend16(*pdata,fcx,fax);
*/			


			pdata++;
			u0+=stepU;
			v0+=stepV;
			}

		// Advance to next rows
		data+=targetHPitch;
		}

	}

