//*** CopperRLE8.cpp ***

#include "CopperRLE8.h"
#include "ColorHelper.h"
#include "StandardLibrary.h"


//*** Opaque_Unclipped_Unmasked ***

void CopperRLE8::Opaque_Unclipped_Unmasked(unsigned char* opaqueData, int activeWidth, int activeHeight, unsigned short* palette, unsigned short* colorData, unsigned char* alphaData, int backBufferDelta,int x, int y)
	{
	unsigned char* dataRLE=opaqueData;
	for (int yi=0; yi<activeHeight; ++yi)
		{
		int xi=0;
		while (xi<activeWidth)
			{
			// Get run length
			int len=*dataRLE;
			++dataRLE;

			RunLength_Opaque_Unclipped_Unmasked(len,colorData,alphaData,&dataRLE,palette);
			len&=0x7f;
			colorData+=len;
			alphaData+=len;

			xi+=len;
			}

		colorData+=backBufferDelta;
		alphaData+=backBufferDelta;
		}
	}


//*** Opaque_Unclipped_Masked ***

void CopperRLE8::Opaque_Unclipped_Masked(unsigned char* opaqueData, int activeWidth, int activeHeight, unsigned short* palette, unsigned short* colorData, unsigned char* alphaData, int backBufferDelta,int x, int y)
	{
	unsigned char* dataRLE=opaqueData;
	for (int yi=0; yi<activeHeight; ++yi)
		{
		int xi=0;
		while (xi<activeWidth)
			{
			// Get run length
			int len=*dataRLE;
			++dataRLE;

			RunLength_Opaque_Unclipped_Masked(len,colorData,alphaData,&dataRLE,palette);
			len&=0x7f;
			colorData+=len;
			alphaData+=len;

			xi+=len;
			}
		colorData+=backBufferDelta;
		alphaData+=backBufferDelta;
		}
	}	
	

//*** Opaque_Clipped_Unmasked ***

void CopperRLE8::Opaque_Clipped_Unmasked(unsigned char* opaqueData, int activeWidth, int activeHeight, unsigned short* palette, unsigned short* colorData, unsigned char* alphaData, int backBufferDelta,int x, int y, int xStart, int yStart, int xEnd, int yEnd)
	{	
	unsigned char* dataRLE=opaqueData;
	
	// Skip clipped rows
	for (int i=0; i<yStart; i++)
		{
		int xi=0;
		while (xi<activeWidth)
			{
			// Get run length
			int len=*dataRLE;
			++dataRLE;

			IgnoreOpaque(len,&dataRLE);
			len&=0x7f;

			xi+=len;
			}
		}
		
	colorData+=yStart*(backBufferDelta+activeWidth);
	alphaData+=yStart*(backBufferDelta+activeWidth);
	
	for (int yi=yStart; yi<(activeHeight-yEnd); yi++)
		{
		int xi=0;
		while (xi<activeWidth)
			{
			// Get run length
			int len=*dataRLE;
			++dataRLE;
			
			// Are we on the edge?
			int runLength=(len&0x7f);
			if (xi<xStart || (xi+runLength)>(activeWidth-xEnd))
				{
				// yes, on the edge, but is the whole segment off screen?
				if ((xi+runLength)<xStart || xi>(activeWidth-xEnd))
					{
					// Yes, whole segment is out of view, so ignore
					IgnoreOpaque(len,&dataRLE);
					}	
				else
					{
					// No, only part of the segment is out of view, so need to draw with clipping
					int clipStart=xStart-xi;
					int clipEnd=(activeWidth-xEnd)-xi;
					RunLength_Opaque_Clipped_Masked(len,colorData,alphaData,&dataRLE,palette,clipStart,clipEnd); 
					// Yes, we use the masked version here, because it doesn't give enough speed benefit to have a special 
					// unmasked version of the clipped one
					}
				}
			else // nope, not near edge, so just barge ahead
				{
				RunLength_Opaque_Unclipped_Unmasked(len,colorData,alphaData,&dataRLE,palette);
				}

			len&=0x7f;
			colorData+=len;
			alphaData+=len;

			xi+=len;
			}
		colorData+=backBufferDelta;
		alphaData+=backBufferDelta;
		}
	}	


