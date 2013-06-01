//*** MaxBlitterRLE8.inl ***

#include "MaxBlitterRLE8.h"
#include "ColorHelper.h"
#include "StandardLibrary.h"

	
//*** Opaque_Unclipped_Unmasked ***

void MaxBlitterRLE8::Opaque_Unclipped_Unmasked(unsigned char* opaqueData, int activeWidth, int activeHeight, unsigned short* palette, unsigned short* data, int backBufferDelta,int x, int y)
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

			RunLength_Opaque_Unclipped_Unmasked(len,data,&dataRLE,palette);
			len&=0x7f;
			data+=len;

			xi+=len;
			}

		data+=backBufferDelta;
		}
	}


//*** Opaque_Unclipped_Masked ***

void MaxBlitterRLE8::Opaque_Unclipped_Masked(unsigned char* opaqueData, int activeWidth, int activeHeight, unsigned short* palette, unsigned short* data, int backBufferDelta,int x, int y)
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

			RunLength_Opaque_Unclipped_Masked(len,data,&dataRLE,palette);
			len&=0x7f;
			data+=len;

			xi+=len;
			}
		data+=backBufferDelta;
		}
	}	

//*** Opaque_Unclipped_Unmasked_Transparent ***

void MaxBlitterRLE8::Opaque_Unclipped_Unmasked_Transparent(unsigned char* opaqueData, int activeWidth, int activeHeight, unsigned short* palette, unsigned short* data, int backBufferDelta,int x, int y, unsigned char alpha)
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

			RunLength_Opaque_Unclipped_Unmasked_Transparent(len,data,&dataRLE,palette,alpha);
			len&=0x7f;
			data+=len;

			xi+=len;
			}

		data+=backBufferDelta;
		}
	}


	

//*** Opaque_Unclipped_Masked_Transparent ***

void MaxBlitterRLE8::Opaque_Unclipped_Masked_Transparent(unsigned char* opaqueData, int activeWidth, int activeHeight, unsigned short* palette, unsigned short* data, int backBufferDelta,int x, int y, unsigned char alpha)
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

			RunLength_Opaque_Unclipped_MaskedTransparent(len,data,&dataRLE,palette,alpha);
			len&=0x7f;
			data+=len;

			xi+=len;
			}
		data+=backBufferDelta;
		}
	}	


//*** Opaque_Clipped_Unmasked ***

void MaxBlitterRLE8::Opaque_Clipped_Unmasked(unsigned char* opaqueData, int activeWidth, int activeHeight, unsigned short* palette, unsigned short* data, int backBufferDelta,int x, int y, int xStart, int yStart, int xEnd, int yEnd)
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
		
	data+=yStart*(backBufferDelta+activeWidth);
	
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
					RunLength_Opaque_Clipped_Masked(len,data,&dataRLE,palette,clipStart,clipEnd); 
					// Yes, we use the masked version here, because it doesn't give enough speed benefit to have a special 
					// unmasked version of the clipped one
					}
				}
			else // nope, not near edge, so just barge ahead
				{
				RunLength_Opaque_Unclipped_Unmasked(len,data,&dataRLE,palette);
				}

			len&=0x7f;
			data+=len;

			xi+=len;
			}
		data+=backBufferDelta;
		}
	}	


//*** Opaque_Clipped_Masked ***

void MaxBlitterRLE8::Opaque_Clipped_Masked(unsigned char* opaqueData, int activeWidth, int activeHeight, unsigned short* palette, unsigned short* data, int backBufferDelta,int x, int y, int xStart, int yStart, int xEnd, int yEnd)
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
		
	data+=yStart*(backBufferDelta+activeWidth);
	
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
			
					RunLength_Opaque_Clipped_Masked(len,data,&dataRLE,palette,clipStart,clipEnd);
					}
				}
			else // nope, not near edge, so just barge ahead
				{
				RunLength_Opaque_Unclipped_Masked(len,data,&dataRLE,palette);
				}

			len&=0x7f;
			data+=len;

			xi+=len;
			}
		data+=backBufferDelta;
		}
	}	
	

//*** Opaque_Clipped_Unmasked_Transparent ***

