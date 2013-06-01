//*** Bitmap.cpp ***

#include "Bitmap.h"
#include "Debug.h"
#include "ColorHelper.h"
#include "Blitter.h"


//*** Constructor ***

Bitmap::Bitmap():
	color_(0),
	alpha_(0),
	width_(0),
	height_(0),
	hPitch_(0),
	vPitch_(0),
	hOffset_(0),
	vOffset_(0)
	{
	}

	
//*** GetColorData ***

unsigned short* Bitmap::GetColorData() const
	{
	return color_;
	}


//*** GetAlphaData ***

unsigned char* Bitmap::GetAlphaData() const
	{
	return alpha_;
	}



//*** GetHPitch ***

int Bitmap::GetHPitch() const
	{
	return hPitch_;		
	}


//*** GetVPitch ***

int Bitmap::GetVPitch() const
	{
	return vPitch_;
	}


//*** GetHOffset ***

int Bitmap::GetHOffset() const
	{
	return hOffset_;		
	}


//*** GetVOffset ***

int Bitmap::GetVOffset() const
	{
	return vOffset_;		
	}


//*** Clear ***

void Bitmap::Clear()
	{
	int pixelCount=hPitch_*vPitch_;
	if (color_)
		{
		MemSet(color_,0,sizeof(unsigned short)*pixelCount);
		}
	if (alpha_)
		{
		MemSet(alpha_,0,sizeof(unsigned char)*pixelCount);
		}
	}


//*** Fill ***

void Bitmap::Fill(int x1, int y1, int x2, int y2, unsigned short color, unsigned char alpha)
	{
	// Don't do anything if a fully transparent fill was requested 
	if (alpha==0)
		{
		return;
		}

	// Adjust for offset
	x1-=hOffset_;
	y1-=vOffset_;
	x2-=hOffset_;
	y2-=vOffset_;

	// Clip
	if (x1<0)
		{
		x1=0;
		}
	if (y1<0)
		{
		y1=0;
		}
	if (x2>=hPitch_)
		{
		x2=hPitch_-1;
		}
	if (y2>=vPitch_)
		{
		y2=vPitch_-1;
		}

	int targetWidth=x2-x1+1;
	int targetHeight=y2-y1+1;

	Blitter::BlitArgs blitArgs;
	blitArgs.color=color;
	blitArgs.alpha=alpha;
	blitArgs.targetColor=color_;
	blitArgs.targetAlpha=alpha_;
	blitArgs.targetHPitch=hPitch_;
	blitArgs.sourceX=x1;
	blitArgs.sourceY=y1;
	blitArgs.sourceWidth=targetWidth;
	blitArgs.sourceHeight=targetHeight;
	blitArgs.targetX=x1;
	blitArgs.targetY=y1;

	// If alpha is 255, we fill it all with a solid color and solid alpha
	if (alpha==255)
		{
		if (color_)
			{
			Blitter::Normal<
				Blitter::OpFill_Color_TC,
				Blitter::StepX_Normal_TC,
				Blitter::StepY_Normal_TC
			>(blitArgs);
			}
		if (alpha_)
			{
			Blitter::Normal_Fill_Alpha_TA(blitArgs);
			}
		}
	else
		{
		if (color_)
			{
			Blitter::Normal<
				Blitter::OpBlend_Color_Alpha_TC,
				Blitter::StepX_Normal_TC,
				Blitter::StepY_Normal_TC
			>(blitArgs);
			}
		if (alpha_)
			{
			Blitter::Normal<
				Blitter::OpMax_Alpha_TA,
				Blitter::StepX_Normal_TA,
				Blitter::StepY_Normal_TA
			>(blitArgs);
			}
		}
	
	}


//*** Fill ***

void Bitmap::Fill(unsigned short color, unsigned char alpha)
	{
	Blitter::BlitArgs blitArgs;
	blitArgs.color=color;
	blitArgs.alpha=alpha;
	blitArgs.targetColor=color_;
	blitArgs.targetAlpha=alpha_;
	blitArgs.targetHPitch=hPitch_;
	blitArgs.sourceX=0;
	blitArgs.sourceY=0;
	blitArgs.sourceWidth=hPitch_;
	blitArgs.sourceHeight=vPitch_;
	blitArgs.targetX=0;
	blitArgs.targetY=0;

	// If alpha is 255, we fill it all with a solid color and solid alpha
	if (alpha==255)
		{
		if (color_)
			{
			Blitter::Normal<
				Blitter::OpFill_Color_TC,
				Blitter::StepX_Normal_TC,
				Blitter::StepY_Normal_TC
			>(blitArgs);
			}
		if (alpha_)
			{
			Blitter::Normal_Fill_Alpha_TA(blitArgs);
			}
		}
	else
		{
		if (color_)
			{
			Blitter::Normal<
				Blitter::OpBlend_Color_Alpha_TC,
				Blitter::StepX_Normal_TC,
				Blitter::StepY_Normal_TC
			>(blitArgs);
			}
		if (alpha_)
			{
			Blitter::Normal<
				Blitter::OpMax_Alpha_TA,
				Blitter::StepX_Normal_TA,
				Blitter::StepY_Normal_TA
			>(blitArgs);
			}
		}
	}


//*** FillAlpha ***