//*** Opaque_Clipped_Masked ***

void CopperRLE8::Opaque_Clipped_Masked(unsigned char* opaqueData, int activeWidth, int activeHeight, unsigned short* palette, unsigned short* colorData, unsigned char* alphaData, int backBufferDelta,int x, int y, int xStart, int yStart, int xEnd, int yEnd)
	{	
	if (yStart>activeHeight-yEnd)
		return;
	if (xStart>activeWidth-xEnd)
		return;
	unsigned char* dataRLE=opaqueData;
	
	// Skip clipped rows
	for (int i=0; i<yStart; i++)
		{
		int xi=0;
		while (xi<activeWidth)
			{
			// Get run length
			int len=*dataRLE;
			++dataRLE;

			IgnoreOpaque(len,&dataRLE);
			len&=0x7f;

			xi+=len;
			}
		}
		
	colorData+=yStart*(backBufferDelta+activeWidth);
	alphaData+=yStart*(backBufferDelta+activeWidth);
	
	for (int yi=yStart; yi<(activeHeight-yEnd); ++yi)
		{
		int xi=0;
		while (xi<activeWidth)
			{
			// Get run length
			int len=*dataRLE;
			++dataRLE;
			
			// Are we on the edge?
			int runLength=(len&0x7f);
			if (xi<xStart || (xi+runLength)>(activeWidth-xEnd))
				{
				// yes, on the edge, but is the whole segment off screen?
				if ((xi+runLength)<xStart || xi>(activeWidth-xEnd))
					{
					// Yes, whole segment is out of view, so ignore
					IgnoreOpaque(len,&dataRLE);
					}	
				else
					{
					// No, only part of the segment is out of view, so need to draw with clipping
					int clipStart=xStart-xi;
					int clipEnd=(activeWidth-xEnd)-xi;
			
					RunLength_Opaque_Clipped_Masked(len,colorData, alphaData,&dataRLE,palette,clipStart,clipEnd);
					}
				}
			else // nope, not near edge, so just barge ahead
				{
				RunLength_Opaque_Unclipped_Masked(len,colorData,alphaData,&dataRLE,palette);
				}

			len&=0x7f;
			colorData+=len;
			alphaData+=len;

			xi+=len;
			}
		colorData+=backBufferDelta;
		alphaData+=backBufferDelta;
		}
	}	
	


//*** Alpha_Unclipped ***
	
void CopperRLE8::Alpha_Unclipped(unsigned char* alphaRLEData, int activeWidth, int activeHeight, unsigned short* palette, unsigned short* colorData, unsigned char* alphaData, int backBufferDelta,int x, int y)
	{	
	unsigned char* dataRLE=alphaRLEData;
	for (int yi=0; yi<activeHeight; ++yi)
		{
		int xi=0;
		while (xi<activeWidth)
			{
			// Get run length
			unsigned int len=*dataRLE;
			++dataRLE;
			if (len&0x80)	
				{
				len&=0x7f;
				// Unique values
				for (unsigned int i=0; i<len; ++i)
					{
					unsigned char alpha=*dataRLE;
					++dataRLE;
					*colorData=palette[*dataRLE];
					*alphaData=alpha;
					++dataRLE;
					++colorData;
					++alphaData;
					}
				}
			else
				{
				unsigned char alpha=*dataRLE;
				++dataRLE;
				if (alpha!=0)
					{
					unsigned short color=palette[*dataRLE];
					++dataRLE;
					
					for (unsigned int i=0; i<len; ++i)
						{
						*colorData=color;
						*alphaData=alpha;
						++colorData;
						++alphaData;
						}
					}
				else
					{
					++dataRLE;
					colorData+=len;
					alphaData+=len;
					}
				}
			xi+=len;
			}
		colorData+=backBufferDelta;
		alphaData+=backBufferDelta;
		}
	}
	


//*** Alpha_Clipped ***
	