void MaxBlitterRLE8::Opaque_Clipped_Unmasked_Transparent(unsigned char* opaqueData, int activeWidth, int activeHeight, unsigned short* palette, unsigned short* data, int backBufferDelta,int x, int y, int xStart, int yStart, int xEnd, int yEnd,unsigned char alpha)
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
		
	data+=yStart*(backBufferDelta+activeWidth);
	
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
					RunLength_Opaque_Clipped_Masked_Transparent(len,data,&dataRLE,palette,clipStart,clipEnd,alpha); 
					// Yes, we use the masked version here, because it doesn't give enough speed benefit to 
					// have a special unmasked version of the clipped one
					}
				}
			else // nope, not near edge, so just barge ahead
				{
				RunLength_Opaque_Unclipped_Unmasked_Transparent(len,data,&dataRLE,palette,alpha);
				}

			len&=0x7f;
			data+=len;

			xi+=len;
			}
		data+=backBufferDelta;
		}
	}	


//*** Opaque_Clipped_Masked_Transparent ***

void MaxBlitterRLE8::Opaque_Clipped_Masked_Transparent(unsigned char* opaqueData, int activeWidth, int activeHeight, unsigned short* palette, unsigned short* data, int backBufferDelta,int x, int y, int xStart, int yStart, int xEnd, int yEnd, unsigned char alpha)
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
		
	data+=yStart*(backBufferDelta+activeWidth);
	
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

					RunLength_Opaque_Clipped_Masked_Transparent(len,data,&dataRLE,palette,clipStart,clipEnd,alpha);
					}
				}
			else // nope, not near edge, so just barge ahead
				{
				RunLength_Opaque_Unclipped_MaskedTransparent(len,data,&dataRLE,palette,alpha);
				}

			len&=0x7f;
			data+=len;

			xi+=len;
			}
		data+=backBufferDelta;
		}
	}	
	

	
//*** Alpha_Unclipped ***
	
void MaxBlitterRLE8::Alpha_Unclipped(unsigned char* alphaData, int activeWidth, int activeHeight, unsigned short* palette, unsigned short* data, int backBufferDelta,int x, int y)
	{	
	unsigned char* dataRLE=alphaData;
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
					if (alpha!=0)
						{
						*data=RGBModulate16(*data,palette[*dataRLE]);
						//Blend(data,palette[*dataRLE],alpha);
						}
					++dataRLE;
					++data;
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
						*data=RGBModulate16(*data,color);
						//Blend(data,color,alpha);
						++data;
						}
					}
				else
					{
					++dataRLE;
					data+=len;
					}
				}
			xi+=len;
			}
		data+=backBufferDelta;
		}
	}
	

//*** Alpha_Clipped ***
	
void MaxBlitterRLE8::Alpha_Clipped(unsigned char* alphaData, int activeWidth, int activeHeight, unsigned short* palette, unsigned short* data, int backBufferDelta,int x, int y, int xStart, int yStart, int xEnd, int yEnd)
	{	
	unsigned char* dataRLE=alphaData;
	
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
		
	data+=yStart*(backBufferDelta+activeWidth);
	
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
						*data=RGBModulate16(*data,palette[*dataRLE]);
						//Blend(data,palette[*dataRLE],alpha);
						}
					++dataRLE;
					++data;
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
							*data=RGBModulate16(*data,color);
							//Blend(data,color,alpha);
							}
						++data;
						}
					}
				else
					{
					++dataRLE;
					data+=len;
					}
				}
			xi+=len;
			}
		data+=backBufferDelta;
		}
	}


//*** Alpha_Unclipped_Transparent ***
	
void MaxBlitterRLE8::Alpha_Unclipped_Transparent(unsigned char* alphaData, int activeWidth, int activeHeight, unsigned short* palette, unsigned short* data, int backBufferDelta,int x, int y, unsigned char transparency)
	{	
	unsigned char* dataRLE=alphaData;
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
					alpha=(unsigned char)(((unsigned int)alpha*(unsigned int)transparency)>>8);
					++dataRLE;
					if (alpha!=0)
						{
						*data=RGBModulate16(*data,palette[*dataRLE]);
						//Blend(data,palette[*dataRLE],alpha);
						}
					++dataRLE;
					++data;
					}
				}
			else
				{
				unsigned char alpha=*dataRLE;
				alpha=(unsigned char)(((unsigned int)alpha*(unsigned int)transparency)>>8);
				++dataRLE;
				if (alpha!=0)
					{
					unsigned short color=palette[*dataRLE];
					++dataRLE;
					
					for (unsigned int i=0; i<len; ++i)
						{
						*data=RGBModulate16(*data,color);
						//Blend(data,color,alpha);
						++data;
						}
					}
				else
					{
					++dataRLE;
					data+=len;
					}
				}
			xi+=len;
			}
		data+=backBufferDelta;
		}
	}
	


