//*** Blitter.inl ***

#include "ColorHelper.h"

//*** OpFill_Color_TC ***

void Blitter::OpFill_Color_TC(OpArgs& opArgs)
	{
	// targetColor is filled with color
	*opArgs.targetColor=opArgs.color;
	}


//*** OpFill_Alpha_TA ***

void Blitter::OpFill_Alpha_TA(OpArgs& opArgs)
	{
	// targetAlpha is filled with alpha
	*opArgs.targetAlpha=opArgs.alpha;
	}


//*** OpCopy_SC_TC ***

void Blitter::OpCopy_SC_TC(OpArgs& opArgs)
	{
	// sourceColor is copied to targetColor
	*opArgs.targetColor=*opArgs.sourceColor;
	}


//*** OpCopy_Color_SC_TC ***

void Blitter::OpCopy_Color_SC_TC(OpArgs& opArgs)
	{
	// sourceColor*color is copied to targetColor
	*opArgs.targetColor=RGBModulate16(*opArgs.sourceColor,opArgs.color);
	}


//*** OpCopy_SA_TA ***

void Blitter::OpCopy_SA_TA(OpArgs& opArgs)
	{
	// sourceAlpha is copied to targetAlpha
	*opArgs.targetAlpha=*opArgs.sourceAlpha;
	}


//*** OpBlend_SC_SA_TC ***

void Blitter::OpBlend_SC_SA_TC(OpArgs& opArgs)
	{
	// sourceColor is blended (sourceAlpha) with targetColor
	*opArgs.targetColor=AlphaBlend16(*opArgs.targetColor,*opArgs.sourceColor,*opArgs.sourceAlpha);
	}


//*** OpBlend_Color_SC_SA_TC ***

void Blitter::OpBlend_Color_SC_SA_TC(OpArgs& opArgs)
	{
	// sourceColor*color is blended (sourceAlpha) with targetColor
	unsigned short color=RGBModulate16(*opArgs.sourceColor,opArgs.color);
	*opArgs.targetColor=AlphaBlend16(*opArgs.targetColor,color,*opArgs.sourceAlpha);
	}


//*** OpBlend_Alpha_SC_TC ***

void Blitter::OpBlend_Alpha_SC_TC(OpArgs& opArgs)
	{
	// sourceColor is blended (alpha) with targetColor
	*opArgs.targetColor=AlphaBlend16(*opArgs.targetColor,*opArgs.sourceColor,opArgs.alpha);
	}


//*** OpBlend_Color_Alpha_SC_TC ***

void Blitter::OpBlend_Color_Alpha_SC_TC(OpArgs& opArgs)
	{
	// sourceColor*color is blended (alpha) with targetColor
	unsigned short color=RGBModulate16(*opArgs.sourceColor,opArgs.color);
	*opArgs.targetColor=AlphaBlend16(*opArgs.targetColor,color,opArgs.alpha);
	}


//*** OpBlend_Alpha_SC_SA_TC ***

void Blitter::OpBlend_Alpha_SC_SA_TC(OpArgs& opArgs)
	{
	// sourceColor is blended (sourceAlpha*alpha) with targetColor
	unsigned char alpha=((*opArgs.sourceAlpha)*opArgs.alpha)>>8;
	*opArgs.targetColor=AlphaBlend16(*opArgs.targetColor,*opArgs.sourceColor,alpha);
	}


//*** OpBlend_Color_Alpha_SC_SA_TC ***

void Blitter::OpBlend_Color_Alpha_SC_SA_TC(OpArgs& opArgs)
	{
	// sourceColor*color is blended (sourceAlpha*alpha) with targetColor
	unsigned char alpha=((*opArgs.sourceAlpha)*opArgs.alpha)>>8;
	unsigned short color=RGBModulate16(*opArgs.sourceColor,opArgs.color);
	*opArgs.targetColor=AlphaBlend16(*opArgs.targetColor,color,alpha);
	}


//*** OpBlend_Color_Alpha_TC ***