void Bitmap::FillAlpha(int x1, int y1, int x2, int y2, unsigned char alpha)
	{
	if (!alpha_)
		{
		return;
		}

	// Adjust for offset
	x1-=hOffset_;
	y1-=vOffset_;
	x2-=hOffset_;
	y2-=vOffset_;

	// Clip
	if (x1<0)
		{
		x1=0;
		}
	if (y1<0)
		{
		y1=0;
		}
	if (x2>=hPitch_)
		{
		x2=hPitch_-1;
		}
	if (y2>=vPitch_)
		{
		y2=vPitch_-1;
		}

	int targetWidth=x2-x1+1;
	int targetHeight=y2-y1+1;

	Blitter::BlitArgs blitArgs;
	blitArgs.alpha=alpha;
	blitArgs.targetColor=color_;
	blitArgs.targetAlpha=alpha_;
	blitArgs.targetHPitch=hPitch_;
	blitArgs.sourceX=x1;
	blitArgs.sourceY=y1;
	blitArgs.sourceWidth=targetWidth;
	blitArgs.sourceHeight=targetHeight;
	blitArgs.targetX=x1;
	blitArgs.targetY=y1;


	Blitter::Normal_Fill_Alpha_TA(blitArgs);
	}


//*** FillAlpha ***

void Bitmap::FillAlpha(unsigned char alpha)
	{
	if (alpha_)
		{
		Blitter::BlitArgs blitArgs;
		blitArgs.alpha=alpha;
		blitArgs.targetColor=color_;
		blitArgs.targetAlpha=alpha_;
		blitArgs.targetHPitch=hPitch_;
		blitArgs.sourceX=0;
		blitArgs.sourceY=0;
		blitArgs.sourceWidth=hPitch_;
		blitArgs.sourceHeight=vPitch_;
		blitArgs.targetX=0;
		blitArgs.targetY=0;

		Blitter::Normal_Fill_Alpha_TA(blitArgs);
		}
	}


//*** GetWidth ***

int Bitmap::GetWidth(Transformation transformation) const
	{
	switch (transformation)
		{
		case NoTransformation:
		case Rotate_180:
		case Mirror_X:
		case Mirror_Y:
			{
			return width_;
			} break;

		case Rotate_90:
		case Rotate_270:
			{
			return height_;
			} break;
		}
	
	return width_;		
	}


//*** GetHeight ***

int Bitmap::GetHeight(Transformation transformation) const
	{
	switch (transformation)
		{
		case NoTransformation:
		case Rotate_180:
		case Mirror_X:
		case Mirror_Y:
			{
			return height_;
			} break;

		case Rotate_90:
		case Rotate_270:
			{
			return width_;
			} break;
		}
	
	return height_;
	}


//*** GetPixelColor ***

unsigned short Bitmap::GetPixelColor(int x, int y, Transformation transformation) const
	{
	if (color_)
		{
		TransformCoordinates(x,y,transformation);
		x-=hOffset_;
		y-=vOffset_;
		if (x>=0 && x<hPitch_ && y>=0 && y<vPitch_)
			{
			return color_[x+y*hPitch_];
			}
		}
	return 0;
	}


//*** GetPixelAlpha ***

unsigned char Bitmap::GetPixelAlpha(int x, int y, Transformation transformation) const
	{
	TransformCoordinates(x,y,transformation);
	x-=hOffset_;
	y-=vOffset_;
	if (x>=0 && x<hPitch_ && y>=0 && y<vPitch_)
		{
		if (alpha_)
			{
			return alpha_[x+y*hPitch_];
			}
		return 255;
		}
	return 0;
	}


//*** SetPixelColor ***

void Bitmap::SetPixelColor(int x, int y,unsigned short color, Transformation transformation)
	{
	if (color_)
		{
		TransformCoordinates(x,y,transformation);
		x-=hOffset_;
		y-=vOffset_;
		if (x>=0 && x<hPitch_ && y>=0 && y<vPitch_)
			{
			color_[x+y*hPitch_]=color;
			}
		}
	}


//*** SetPixelAlpha ***

void Bitmap::SetPixelAlpha(int x, int y,unsigned char alpha, Transformation transformation)
	{
	if (alpha_)
		{
		TransformCoordinates(x,y,transformation);
		x-=hOffset_;
		y-=vOffset_;
		if (x>=0 && x<hPitch_ && y>=0 && y<vPitch_)
			{
			alpha_[x+y*hPitch_]=alpha;
			}
		}
	}


//*** BlendPixel ***

void Bitmap::BlendPixel(int x, int y,unsigned short color, unsigned char alpha, Transformation transformation)
	{
	if (color_)
		{
		TransformCoordinates(x,y,transformation);
		x-=hOffset_;
		y-=vOffset_;
		if (x>=0 && x<hPitch_ && y>=0 && y<vPitch_)
			{
			if (alpha==255)
				{
				color_[x+y*width_]=color;
				}
			else if (alpha>0)
				{
				unsigned short* destination=&color_[x+y*hPitch_];
				*destination=AlphaBlend16(*destination,color,alpha);
				}
			}
		}
	}


//*** Blit ***

void Bitmap::Blit(Bitmap& target, int x, int y, unsigned short modulate, unsigned char alpha, Transformation transformation, bool maxBlit) const
	{
	Blit(0, 0, width_-1, height_-1, target, x, y, modulate, alpha, transformation, maxBlit );
	}


//*** Blit ***