//*** Alpha_Clipped_Transparent ***
	
void MaxBlitterRLE8::Alpha_Clipped_Transparent(unsigned char* alphaData, int activeWidth, int activeHeight, unsigned short* palette, unsigned short* data, int backBufferDelta,int x, int y, int xStart, int yStart, int xEnd, int yEnd, unsigned char transparency)
	{	
	unsigned char* dataRLE=alphaData;
	
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
		
	data+=yStart*(backBufferDelta+activeWidth);
	
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
					alpha=(unsigned char)(((unsigned int)alpha*(unsigned int)transparency)>>8);
					++dataRLE;
					if (alpha!=0 && (xi+i)>=xStart && (xi+i)<=(activeWidth-xEnd))
						{
						*data=RGBModulate16(*data,palette[*dataRLE]);
						//Blend(data,palette[*dataRLE],alpha);
						}
					++dataRLE;
					++data;
					}
				}
			else
				{
				unsigned char alpha=*dataRLE;
				alpha=(unsigned char)(((unsigned int)alpha*(unsigned int)transparency)>>8);
				++dataRLE;
				if (alpha!=0)
					{
					unsigned short color=palette[*dataRLE];
					++dataRLE;
					
					for (int i=0; i<len; ++i)
						{
						if ((xi+i)>=xStart && (xi+i)<=(activeWidth-xEnd))
							{
							*data=RGBModulate16(*data,color);
							//Blend(data,color,alpha);
							}
						++data;
						}
					}
				else
					{
					++dataRLE;
					data+=len;
					}
				}
			xi+=len;
			}
		data+=backBufferDelta;
		}
	}


//*** Fill ***

void MaxBlitterRLE8::Fill(unsigned short* data, unsigned short color, int len)
	{
	for (int i=0; i<len; ++i)
		{
		*data=RGBModulate16(*data, color);
		data++;
		}
	}


//*** Blend ***
/*
void MaxBlitterRLE8::Blend(unsigned short* destination, unsigned int color, unsigned int alpha)	
	{
	if (alpha==255)
		{
		*destination=(unsigned short)color;
		}
	else
		{
		unsigned int inva=255-alpha;
		unsigned int s=*destination;
		unsigned int sr=(s & 0xf800)>>11;
		unsigned int sg=(s & 0x7e0)>>5;
		unsigned int sb=(s & 0x1f);
		unsigned int dr=(color & 0xf800)>>11;
		unsigned int dg=(color & 0x7e0)>>5;
		unsigned int db=(color & 0x1f);
		unsigned int r=((sr*inva)+dr*alpha)>>8;
		unsigned int g=((sg*inva)+dg*alpha)>>8;
		unsigned int b=((sb*inva)+db*alpha)>>8;
		unsigned int c=(r<<11)|(g<<5)|b;
		*destination=(unsigned short)c;
		}
	}
*/

//*** FillTransparent ***

void MaxBlitterRLE8::FillTransparent(unsigned short* data, unsigned short color, int len, unsigned char alpha)
	{
	for (int i=0; i<len; ++i)
		{
		*data=RGBModulate16(*data,color);
//		Blend(data,color,alpha);
		data++;
		}
	}


//*** BurstFill ***

// This is to make sure we fill on 4 byte alignment as much as possible
void MaxBlitterRLE8::BurstFill(unsigned short* data, unsigned short color, int len)
	{
	while( len )
		{
		*data = RGBModulate16( *data, color );
		*data++;
		len--;
		}
	}	
	

//*** RunLength_Opaque_Unclipped_Unmasked ***