void Blitter::OpBlend_Color_Alpha_TC(OpArgs& opArgs)
	{
	// color is blended (alpha) with targetColor
	*opArgs.targetColor=AlphaBlend16(*opArgs.targetColor,opArgs.color,opArgs.alpha);
	}


//*** OpBlend_Color_SA_TC ***

void Blitter::OpBlend_Color_SA_TC(OpArgs& opArgs)
	{
	// color is blended (sourceAlpha) with targetColor
	*opArgs.targetColor=AlphaBlend16(*opArgs.targetColor,opArgs.color,*opArgs.sourceAlpha);
	}


//*** OpBlend_Color_Alpha_SA_TC ***

void Blitter::OpBlend_Color_Alpha_SA_TC(OpArgs& opArgs)
	{
	// color is blended (sourceAlpha*alpha) with targetColor
	unsigned char alpha=((*opArgs.sourceAlpha)*opArgs.alpha)>>8;
	*opArgs.targetColor=AlphaBlend16(*opArgs.targetColor,opArgs.color,alpha);
	}


//*** OpMax_Alpha_TA ***

void Blitter::OpMax_Alpha_TA(OpArgs& opArgs)
	{
	// targetAlpha is set to max(alpha,targetAlpha)
	if (opArgs.alpha>*opArgs.targetAlpha)
		{
		*opArgs.targetAlpha=opArgs.alpha;
		}
	}


//*** OpMax_SA_TA ***

void Blitter::OpMax_SA_TA(OpArgs& opArgs)
	{
	// targetAlpha is set to max(sourceAlpha,targetAlpha)
	if (*opArgs.sourceAlpha>*opArgs.targetAlpha)
		{
		*opArgs.targetAlpha=*opArgs.sourceAlpha;
		}
	}


//*** OpMax_Alpha_SA_TA ***

void Blitter::OpMax_Alpha_SA_TA(OpArgs& opArgs)
	{
	// targetAlpha is set to max(sourceAlpha*alpha,targetAlpha)
	unsigned char alpha=((*opArgs.sourceAlpha)*opArgs.alpha)>>8;
	if (alpha>*opArgs.targetAlpha)
		{
		*opArgs.targetAlpha=alpha;
		}
	}


//*** Normal ***

template<Blitter::Operation operation, Blitter::Step stepX, Blitter::Step stepY> 
static void Blitter::Normal(BlitArgs& blitArgs)
	{ 
	int sourceStart=blitArgs.sourceX+blitArgs.sourceY*blitArgs.sourceHPitch;
	int targetStart=blitArgs.targetX+blitArgs.targetY*blitArgs.targetHPitch;

	OpArgs opArgs;
	opArgs.color=blitArgs.color;
	opArgs.alpha=blitArgs.alpha;
	opArgs.sourceColor=&blitArgs.sourceColor[sourceStart];
	opArgs.sourceAlpha=&blitArgs.sourceAlpha[sourceStart];
	opArgs.targetColor=&blitArgs.targetColor[targetStart];
	opArgs.targetAlpha=&blitArgs.targetAlpha[targetStart];
	
	StepArgs stepArgs;
	stepArgs.sourceRowStep=blitArgs.sourceHPitch-blitArgs.sourceWidth;
	stepArgs.targetRowStep=blitArgs.targetHPitch-blitArgs.sourceWidth;

	for (int y=0; y<blitArgs.sourceHeight; y++)
		{
		for (int x=0; x<blitArgs.sourceWidth; x++)
			{
			operation(opArgs);

			stepX(opArgs,stepArgs);
			}
	
		stepY(opArgs,stepArgs);
		}
	}


//**** Rot090 ***