void Bitmap::Blit(int x1, int y1, int x2, int y2, Bitmap& target, int x, int y, unsigned short modulate, unsigned char alpha, Transformation transformation, bool maxBlit ) const
	{
	// Don't do anything if alpha is set to 0, as that means every pixel would be written at 100% transparent
	if (alpha==0)
		{
		return;
		}


	// Perform clipping
	int sourceX=x1;
	int sourceY=y1;
	int sourceWidth=(x2-x1)+1;
	int sourceHeight=(y2-y1)+1;
	int targetX=x;
	int targetY=y;

	if (hPitch_!=width_ || vPitch_!=height_)
		{
		if (hOffset_>sourceX)
			{
			targetX+=hOffset_-sourceX;
			sourceWidth-=hOffset_-sourceX;
			sourceX=hOffset_;
			}
		if (vOffset_>sourceY)
			{
			targetY+=vOffset_-sourceY;
			sourceHeight-=vOffset_-sourceY;
			sourceY=vOffset_;
			}
		if (sourceX+sourceWidth>hOffset_+hPitch_)
			{
			sourceWidth-=(sourceX+sourceWidth)-(hOffset_+hPitch_);
			}
		if (sourceY+sourceHeight>vOffset_+vPitch_)
			{
			sourceHeight-=(sourceY+sourceHeight)-(vOffset_+vPitch_);
			}
		}

	if (!Clip(sourceX,sourceY,sourceWidth,sourceHeight,targetX,targetY,target.GetWidth(),target.GetHeight(),transformation))
		{
		// Nothing to draw, so bail out
		return;
		}


	// Sanity checks - will only trigger if there's a bug in the Clip code
	Assert(sourceX>=0 && sourceY>=0 && (sourceX+sourceWidth)<=GetWidth(transformation) && (sourceY+sourceHeight)<=GetHeight(transformation),"Source rect out of range");
	Assert(targetX>=0 && targetY>=0 && (targetX+sourceWidth)<=target.GetWidth() && (targetY+sourceHeight)<=target.GetHeight(),"Target rect out of range");

///\todo  This test must take transformations into account
//	Assert(sourceX>=hOffset_ && sourceY>=vOffset_ && (sourceX+sourceWidth)<=hOffset_+hPitch_ && (sourceY+sourceHeight)<=vOffset_+vPitch_,"Source rect out of range");

	// Set up the blitting parameters
	unsigned short* sourceColor=color_;
	unsigned char* sourceAlpha=alpha_;
	int sourceHPitch=hPitch_;
	int sourceVPitch=vPitch_;
	unsigned short* targetColor=target.GetColorData();
	unsigned char* targetAlpha=target.GetAlphaData();
	int targetPitch=target.GetHPitch();

	sourceX-=hOffset_;
	sourceY-=vOffset_;

	Blitter::BlitArgs blitArgs;
	blitArgs.color=modulate;
	blitArgs.alpha=alpha;
	blitArgs.sourceColor=sourceColor;
	blitArgs.sourceAlpha=sourceAlpha;
	blitArgs.targetColor=targetColor;
	blitArgs.targetAlpha=targetAlpha;
	blitArgs.sourceHPitch=sourceHPitch;
	blitArgs.sourceVPitch=sourceVPitch;
	blitArgs.targetHPitch=targetPitch;
	blitArgs.sourceX=sourceX;
	blitArgs.sourceY=sourceY;
	blitArgs.sourceWidth=sourceWidth;
	blitArgs.sourceHeight=sourceHeight;
	blitArgs.targetX=targetX;
	blitArgs.targetY=targetY;

	// If we have a color, and target does too, but we don't have an alpha channel
	if (sourceColor && targetColor && !sourceAlpha)
		{
		// Pick the right blitting for the specified transformation
		switch (transformation)
			{
			case NoTransformation:
				{
				if (modulate==0xffff && alpha==255) 
					{
					Blitter::Normal_Copy_SC_TC(blitArgs);
					}
				else if (modulate!=0xffff && alpha==255) 
					{
					Blitter::Normal<
						Blitter::OpCopy_Color_SC_TC,
						Blitter::StepX_Normal_SC_TC,
						Blitter::StepY_Normal_SC_TC
					>(blitArgs);
					}
				else if (modulate==0xffff && alpha!=255) 
					{
					Blitter::Normal<
						Blitter::OpBlend_Alpha_SC_TC,
						Blitter::StepX_Normal_SC_TC,
						Blitter::StepY_Normal_SC_TC
					>(blitArgs);
					}
				else 
					{
					Blitter::Normal<
						Blitter::OpBlend_Color_Alpha_SC_TC,
						Blitter::StepX_Normal_SC_TC,
						Blitter::StepY_Normal_SC_TC
					>(blitArgs);
					}
				} break;

			case Rotate_90:
				{
				if (modulate==0xffff && alpha==255)
					{
					Blitter::Rot090<
						Blitter::OpCopy_SC_TC,
						Blitter::StepX_Rot090_SC_TC,
						Blitter::StepY_Rot090_SC_TC
					>(blitArgs);
					}
				else if (modulate!=0xffff && alpha==255)
					{
					Blitter::Rot090<
						Blitter::OpCopy_Color_SC_TC,
						Blitter::StepX_Rot090_SC_TC,
						Blitter::StepY_Rot090_SC_TC
					>(blitArgs);
					}
				else if (modulate==0xffff && alpha!=255)
					{
					Blitter::Rot090<
						Blitter::OpBlend_Alpha_SC_TC,
						Blitter::StepX_Rot090_SC_TC,
						Blitter::StepY_Rot090_SC_TC
					>(blitArgs);
					}
				else 
					{
					Blitter::Rot090<
						Blitter::OpBlend_Color_Alpha_SC_TC,
						Blitter::StepX_Rot090_SC_TC,
						Blitter::StepY_Rot090_SC_TC
					>(blitArgs);
					}
				} break;

			case Rotate_180:
				{
				if (modulate==0xffff && alpha==255)
					{
					Blitter::Rot180<
						Blitter::OpCopy_SC_TC,
						Blitter::StepX_Rot180_SC_TC,
						Blitter::StepY_Rot180_SC_TC
					>(blitArgs);
					}
				else if (modulate!=0xffff && alpha==255)
					{
					Blitter::Rot180<
						Blitter::OpCopy_Color_SC_TC,
						Blitter::StepX_Rot180_SC_TC,
						Blitter::StepY_Rot180_SC_TC
					>(blitArgs);
					}
				else if (modulate==0xffff && alpha!=255)
					{
					Blitter::Rot180<
						Blitter::OpBlend_Alpha_SC_TC,
						Blitter::StepX_Rot180_SC_TC,
						Blitter::StepY_Rot180_SC_TC
					>(blitArgs);
					}
				else 
					{
					Blitter::Rot180<
						Blitter::OpBlend_Color_Alpha_SC_TC,
						Blitter::StepX_Rot180_SC_TC,
						Blitter::StepY_Rot180_SC_TC
					>(blitArgs);
					}
				} break;

			case Rotate_270:
				{
				if (modulate==0xffff && alpha==255)
					{
					Blitter::Rot270<
						Blitter::OpCopy_SC_TC,
						Blitter::StepX_Rot270_SC_TC,
						Blitter::StepY_Rot270_SC_TC
					>(blitArgs);
					}
				else if (modulate!=0xffff && alpha==255)
					{
					Blitter::Rot270<
						Blitter::OpCopy_Color_SC_TC,
						Blitter::StepX_Rot270_SC_TC,
						Blitter::StepY_Rot270_SC_TC
					>(blitArgs);
					}
				else if (modulate==0xffff && alpha!=255)
					{
					Blitter::Rot270<
						Blitter::OpBlend_Alpha_SC_TC,
						Blitter::StepX_Rot270_SC_TC,
						Blitter::StepY_Rot270_SC_TC
					>(blitArgs);
					}
				else 
					{
					Blitter::Rot270<
						Blitter::OpBlend_Color_Alpha_SC_TC,
						Blitter::StepX_Rot270_SC_TC,
						Blitter::StepY_Rot270_SC_TC
					>(blitArgs);
					}
				} break;

			case Mirror_X:
				{
				if (modulate==0xffff && alpha==255)
					{
					Blitter::Flip_X<
						Blitter::OpCopy_SC_TC,
						Blitter::StepX_Flip_X_SC_TC,
						Blitter::StepY_Flip_X_SC_TC
					>(blitArgs);
					}
				else if (modulate!=0xffff && alpha==255)
					{
					Blitter::Flip_X<
						Blitter::OpCopy_Color_SC_TC,
						Blitter::StepX_Flip_X_SC_TC,
						Blitter::StepY_Flip_X_SC_TC
					>(blitArgs);
					}
				else if (modulate==0xffff && alpha!=255)
					{
					Blitter::Flip_X<
						Blitter::OpBlend_Alpha_SC_TC,
						Blitter::StepX_Flip_X_SC_TC,
						Blitter::StepY_Flip_X_SC_TC
					>(blitArgs);
					}
				else 
					{
					Blitter::Flip_X<
						Blitter::OpBlend_Color_Alpha_SC_TC,
						Blitter::StepX_Flip_X_SC_TC,
						Blitter::StepY_Flip_X_SC_TC
					>(blitArgs);
					}
				} break;

			case Mirror_Y:
				{
				if (modulate==0xffff && alpha==255)
					{
					Blitter::Flip_Y_Copy_SC_TC(blitArgs);
					}
				else if (modulate!=0xffff && alpha==255)
					{
					Blitter::Flip_Y<
						Blitter::OpCopy_Color_SC_TC,
						Blitter::StepX_Flip_Y_SC_TC,
						Blitter::StepY_Flip_Y_SC_TC
					>(blitArgs);
					}
				else if (modulate==0xffff && alpha!=255)
					{
					Blitter::Flip_Y<
						Blitter::OpBlend_Alpha_SC_TC,
						Blitter::StepX_Flip_Y_SC_TC,
						Blitter::StepY_Flip_Y_SC_TC
					>(blitArgs);
					}
				else 
					{
					Blitter::Flip_Y<
						Blitter::OpBlend_Color_Alpha_SC_TC,
						Blitter::StepX_Flip_Y_SC_TC,
						Blitter::StepY_Flip_Y_SC_TC
					>(blitArgs);
					}
				} break;

			}
		}


	// If we have a color, and target does too, but we have an alpha channel
	if (sourceColor && targetColor && sourceAlpha)
		{
		// Pick the right blitting for the specified transformation
		switch (transformation)
			{
			case NoTransformation:
				{
				if (modulate==0xffff && alpha==255)
					{
					Blitter::Normal<
						Blitter::OpBlend_SC_SA_TC,
						Blitter::StepX_Normal_SC_SA_TC,
						Blitter::StepY_Normal_SC_SA_TC
					>(blitArgs);
					}
				else if (modulate!=0xffff && alpha==255)
					{
					Blitter::Normal<
						Blitter::OpBlend_Color_SC_SA_TC,
						Blitter::StepX_Normal_SC_SA_TC,
						Blitter::StepY_Normal_SC_SA_TC
					>(blitArgs);
					}
				else if (modulate==0xffff && alpha!=255)
					{
					Blitter::Normal<
						Blitter::OpBlend_Alpha_SC_SA_TC,
						Blitter::StepX_Normal_SC_SA_TC,
						Blitter::StepY_Normal_SC_SA_TC
					>(blitArgs);
					}
				else 
					{
					Blitter::Normal<
						Blitter::OpBlend_Color_Alpha_SC_SA_TC,
						Blitter::StepX_Normal_SC_SA_TC,
						Blitter::StepY_Normal_SC_SA_TC
					>(blitArgs);
					}
				} break;

			case Rotate_90:
				{
				if (modulate==0xffff && alpha==255)
					{
					Blitter::Rot090<
						Blitter::OpBlend_SC_SA_TC,
						Blitter::StepX_Rot090_SC_SA_TC,
						Blitter::StepY_Rot090_SC_SA_TC
					>(blitArgs);
					}
				else if (modulate!=0xffff && alpha==255)
					{
					Blitter::Rot090<
						Blitter::OpBlend_Color_SC_SA_TC,
						Blitter::StepX_Rot090_SC_SA_TC,
						Blitter::StepY_Rot090_SC_SA_TC
					>(blitArgs);
					}
				else if (modulate==0xffff && alpha!=255)
					{
					Blitter::Rot090<
						Blitter::OpBlend_Alpha_SC_SA_TC,
						Blitter::StepX_Rot090_SC_SA_TC,
						Blitter::StepY_Rot090_SC_SA_TC
					>(blitArgs);
					}
				else 
					{
					Blitter::Rot090<
						Blitter::OpBlend_Color_Alpha_SC_SA_TC,
						Blitter::StepX_Rot090_SC_SA_TC,
						Blitter::StepY_Rot090_SC_SA_TC
					>(blitArgs);
					}
				} break;

			case Rotate_180:
				{
				if (modulate==0xffff && alpha==255)
					{
					Blitter::Rot180<
						Blitter::OpBlend_SC_SA_TC,
						Blitter::StepX_Rot180_SC_SA_TC,
						Blitter::StepY_Rot180_SC_SA_TC
					>(blitArgs);
					}
				else if (modulate!=0xffff && alpha==255)
					{
					Blitter::Rot180<
						Blitter::OpBlend_Color_SC_SA_TC,
						Blitter::StepX_Rot180_SC_SA_TC,
						Blitter::StepY_Rot180_SC_SA_TC
					>(blitArgs);
					}
				else if (modulate==0xffff && alpha!=255)
					{
					Blitter::Rot180<
						Blitter::OpBlend_Alpha_SC_SA_TC,
						Blitter::StepX_Rot180_SC_SA_TC,
						Blitter::StepY_Rot180_SC_SA_TC
					>(blitArgs);
					}
				else 
					{
					Blitter::Rot180<
						Blitter::OpBlend_Color_Alpha_SC_SA_TC,
						Blitter::StepX_Rot180_SC_SA_TC,
						Blitter::StepY_Rot180_SC_SA_TC
					>(blitArgs);
					}
				} break;

			case Rotate_270:
				{
				if (modulate==0xffff && alpha==255)
					{
					Blitter::Rot270<
						Blitter::OpBlend_SC_SA_TC,
						Blitter::StepX_Rot270_SC_SA_TC,
						Blitter::StepY_Rot270_SC_SA_TC
					>(blitArgs);
					}
				else if (modulate!=0xffff && alpha==255)
					{
					Blitter::Rot270<
						Blitter::OpBlend_Color_SC_SA_TC,
						Blitter::StepX_Rot270_SC_SA_TC,
						Blitter::StepY_Rot270_SC_SA_TC
					>(blitArgs);
					}
				else if (modulate==0xffff && alpha!=255)
					{
					Blitter::Rot270<
						Blitter::OpBlend_Alpha_SC_SA_TC,
						Blitter::StepX_Rot270_SC_SA_TC,
						Blitter::StepY_Rot270_SC_SA_TC
					>(blitArgs);
					}
				else 
					{
					Blitter::Rot270<
						Blitter::OpBlend_Color_Alpha_SC_SA_TC,
						Blitter::StepX_Rot270_SC_SA_TC,
						Blitter::StepY_Rot270_SC_SA_TC
					>(blitArgs);
					}
				} break;

			case Mirror_X:
				{
				if (modulate==0xffff && alpha==255)
					{
					Blitter::Flip_X<
						Blitter::OpBlend_SC_SA_TC,
						Blitter::StepX_Flip_X_SC_SA_TC,
						Blitter::StepY_Flip_X_SC_SA_TC
					>(blitArgs);
					}
				else if (modulate!=0xffff && alpha==255)
					{
					Blitter::Flip_X<
						Blitter::OpBlend_Color_SC_SA_TC,
						Blitter::StepX_Flip_X_SC_SA_TC,
						Blitter::StepY_Flip_X_SC_SA_TC
					>(blitArgs);
					}
				else if (modulate==0xffff && alpha!=255)
					{
					Blitter::Flip_X<
						Blitter::OpBlend_Alpha_SC_SA_TC,
						Blitter::StepX_Flip_X_SC_SA_TC,
						Blitter::StepY_Flip_X_SC_SA_TC
					>(blitArgs);
					}
				else 
					{
					Blitter::Flip_X<
						Blitter::OpBlend_Color_Alpha_SC_SA_TC,
						Blitter::StepX_Flip_X_SC_SA_TC,
						Blitter::StepY_Flip_X_SC_SA_TC
					>(blitArgs);
					}
				} break;

			case Mirror_Y:
				{
				if (modulate==0xffff && alpha==255)
					{
					Blitter::Flip_Y<
						Blitter::OpBlend_SC_SA_TC,
						Blitter::StepX_Flip_Y_SC_SA_TC,
						Blitter::StepY_Flip_Y_SC_SA_TC
					>(blitArgs);
					}
				else if (modulate!=0xffff && alpha==255)
					{
					Blitter::Flip_Y<
						Blitter::OpBlend_Color_SC_SA_TC,
						Blitter::StepX_Flip_Y_SC_SA_TC,
						Blitter::StepY_Flip_Y_SC_SA_TC
					>(blitArgs);
					}
				else if (modulate==0xffff && alpha!=255)
					{
					Blitter::Flip_Y<
						Blitter::OpBlend_Alpha_SC_SA_TC,
						Blitter::StepX_Flip_Y_SC_SA_TC,
						Blitter::StepY_Flip_Y_SC_SA_TC
					>(blitArgs);
					}
				else 
					{
					Blitter::Flip_Y<
						Blitter::OpBlend_Color_Alpha_SC_SA_TC,
						Blitter::StepX_Flip_Y_SC_SA_TC,
						Blitter::StepY_Flip_Y_SC_SA_TC
					>(blitArgs);
					}
				} break;
			}
		}

	// If we don't have color, but target does, and we have alpha channel 
	if (!sourceColor && targetColor && sourceAlpha)
		{
		// Pick the right blitting for the specified transformation
		switch (transformation)
			{
			case NoTransformation:
				{
				if (alpha==255)
					{
					Blitter::Normal<
						Blitter::OpBlend_Color_SA_TC,
						Blitter::StepX_Normal_SA_TC,
						Blitter::StepY_Normal_SA_TC
					>(blitArgs);
					}
				else 
					{
					Blitter::Normal<
						Blitter::OpBlend_Color_Alpha_SA_TC,
						Blitter::StepX_Normal_SA_TC,
						Blitter::StepY_Normal_SA_TC
					>(blitArgs);
					}
				} break;

			case Rotate_90:
				{
				if (alpha==255)
					{
					Blitter::Rot090<
						Blitter::OpBlend_Color_SA_TC,
						Blitter::StepX_Rot090_SA_TC,
						Blitter::StepY_Rot090_SA_TC
					>(blitArgs);
					}
				else 
					{
					Blitter::Rot090<
						Blitter::OpBlend_Color_Alpha_SA_TC,
						Blitter::StepX_Rot090_SA_TC,
						Blitter::StepY_Rot090_SA_TC
					>(blitArgs);
					}
				} break;

			case Rotate_180:
				{
				if (alpha==255)
					{
					Blitter::Rot180<
						Blitter::OpBlend_Color_SA_TC,
						Blitter::StepX_Rot180_SA_TC,
						Blitter::StepY_Rot180_SA_TC
					>(blitArgs);
					}
				else 
					{
					Blitter::Rot180<
						Blitter::OpBlend_Color_Alpha_SA_TC,
						Blitter::StepX_Rot180_SA_TC,
						Blitter::StepY_Rot180_SA_TC
					>(blitArgs);
					}
				} break;

			case Rotate_270:
				{
				if (alpha==255)
					{
					Blitter::Rot270<
						Blitter::OpBlend_Color_SA_TC,
						Blitter::StepX_Rot270_SA_TC,
						Blitter::StepY_Rot270_SA_TC
					>(blitArgs);
					}
				else 
					{
					Blitter::Rot270<
						Blitter::OpBlend_Color_Alpha_SA_TC,
						Blitter::StepX_Rot270_SA_TC,
						Blitter::StepY_Rot270_SA_TC
					>(blitArgs);
					}
				} break;

			case Mirror_X:
				{
				if (alpha==255)
					{
					Blitter::Flip_X<
						Blitter::OpBlend_Color_SA_TC,
						Blitter::StepX_Flip_X_SA_TC,
						Blitter::StepY_Flip_X_SA_TC
					>(blitArgs);
					}
				else 
					{
					Blitter::Flip_X<
						Blitter::OpBlend_Color_Alpha_SA_TC,
						Blitter::StepX_Flip_X_SA_TC,
						Blitter::StepY_Flip_X_SA_TC
					>(blitArgs);
					}
				} break;

			case Mirror_Y:
				{
				if (alpha==255)
					{
					Blitter::Flip_Y<
						Blitter::OpBlend_Color_SA_TC,
						Blitter::StepX_Flip_Y_SA_TC,
						Blitter::StepY_Flip_Y_SA_TC
					>(blitArgs);
					}
				else 
					{
					Blitter::Flip_Y<
						Blitter::OpBlend_Color_Alpha_SA_TC,
						Blitter::StepX_Flip_Y_SA_TC,
						Blitter::StepY_Flip_Y_SA_TC
					>(blitArgs);
					}
				} break;
			}
		}

	// If we don't have color, but target does 
	if (!sourceColor && targetColor && !sourceAlpha)
		{
		if (alpha==255)
			{
			Blitter::Normal<
				Blitter::OpFill_Color_TC,
				Blitter::StepX_Normal_TC,
				Blitter::StepY_Normal_TC
			>(blitArgs);
			}
		else
			{
			Blitter::Normal<
				Blitter::OpBlend_Color_Alpha_TC,
				Blitter::StepX_Normal_TC,
				Blitter::StepY_Normal_TC
			>(blitArgs);
			}
		}

	// If we have an alpha channel, and target does too
	if (sourceAlpha && targetAlpha)
		{
		// Pick the right blitting for the specified transformation
		switch (transformation)
			{
			case NoTransformation:
				{
				if (alpha==255) 
					{
					Blitter::Normal<
						Blitter::OpMax_SA_TA,
						Blitter::StepX_Normal_SA_TA,
						Blitter::StepY_Normal_SA_TA
					>(blitArgs);
					}
				else
					{
					Blitter::Normal<
						Blitter::OpMax_Alpha_SA_TA,
						Blitter::StepX_Normal_SA_TA,
						Blitter::StepY_Normal_SA_TA
					>(blitArgs);
					}
				} break;

			case Rotate_90:
				{
				if (alpha==255) 
					{
					Blitter::Rot090<
						Blitter::OpMax_SA_TA,
						Blitter::StepX_Rot090_SA_TA,
						Blitter::StepY_Rot090_SA_TA
					>(blitArgs);
					}
				else
					{
					Blitter::Rot090<
						Blitter::OpMax_Alpha_SA_TA,
						Blitter::StepX_Rot090_SA_TA,
						Blitter::StepY_Rot090_SA_TA
					>(blitArgs);
					}
				} break;

			case Rotate_180:
				{
				if (alpha==255) 
					{
					Blitter::Rot180<
						Blitter::OpMax_SA_TA,
						Blitter::StepX_Rot180_SA_TA,
						Blitter::StepY_Rot180_SA_TA
					>(blitArgs);
					}
				else
					{
					Blitter::Rot180<
						Blitter::OpMax_Alpha_SA_TA,
						Blitter::StepX_Rot180_SA_TA,
						Blitter::StepY_Rot180_SA_TA
					>(blitArgs);
					}
				} break;

			case Rotate_270:
				{
				if (alpha==255) 
					{
					Blitter::Rot270<
						Blitter::OpMax_SA_TA,
						Blitter::StepX_Rot270_SA_TA,
						Blitter::StepY_Rot270_SA_TA
					>(blitArgs);
					}
				else
					{
					Blitter::Rot270<
						Blitter::OpMax_Alpha_SA_TA,
						Blitter::StepX_Rot270_SA_TA,
						Blitter::StepY_Rot270_SA_TA
					>(blitArgs);
					}
				} break;

			case Mirror_X:
				{
				if (alpha==255) 
					{
					Blitter::Flip_X<
						Blitter::OpMax_SA_TA,
						Blitter::StepX_Flip_X_SA_TA,
						Blitter::StepY_Flip_X_SA_TA
					>(blitArgs);
					}
				else
					{
					Blitter::Flip_X<
						Blitter::OpMax_Alpha_SA_TA,
						Blitter::StepX_Flip_X_SA_TA,
						Blitter::StepY_Flip_X_SA_TA
					>(blitArgs);
					}
				} break;

			case Mirror_Y:
				{
				if (alpha==255) 
					{
					Blitter::Flip_Y<
						Blitter::OpMax_SA_TA,
						Blitter::StepX_Flip_Y_SA_TA,
						Blitter::StepY_Flip_Y_SA_TA
					>(blitArgs);
					}
				else
					{
					Blitter::Flip_Y<
						Blitter::OpMax_Alpha_SA_TA,
						Blitter::StepX_Flip_Y_SA_TA,
						Blitter::StepY_Flip_Y_SA_TA
					>(blitArgs);
					}
				} break;

			}
		}

	// If we don't have an alpha channel, but target does, set it to full opaque if alpha is 255, otherwise blend the alpha
	if (!sourceAlpha && targetAlpha)
		{
		if (alpha==255)
			{
			Blitter::Normal_Fill_Alpha_TA(blitArgs);
			}
		else
			{
			Blitter::Normal<
				Blitter::OpMax_Alpha_TA,
				Blitter::StepX_Normal_TA,
				Blitter::StepY_Normal_TA
			>(blitArgs);
			}
		}
	}