void MaxBlitterRLE8::RunLength_Opaque_Unclipped_Unmasked(int len,unsigned short* data, unsigned char** source, unsigned short* palette)
	{
	unsigned char* dataRLE=*source;
	if (len&0x80)	
		{
		len&=0x7f;
		// Unique values
		for (int i=0; i<len; ++i)
			{
			*data=RGBModulate16(*data, palette[*dataRLE]);
			++data;
			++(dataRLE);
			}
		}
	else
		{
		// Run of values
		#ifdef _DEBUG
			Fill(data,palette[*dataRLE],len);
		#else
			BurstFill(data,palette[*dataRLE],len);
		#endif
		++(dataRLE);
		}
	*source=dataRLE;
	}	


//*** RunLength_Opaque_Unclipped_Masked ***

void MaxBlitterRLE8::RunLength_Opaque_Unclipped_Masked(int len,unsigned short* data, unsigned char** source, unsigned short* palette)
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
				*data=RGBModulate16(*data,palette[*dataRLE]);
				}
			++data;
			++(dataRLE);
			}
		}
	else
		{
		// Run of values
		if ((*dataRLE)<255)
			{
			#ifdef _DEBUG
				Fill(data,palette[*dataRLE],len);
			#else
				BurstFill(data,palette[*dataRLE],len);
			#endif
			}
		++(dataRLE);
		}
	*source=dataRLE;
	}	
	

//*** RunLength_Opaque_Unclipped_Unmasked_Transparent ***

void MaxBlitterRLE8::RunLength_Opaque_Unclipped_Unmasked_Transparent(int len,unsigned short* data, unsigned char** source, unsigned short* palette, unsigned char alpha)
	{
	unsigned char* dataRLE=*source;
	if (len&0x80)	
		{
		len&=0x7f;
		// Unique values
		for (int i=0; i<len; ++i)
			{
			*data=RGBModulate16(*data,palette[*dataRLE]);
//			Blend(data,palette[*dataRLE],alpha);
			++data;
			++(dataRLE);
			}
		}
	else
		{
		// Run of values
		FillTransparent(data,palette[*dataRLE],len,alpha);
		++(dataRLE);
		}
	*source=dataRLE;
	}	
	

//*** RunLength_Opaque_Unclipped_MaskedTransparent ***

void MaxBlitterRLE8::RunLength_Opaque_Unclipped_MaskedTransparent(int len,unsigned short* data, unsigned char** source, unsigned short* palette, unsigned char alpha)
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
				*data=RGBModulate16(*data,palette[*dataRLE]);
//				Blend(data,palette[*dataRLE],alpha);
				}
			++data;
			++(dataRLE);
			}
		}
	else
		{
		// Run of values
		if ((*dataRLE)<255)
			{
			FillTransparent(data,palette[*dataRLE],len,alpha);
			}
		++(dataRLE);
		}
	*source=dataRLE;
	}	
	

//*** RunLength_Opaque_Clipped_Masked ***

void MaxBlitterRLE8::RunLength_Opaque_Clipped_Masked(int len,unsigned short* data, unsigned char** source, unsigned short* palette, int clipStart, int clipEnd)
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
				*data=RGBModulate16(*data, palette[*dataRLE]);
			++x;
			++data;
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
					*data=RGBModulate16(*data,color);
					}
				++x;
				++data;
				}
			}
		else
			{
			x+=len;
			data+=len;
			}
			
		++(dataRLE);
		}
	*source=dataRLE;
	}	


//*** RunLength_Opaque_Clipped_Masked_Transparent ***

void MaxBlitterRLE8::RunLength_Opaque_Clipped_Masked_Transparent(int len,unsigned short* data, unsigned char** source, unsigned short* palette, int clipStart, int clipEnd, unsigned char alpha)
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
				*data=RGBModulate16(*data,palette[*dataRLE]);
				//Blend(data,palette[*dataRLE],alpha);
				}
			++x;
			++data;
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
					*data=RGBModulate16(*data,color);
//					Blend(data,color,alpha);
					}
				++x;
				++data;
				}
			}
		else
			{
			x+=len;
			data+=len;
			}
			
		++(dataRLE);
		}
	*source=dataRLE;
	}	


//*** IgnoreOpaque ***

void MaxBlitterRLE8::IgnoreOpaque(int len, unsigned char** source)
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

void MaxBlitterRLE8::IgnoreAlpha(int len, unsigned char** source)
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
	