void CopperRLE8::Alpha_Clipped(unsigned char* alphaRLEData, int activeWidth, int activeHeight, unsigned short* palette, unsigned short* colorData, unsigned char* alphaData, int backBufferDelta,int x, int y, int xStart, int yStart, int xEnd, int yEnd)
	{	
	unsigned char* dataRLE=alphaRLEData;
	
	// Skip clipped rows
	for (int i=0; i<yStart; i++)
		{
		int xi=0;
		while (xi<activeWidth)
			{
			// Get run length
			int len=*dataRLE;
			++dataRLE;

			IgnoreAlpha(len,&dataRLE);
			len&=0x7f;

			xi+=len;
			}
		}
		
	colorData+=yStart*(backBufferDelta+activeWidth);
	alphaData+=yStart*(backBufferDelta+activeWidth);
	
	for (int yi=yStart; yi<activeHeight-yEnd; ++yi)
		{
		int xi=0;
		while (xi<activeWidth)
			{
			// Get run length
			int len=*dataRLE;
			++dataRLE;
			if (len&0x80)	
				{
				len&=0x7f;
				// Unique values
				for (int i=0; i<len; ++i)
					{
					unsigned char alpha=*dataRLE;
					++dataRLE;
					if (alpha!=0 && (xi+i)>=xStart && (xi+i)<=(activeWidth-xEnd))
						{
						*colorData=palette[*dataRLE];
						*alphaData=alpha;
						}
					++dataRLE;
					++colorData;
					++alphaData;
					}
				}
			else
				{
				unsigned char alpha=*dataRLE;
				++dataRLE;
				if (alpha!=0)
					{
					unsigned short color=palette[*dataRLE];
					++dataRLE;
					
					for (int i=0; i<len; ++i)
						{
						if ((xi+i)>=xStart && (xi+i)<=(activeWidth-xEnd))
							{
							*colorData=color;
							*alphaData=alpha;
							}
						++colorData;
						++alphaData;
						}
					}
				else
					{
					++dataRLE;
					colorData+=len;
					alphaData+=len;
					}
				}
			xi+=len;
			}
		colorData+=backBufferDelta;
		alphaData+=backBufferDelta;
		}
	}



//*** RunLength_Opaque_Unclipped_Unmasked ***

void CopperRLE8::RunLength_Opaque_Unclipped_Unmasked(int len,unsigned short* colorData, unsigned char* alphaData, unsigned char** source, unsigned short* palette)
	{
	unsigned char* dataRLE=*source;
	if (len&0x80)	
		{
		len&=0x7f;
		// Unique values
		for (int i=0; i<len; ++i)
			{
			*colorData=palette[*dataRLE];
			*alphaData=255;
			++colorData;
			++alphaData;
			++(dataRLE);
			}
		}
	else
		{
		// Run of values
		#ifdef _DEBUG
			Fill(colorData,palette[*dataRLE],len);
			AlphaFill(alphaData,len);
		#else
			BurstFill(colorData,palette[*dataRLE],len);
			AlphaBurstFill(alphaData,len);
		#endif
		++(dataRLE);
		}
	*source=dataRLE;
	}	
	

//*** RunLength_Opaque_Clipped_Masked ***

void CopperRLE8::RunLength_Opaque_Clipped_Masked(int len,unsigned short* colorData, unsigned char* alphaData, unsigned char** source, unsigned short* palette, int clipStart, int clipEnd)
	{
	unsigned char* dataRLE=*source;
	int x=0;
	if (len&0x80)	
		{
		len&=0x7f;
		// Unique values
		for (int i=0; i<len; ++i)
			{
			if ((*dataRLE)<255 && x>=clipStart && x<=clipEnd)
				{
				*colorData=palette[*dataRLE];
				*alphaData=0xff;
				}
			++x;
			++colorData;
			++alphaData;
			++(dataRLE);
			}
		}
	else
		{	
		// Run of values
		int colorIndex=*dataRLE;
		if(colorIndex<255)
			{
			unsigned short color=palette[colorIndex];
			for (int i=0; i<len; ++i)
				{
				if (colorIndex<255 && x>=clipStart && x<=clipEnd)
					{
					*colorData=color;
					*alphaData=0xff;
					}
				++x;
				++colorData;
				++alphaData;
				}
			}
		else
			{
			x+=len;
			colorData+=len;
			alphaData+=len;
			}
			
		++(dataRLE);
		}
	*source=dataRLE;
	}	


//*** RunLength_Opaque_Unclipped_Masked ***