//*** Copy ***

void Bitmap::Copy(Bitmap& target, int x, int y, unsigned short modulate, Transformation transformation) const
	{
	Copy(0, 0, width_-1, height_-1, target, x, y, modulate, transformation);
	}


//*** Copy ***

void Bitmap::Copy(int x1, int y1, int x2, int y2, Bitmap& target, int x, int y, unsigned short modulate, Transformation transformation) const
	{
	// Perform clipping
	int sourceX=x1;
	int sourceY=y1;
	int sourceWidth=(x2-x1)+1;
	int sourceHeight=(y2-y1)+1;
	int targetX=x;
	int targetY=y;
	if (!Clip(sourceX,sourceY,sourceWidth,sourceHeight,targetX,targetY,target.GetWidth(),target.GetHeight(),transformation))
		{
		// Nothing to draw, so bail out
		return;
		}

	// Sanity checks - will only trigger if there's a bug in the Clip code
	Assert(sourceX>=0 && sourceY>=0 && (sourceX+sourceWidth)<=GetWidth(transformation) && (sourceY+sourceHeight)<=GetHeight(transformation),"Source rect out of range");
	Assert(targetX>=0 && targetY>=0 && (targetX+sourceWidth)<=target.GetWidth() && (targetY+sourceHeight)<=target.GetHeight(),"Target rect out of range");

	// Set up the blitting parameters
	unsigned short* sourceColor=color_;
	unsigned char* sourceAlpha=alpha_;
	int sourceHPitch=width_;
	int sourceVPitch=height_;
	unsigned short* targetColor=target.GetColorData();
	unsigned char* targetAlpha=target.GetAlphaData();
	int targetPitch=target.GetWidth();

	Blitter::BlitArgs blitArgs;
	blitArgs.color=modulate;
	blitArgs.alpha=255;
	blitArgs.sourceColor=sourceColor;
	blitArgs.sourceAlpha=sourceAlpha;
	blitArgs.targetColor=targetColor;
	blitArgs.targetAlpha=targetAlpha;
	blitArgs.sourceHPitch=sourceHPitch;
	blitArgs.sourceVPitch=sourceVPitch;
	blitArgs.targetHPitch=targetPitch;
	blitArgs.sourceX=sourceX;
	blitArgs.sourceY=sourceY;
	blitArgs.sourceWidth=sourceWidth;
	blitArgs.sourceHeight=sourceHeight;
	blitArgs.targetX=targetX;
	blitArgs.targetY=targetY;

	// Copy color channel, if we have one
	if (sourceColor && targetColor)
		{
		// Pick the right copy type for the specified transformation
		switch (transformation)
			{
			case NoTransformation:
				{
				if (modulate==0xffff)
					{
					Blitter::Normal_Copy_SC_TC(blitArgs);
					}
				else
					{
					Blitter::Normal<
						Blitter::OpCopy_Color_SC_TC,
						Blitter::StepX_Normal_SC_TC,
						Blitter::StepY_Normal_SC_TC
					>(blitArgs);
					}
				} break;

			case Rotate_90:
				{
				if (modulate==0xffff)
					{
					Blitter::Rot090<
						Blitter::OpCopy_SC_TC,
						Blitter::StepX_Rot090_SC_TC,
						Blitter::StepY_Rot090_SC_TC
					>(blitArgs);
					}
				else
					{
					Blitter::Rot090<
						Blitter::OpCopy_Color_SC_TC,
						Blitter::StepX_Rot090_SC_TC,
						Blitter::StepY_Rot090_SC_TC
					>(blitArgs);
					}
				} break;

			case Rotate_180:
				{
				if (modulate==0xffff)
					{
					Blitter::Rot180<
						Blitter::OpCopy_SC_TC,
						Blitter::StepX_Rot180_SC_TC,
						Blitter::StepY_Rot180_SC_TC
					>(blitArgs);
					}
				else
					{
					Blitter::Rot180<
						Blitter::OpCopy_Color_SC_TC,
						Blitter::StepX_Rot180_SC_TC,
						Blitter::StepY_Rot180_SC_TC
					>(blitArgs);
					}
				} break;

			case Rotate_270:
				{
				if (modulate==0xffff)
					{
					Blitter::Rot270<
						Blitter::OpCopy_SC_TC,
						Blitter::StepX_Rot270_SC_TC,
						Blitter::StepY_Rot270_SC_TC
					>(blitArgs);
					}
				else
					{
					Blitter::Rot270<
						Blitter::OpCopy_Color_SC_TC,
						Blitter::StepX_Rot270_SC_TC,
						Blitter::StepY_Rot270_SC_TC
					>(blitArgs);
					}
				} break;

			case Mirror_X:
				{
				if (modulate==0xffff)
					{
					Blitter::Flip_X<
						Blitter::OpCopy_SC_TC,
						Blitter::StepX_Flip_X_SC_TC,
						Blitter::StepY_Flip_X_SC_TC
					>(blitArgs);
					}
				else
					{
					Blitter::Flip_X<
						Blitter::OpCopy_Color_SC_TC,
						Blitter::StepX_Flip_X_SC_TC,
						Blitter::StepY_Flip_X_SC_TC
					>(blitArgs);
					}
				} break;

			case Mirror_Y:
				{
				if (modulate==0xffff)
					{
					Blitter::Flip_Y_Copy_SC_TC(blitArgs);
					}
				else
					{
					Blitter::Flip_Y<
						Blitter::OpCopy_Color_SC_TC,
						Blitter::StepX_Flip_Y_SC_TC,
						Blitter::StepY_Flip_Y_SC_TC
					>(blitArgs);
					}
				} break;

			}
		}

	// Copy alpha channel, if we have one
	if (sourceAlpha && targetAlpha)
		{
		// Pick the right copy type for the specified transformation
		switch (transformation)
			{
			case NoTransformation:
				{
				Blitter::Normal_Copy_SA_TA(blitArgs);
				} break;

			case Rotate_90:
				{
				Blitter::Rot090<
					Blitter::OpCopy_SA_TA,
					Blitter::StepX_Rot090_SA_TA,
					Blitter::StepY_Rot090_SA_TA
				>(blitArgs);
				} break;

			case Rotate_180:
				{
				Blitter::Rot180<
					Blitter::OpCopy_SA_TA,
					Blitter::StepX_Rot180_SA_TA,
					Blitter::StepY_Rot180_SA_TA
				>(blitArgs);
				} break;

			case Rotate_270:
				{
				Blitter::Rot270<
					Blitter::OpCopy_SA_TA,
					Blitter::StepX_Rot270_SA_TA,
					Blitter::StepY_Rot270_SA_TA
				>(blitArgs);
				} break;

			case Mirror_X:
				{
				Blitter::Flip_X<
					Blitter::OpCopy_SA_TA,
					Blitter::StepX_Flip_X_SA_TA,
					Blitter::StepY_Flip_X_SA_TA
				>(blitArgs);
				} break;

			case Mirror_Y:
				{
				Blitter::Flip_Y_Copy_SA_TA(blitArgs);
				} break;
			}
		}


	// If we don't have color, but target does, fill it with solid color
	if (!sourceColor && targetColor)
		{
		Blitter::Normal<
			Blitter::OpFill_Color_TC,
			Blitter::StepX_Normal_TC,
			Blitter::StepY_Normal_TC
		>(blitArgs);
		}

	// If target has an alpha channel, fill it with full opaque
	if (!sourceAlpha && targetAlpha)
		{
		Blitter::Normal_Fill_Alpha_TA(blitArgs);
		}

	}