template<Blitter::Operation operation, Blitter::Step stepX, Blitter::Step stepY> 
static void Blitter::Rot090(BlitArgs& blitArgs)
	{ 
	int sourceStart=blitArgs.sourceY+((blitArgs.sourceVPitch-1)-blitArgs.sourceX)*blitArgs.sourceHPitch;
	int targetStart=blitArgs.targetX+blitArgs.targetY*blitArgs.targetHPitch;

	OpArgs opArgs;
	opArgs.color=blitArgs.color;
	opArgs.alpha=blitArgs.alpha;
	opArgs.sourceColor=&blitArgs.sourceColor[sourceStart];
	opArgs.sourceAlpha=&blitArgs.sourceAlpha[sourceStart];
	opArgs.targetColor=&blitArgs.targetColor[targetStart];
	opArgs.targetAlpha=&blitArgs.targetAlpha[targetStart];
	
	StepArgs stepArgs;
	stepArgs.sourceRowStep=blitArgs.sourceWidth*blitArgs.sourceHPitch+1;
	stepArgs.targetRowStep=blitArgs.targetHPitch-blitArgs.sourceWidth;
	stepArgs.sourceHPitch=blitArgs.sourceHPitch;


	for (int y=0; y<blitArgs.sourceHeight; y++)
		{
		for (int x=0; x<blitArgs.sourceWidth; x++)
			{
			operation(opArgs);

			stepX(opArgs,stepArgs);
			}

		stepY(opArgs,stepArgs);
		}
	}


//**** Rot180 ***

template<Blitter::Operation operation, Blitter::Step stepX, Blitter::Step stepY> 
static void Blitter::Rot180(BlitArgs& blitArgs)
	{ 
	int sourceStart=(blitArgs.sourceHPitch-1)-blitArgs.sourceX+((blitArgs.sourceVPitch-1)-blitArgs.sourceY)*blitArgs.sourceHPitch;
	int targetStart=blitArgs.targetX+blitArgs.targetY*blitArgs.targetHPitch;

	OpArgs opArgs;
	opArgs.color=blitArgs.color;
	opArgs.alpha=blitArgs.alpha;
	opArgs.sourceColor=&blitArgs.sourceColor[sourceStart];
	opArgs.sourceAlpha=&blitArgs.sourceAlpha[sourceStart];
	opArgs.targetColor=&blitArgs.targetColor[targetStart];
	opArgs.targetAlpha=&blitArgs.targetAlpha[targetStart];
	
	StepArgs stepArgs;
	stepArgs.sourceRowStep=-(blitArgs.sourceHPitch-blitArgs.sourceWidth);
	stepArgs.targetRowStep=blitArgs.targetHPitch-blitArgs.sourceWidth;
	stepArgs.sourceHPitch=blitArgs.sourceHPitch;

	for (int y=0; y<blitArgs.sourceHeight; y++)
		{
		for (int x=0; x<blitArgs.sourceWidth; x++)
			{
			operation(opArgs);

			stepX(opArgs,stepArgs);
			}

		stepY(opArgs,stepArgs);
		}
	}


//**** Rot270 ***

template<Blitter::Operation operation, Blitter::Step stepX, Blitter::Step stepY> 
static void Blitter::Rot270(BlitArgs& blitArgs)
	{ 
	int sourceStart=(blitArgs.sourceHPitch-1-blitArgs.sourceY)+blitArgs.sourceX*blitArgs.sourceHPitch;
	int targetStart=blitArgs.targetX+blitArgs.targetY*blitArgs.targetHPitch;

	OpArgs opArgs;
	opArgs.color=blitArgs.color;
	opArgs.alpha=blitArgs.alpha;
	opArgs.sourceColor=&blitArgs.sourceColor[sourceStart];
	opArgs.sourceAlpha=&blitArgs.sourceAlpha[sourceStart];
	opArgs.targetColor=&blitArgs.targetColor[targetStart];
	opArgs.targetAlpha=&blitArgs.targetAlpha[targetStart];
	
	StepArgs stepArgs;
	stepArgs.sourceRowStep=-(blitArgs.sourceWidth*blitArgs.sourceHPitch+1);
	stepArgs.targetRowStep=blitArgs.targetHPitch-blitArgs.sourceWidth;
	stepArgs.sourceHPitch=blitArgs.sourceHPitch;

	for (int y=0; y<blitArgs.sourceHeight; y++)
		{
		for (int x=0; x<blitArgs.sourceWidth; x++)
			{
			operation(opArgs);

			stepX(opArgs,stepArgs);
			}

		stepY(opArgs,stepArgs);
		}
	}


//**** Flip_X ***