void CopperRLE8::RunLength_Opaque_Unclipped_Masked(int len,unsigned short* colorData, unsigned char* alphaData, unsigned char** source, unsigned short* palette)
	{
	unsigned char* dataRLE=*source;
	if (len&0x80)	
		{
		len&=0x7f;
		// Unique values
		for (int i=0; i<len; ++i)
			{
			if ((*dataRLE)<255)
				{
				*colorData=palette[*dataRLE];
				*alphaData=0xff;
				}
			++colorData;
			++alphaData;
			++(dataRLE);
			}
		}
	else
		{
		// Run of values
		if ((*dataRLE)<255)
			{
			#ifdef _DEBUG
				Fill(colorData,palette[*dataRLE],len);
				AlphaFill(alphaData,len);
			#else
				BurstFill(colorData,palette[*dataRLE],len);
				AlphaBurstFill(alphaData,len);
			#endif
			}
		++(dataRLE);
		}
	*source=dataRLE;
	}	
	

//*** Fill ***

void CopperRLE8::Fill(unsigned short* data, unsigned short color, int len)
	{
	for (int i=0; i<len; ++i)
		{
		*data=color;
		data++;
		}
	}



//*** BurstFill ***

// This is to make sure we fill on 4 byte alignment as much as possible
void CopperRLE8::BurstFill(unsigned short* data, unsigned short color, int len)
	{
	int len2=len;


#pragma warning (push)
#pragma warning( disable: 4311)
#ifdef _WIN32
	if (((int)data)!=(((int)data)&(0xfffffffc)))
#else
	if (((long long)data)!=(((long long)data)&(0xfffffffffffffffc)))
#endif 
#pragma warning (pop)
		{
		*data=color;
		++data;
		--len2;
		if (len2==1)
			{
			*data=color;
			++data;
			--len2;
			}
		}			
	if (len2)
		{
		unsigned int color32=color | (color<<16);
		int halflen=len2/2;
		unsigned int* data32=reinterpret_cast<unsigned int*>(data);
		for (int i=0; i<halflen; ++i)
			{
			*data32=color32;
			++data32;
			}
		data=reinterpret_cast<unsigned short*>(data32);
		if (len2-halflen*2)
			{
			*data=color;
			++data;
			}
		}
	}	
	


//*** AlphaFill ***

void CopperRLE8::AlphaFill(unsigned char*data, int len)
	{
	for (int i=0; i<len; ++i)
		{
		*data=0xff;
		data++;
		}
	}


//*** AlphaBurstFill ***

// This is to make sure we fill on 4 byte alignment as much as possible
void CopperRLE8::AlphaBurstFill(unsigned char* data, int len)
	{
	int len2=len;

#pragma warning (push)
#pragma warning( disable: 4311)
#ifdef _WIN32
	while (((int)data)!=(((int)data)&(0xfffffffc)))
#else
	while (((long long)data)!=(((long long)data)&(0xfffffffffffffffc)))
#endif 
#pragma warning (pop)

		{
		*data=0xff;
		++data;
		--len2;
		}			
	if (len2)
		{
		int quarterlen=len2/4;
		unsigned int* data32=reinterpret_cast<unsigned int*>(data);
		for (int i=0; i<quarterlen; ++i)
			{
			*data32=0xffffffff;
			++data32;
			}
		data=reinterpret_cast<unsigned char*>(data32);
		while (len2-quarterlen*4)
			{
			*data=0xff;
			++data;
			len2--;
			}
		}
	}	
	




//*** IgnoreOpaque ***

void CopperRLE8::IgnoreOpaque(int len, unsigned char** source)
	{
	unsigned char* dataRLE=*source;
	if (len&0x80)	
		{
		len&=0x7f;
		dataRLE+=len;
		}
	else
		{
		++dataRLE;
		}
	*source=dataRLE;
	}	
	

//*** IgnoreAlpha ***

void CopperRLE8::IgnoreAlpha(int len, unsigned char** source)
	{
	unsigned char* dataRLE=*source;
	if (len&0x80)	
		{
		len&=0x7f;
		dataRLE+=len*2;
		}
	else
		{
		dataRLE+=2;
		}
	*source=dataRLE;
	}	
	