template<Blitter::Operation operation, Blitter::Step stepX, Blitter::Step stepY> 
static void Blitter::Flip_X(BlitArgs& blitArgs)
	{ 
	int sourceStart=(blitArgs.sourceHPitch-1)-blitArgs.sourceX+blitArgs.sourceY*blitArgs.sourceHPitch;
	int targetStart=blitArgs.targetX+blitArgs.targetY*blitArgs.targetHPitch;

	OpArgs opArgs;
	opArgs.color=blitArgs.color;
	opArgs.alpha=blitArgs.alpha;
	opArgs.sourceColor=&blitArgs.sourceColor[sourceStart];
	opArgs.sourceAlpha=&blitArgs.sourceAlpha[sourceStart];
	opArgs.targetColor=&blitArgs.targetColor[targetStart];
	opArgs.targetAlpha=&blitArgs.targetAlpha[targetStart];
	
	StepArgs stepArgs;
	stepArgs.sourceRowStep=blitArgs.sourceHPitch+blitArgs.sourceWidth;
	stepArgs.targetRowStep=blitArgs.targetHPitch-blitArgs.sourceWidth;
	stepArgs.sourceHPitch=blitArgs.sourceHPitch;

	for (int y=0; y<blitArgs.sourceHeight; y++)
		{
		for (int x=0; x<blitArgs.sourceWidth; x++)
			{
			operation(opArgs);

			stepX(opArgs,stepArgs);
			}

		stepY(opArgs,stepArgs);
		}
	}


//**** Flip_Y ***

template<Blitter::Operation operation, Blitter::Step stepX, Blitter::Step stepY> 
static void Blitter::Flip_Y(BlitArgs& blitArgs)
	{ 
	int sourceStart=blitArgs.sourceX+((blitArgs.sourceVPitch-1)-blitArgs.sourceY)*blitArgs.sourceHPitch;
	int targetStart=blitArgs.targetX+blitArgs.targetY*blitArgs.targetHPitch;

	OpArgs opArgs;
	opArgs.color=blitArgs.color;
	opArgs.alpha=blitArgs.alpha;
	opArgs.sourceColor=&blitArgs.sourceColor[sourceStart];
	opArgs.sourceAlpha=&blitArgs.sourceAlpha[sourceStart];
	opArgs.targetColor=&blitArgs.targetColor[targetStart];
	opArgs.targetAlpha=&blitArgs.targetAlpha[targetStart];
	
	StepArgs stepArgs;
	stepArgs.sourceRowStep=-(blitArgs.sourceHPitch+blitArgs.sourceWidth);
	stepArgs.targetRowStep=blitArgs.targetHPitch-blitArgs.sourceWidth;
	stepArgs.sourceHPitch=blitArgs.sourceHPitch;

	for (int y=0; y<blitArgs.sourceHeight; y++)
		{
		for (int x=0; x<blitArgs.sourceWidth; x++)
			{
			operation(opArgs);

			stepX(opArgs,stepArgs);
			}

		stepY(opArgs,stepArgs);
		}
	}


// targetAlpha is filled with alpha
void Blitter::Normal_Fill_Alpha_TA(BlitArgs& blitArgs)
	{
	// Fill with a single memset when we can
	if (blitArgs.sourceWidth==blitArgs.targetHPitch)
		{
		unsigned char* destination=&blitArgs.targetAlpha[blitArgs.targetX+blitArgs.targetY*blitArgs.targetHPitch];
		MemSet(destination,blitArgs.alpha,blitArgs.sourceWidth*blitArgs.sourceHeight);
		return;
		}

	// Fill row by row
	unsigned char* destination=&blitArgs.targetAlpha[blitArgs.targetX+blitArgs.targetY*blitArgs.targetHPitch];
	int rowStep=blitArgs.targetHPitch;

	for (int y=0; y<blitArgs.sourceHeight; y++)
		{
		MemSet(destination,blitArgs.alpha,blitArgs.sourceWidth);

		destination+=rowStep; // Step to the next row
		}
	}

// sourceColor is copied to targetColor
void Blitter::Normal_Copy_SC_TC(BlitArgs& blitArgs)
	{
	/// Use a single memcpy when we can
	if (blitArgs.sourceHPitch==blitArgs.sourceWidth && blitArgs.targetHPitch==blitArgs.sourceWidth)
		{
		MemCpy(	&blitArgs.targetColor[blitArgs.targetX+blitArgs.targetY*blitArgs.targetHPitch],
				&blitArgs.sourceColor[blitArgs.sourceX+blitArgs.sourceY*blitArgs.sourceHPitch],
				2*blitArgs.sourceWidth*blitArgs.sourceHeight
				);
		return;
		}

	unsigned short* source=&blitArgs.sourceColor[blitArgs.sourceX+blitArgs.sourceY*blitArgs.sourceHPitch];
	unsigned short* destination=&blitArgs.targetColor[blitArgs.targetX+blitArgs.targetY*blitArgs.targetHPitch];
	int rowSize=blitArgs.sourceWidth*2;

	for (int y=0; y<blitArgs.sourceHeight; y++)
		{
		MemCpy(destination,source,rowSize);
		source+=blitArgs.sourceHPitch;
		destination+=blitArgs.targetHPitch;
		}
	}


// sourceColor is copied to targetColor
void Blitter::Flip_Y_Copy_SC_TC(BlitArgs& blitArgs)
	{
	unsigned short* source=&blitArgs.sourceColor[blitArgs.sourceX+((blitArgs.sourceVPitch-1)-blitArgs.sourceY)*blitArgs.sourceHPitch];
	unsigned short* destination=&blitArgs.targetColor[blitArgs.targetX+blitArgs.targetY*blitArgs.targetHPitch];
	int rowSize=blitArgs.sourceWidth*2;
	int sourceRowStep=-blitArgs.sourceHPitch;
	int destinationRowStep=blitArgs.targetHPitch;

	for (int y=0; y<blitArgs.sourceHeight; y++)
		{
		MemCpy(destination,source,rowSize);
		source+=sourceRowStep;
		destination+=destinationRowStep;
		}
	}


// sourceAlpha is copied to targetAlpha
void Blitter::Normal_Copy_SA_TA(BlitArgs& blitArgs)
	{
	/// Use a single memcpy when we can
	if (blitArgs.sourceHPitch==blitArgs.sourceWidth && blitArgs.targetHPitch==blitArgs.sourceWidth)
		{
		MemCpy(	&blitArgs.targetAlpha[blitArgs.targetX+blitArgs.targetY*blitArgs.targetHPitch],
				&blitArgs.sourceAlpha[blitArgs.sourceX+blitArgs.sourceY*blitArgs.sourceHPitch],
				blitArgs.sourceWidth*blitArgs.sourceHeight)
				;
		return;
		}

	unsigned char* source=&blitArgs.sourceAlpha[blitArgs.sourceX+blitArgs.sourceY*blitArgs.sourceHPitch];
	unsigned char* destination=&blitArgs.targetAlpha[blitArgs.targetX+blitArgs.targetY*blitArgs.targetHPitch];

	for (int y=0; y<blitArgs.sourceHeight; y++)
		{
		MemCpy(destination,source,blitArgs.sourceWidth);
		source+=blitArgs.sourceHPitch;
		destination+=blitArgs.targetHPitch;
		}
	}


// sourceAlpha is copied to targetAlpha
void Blitter::Flip_Y_Copy_SA_TA(BlitArgs& blitArgs)
	{
	unsigned char* source=&blitArgs.sourceAlpha[blitArgs.sourceX+((blitArgs.sourceVPitch-1)-blitArgs.sourceY)*blitArgs.sourceHPitch];
	unsigned char* destination=&blitArgs.targetAlpha[blitArgs.targetX+blitArgs.targetY*blitArgs.targetHPitch];
	int sourceRowStep=-blitArgs.sourceHPitch;
	int destinationRowStep=blitArgs.targetHPitch;

	for (int y=0; y<blitArgs.sourceHeight; y++)
		{
		MemCpy(destination,source,blitArgs.sourceWidth);
		source+=sourceRowStep;
		destination+=destinationRowStep;
		}
	}
