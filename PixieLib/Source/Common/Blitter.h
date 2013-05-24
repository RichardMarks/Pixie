
/**
 * \class	Blitter
 * 
 * \ingroup	graphics
 * \brief	Helper class for bitmap blitting
 * \author	Mattias Gustavsson	
 * 
 * \todo	Additive blitting + more blend modes
 */

#ifndef __Blitter_H__
#define __Blitter_H__

// Includes

// Forward declares

// Blitter
class Blitter
	{
	#define INLINE __forceinline static
	public:
		struct OpArgs;
		struct StepArgs;
		typedef void (*Operation)(OpArgs& opArgs);
		typedef void (*Step)(OpArgs& opArgs, StepArgs& stepArgs);

		struct BlitArgs
			{
			unsigned short color;
			unsigned char alpha;
			unsigned short* sourceColor;
			unsigned char* sourceAlpha;
			unsigned short* targetColor;
			unsigned char* targetAlpha;
			int sourceHPitch;
			int sourceVPitch;
			int targetHPitch;
			int sourceX;
			int sourceY;
			int sourceWidth;
			int sourceHeight;
			int targetX;
			int targetY;
			};

		template<Operation operation, Step stepX, Step stepY> static void Normal(BlitArgs& blitArgs);

		template<Operation operation, Step stepX, Step stepY> static void Rot090(BlitArgs& blitArgs);

		template<Operation operation, Step stepX, Step stepY> static void Rot180(BlitArgs& blitArgs);

		template<Operation operation, Step stepX, Step stepY> static void Rot270(BlitArgs& blitArgs);		

		template<Operation operation, Step stepX, Step stepY> static void Flip_X(BlitArgs& blitArgs);		

		template<Operation operation, Step stepX, Step stepY> static void Flip_Y(BlitArgs& blitArgs);

		// targetAlpha is filled with alpha
		inline static void Normal_Fill_Alpha_TA(BlitArgs& blitArgs);

		// sourceColor is copied to targetColor
		inline static void Normal_Copy_SC_TC(BlitArgs& blitArgs);
	
		// sourceColor is copied to targetColor
		inline static void Flip_Y_Copy_SC_TC(BlitArgs& blitArgs);

		// sourceAlpha is copied to targetAlpha
		inline static void Normal_Copy_SA_TA(BlitArgs& blitArgs);

		// sourceAlpha is copied to targetAlpha
		inline static void Flip_Y_Copy_SA_TA(BlitArgs& blitArgs);

		struct OpArgs
			{
			unsigned short color;
			unsigned char alpha;
			unsigned short* sourceColor;
			unsigned char* sourceAlpha;
			unsigned short* targetColor;
			unsigned char* targetAlpha;
			};

		// targetColor is filled with color
		INLINE void OpFill_Color_TC(OpArgs& opArgs);

		// targetAlpha is filled with alpha
		INLINE void OpFill_Alpha_TA(OpArgs& opArgs);

		// sourceColor is copied to targetColor
		INLINE void OpCopy_SC_TC(OpArgs& opArgs);

		// sourceColor*color is copied to targetColor
		INLINE void OpCopy_Color_SC_TC(OpArgs& opArgs);

		// sourceAlpha is copied to targetAlpha
		INLINE void OpCopy_SA_TA(OpArgs& opArgs);

		// sourceColor is blended (sourceAlpha) with targetColor
		INLINE void OpBlend_SC_SA_TC(OpArgs& opArgs);

		// sourceColor*color is blended (sourceAlpha) with targetColor
		INLINE void OpBlend_Color_SC_SA_TC(OpArgs& opArgs);

		// sourceColor is blended (alpha) with targetColor
		INLINE void OpBlend_Alpha_SC_TC(OpArgs& opArgs);

		// sourceColor*color is blended (alpha) with targetColor
		INLINE void OpBlend_Color_Alpha_SC_TC(OpArgs& opArgs);

		// sourceColor is blended (sourceAlpha*alpha) with targetColor
		INLINE void OpBlend_Alpha_SC_SA_TC(OpArgs& opArgs);

		// sourceColor*color is blended (sourceAlpha*alpha) with targetColor
		INLINE void OpBlend_Color_Alpha_SC_SA_TC(OpArgs& opArgs);

		// color is blended (alpha) with targetColor
		INLINE void OpBlend_Color_Alpha_TC(OpArgs& opArgs);

		// color is blended (sourceAlpha) with targetColor
		INLINE void OpBlend_Color_SA_TC(OpArgs& opArgs);

		// color is blended (sourceAlpha*alpha) with targetColor
		INLINE void OpBlend_Color_Alpha_SA_TC(OpArgs& opArgs);

		// targetAlpha is set to max(alpha,targetAlpha)
		INLINE void OpMax_Alpha_TA(OpArgs& opArgs);

		// targetAlpha is set to max(sourceAlpha,targetAlpha)
		INLINE void OpMax_SA_TA(OpArgs& opArgs);

		// targetAlpha is set to max(sourceAlpha*alpha,targetAlpha)
		INLINE void OpMax_Alpha_SA_TA(OpArgs& opArgs);

		struct StepArgs
			{
			int sourceRowStep;
			int targetRowStep;
			int sourceHPitch;
			};


		#define DefArgs OpArgs& opArgs, StepArgs& stepArgs
		#define PassArgs opArgs, stepArgs

		INLINE void StepX_Normal_TC			(DefArgs) { StepFunc<X_Normal_TC>							(PassArgs); }; 
		INLINE void StepY_Normal_TC			(DefArgs) { StepFunc<Y_Normal_TC>							(PassArgs); }; 
		INLINE void StepX_Normal_TA			(DefArgs) { StepFunc<X_Normal_TA>							(PassArgs); }; 
		INLINE void StepY_Normal_TA			(DefArgs) { StepFunc<Y_Normal_TA>							(PassArgs); }; 
		INLINE void StepX_Normal_SC_TC		(DefArgs) { StepFunc<X_Normal_SC, X_Normal_TC>				(PassArgs); }; 
		INLINE void StepY_Normal_SC_TC		(DefArgs) { StepFunc<Y_Normal_SC, Y_Normal_TC>				(PassArgs); }; 
		INLINE void StepX_Normal_SA_TA		(DefArgs) { StepFunc<X_Normal_SA, X_Normal_TA>				(PassArgs); }; 
		INLINE void StepY_Normal_SA_TA		(DefArgs) { StepFunc<Y_Normal_SA, Y_Normal_TA>				(PassArgs); }; 
		INLINE void StepX_Normal_SA_TC		(DefArgs) { StepFunc<X_Normal_SA, X_Normal_TC>				(PassArgs); }; 
		INLINE void StepY_Normal_SA_TC		(DefArgs) { StepFunc<Y_Normal_SA, Y_Normal_TC>				(PassArgs); }; 
		INLINE void StepX_Normal_SC_SA_TC	(DefArgs) { StepFunc<X_Normal_SC, X_Normal_SA, X_Normal_TC>	(PassArgs); }; 
		INLINE void StepY_Normal_SC_SA_TC	(DefArgs) { StepFunc<Y_Normal_SC, Y_Normal_SA, Y_Normal_TC>	(PassArgs); }; 

		INLINE void StepX_Rot090_TC			(DefArgs) { StepFunc<X_Rot090_TC>							(PassArgs); }; 
		INLINE void StepY_Rot090_TC			(DefArgs) { StepFunc<Y_Rot090_TC>							(PassArgs); }; 
		INLINE void StepX_Rot090_TA			(DefArgs) { StepFunc<X_Rot090_TA>							(PassArgs); }; 
		INLINE void StepY_Rot090_TA			(DefArgs) { StepFunc<Y_Rot090_TA>							(PassArgs); }; 
		INLINE void StepX_Rot090_SC_TC		(DefArgs) { StepFunc<X_Rot090_SC, X_Rot090_TC>				(PassArgs); }; 
		INLINE void StepY_Rot090_SC_TC		(DefArgs) { StepFunc<Y_Rot090_SC, Y_Rot090_TC>				(PassArgs); }; 
		INLINE void StepX_Rot090_SA_TA		(DefArgs) { StepFunc<X_Rot090_SA, X_Rot090_TA>				(PassArgs); }; 
		INLINE void StepY_Rot090_SA_TA		(DefArgs) { StepFunc<Y_Rot090_SA, Y_Rot090_TA>				(PassArgs); }; 
		INLINE void StepX_Rot090_SA_TC		(DefArgs) { StepFunc<X_Rot090_SA, X_Rot090_TC>				(PassArgs); }; 
		INLINE void StepY_Rot090_SA_TC		(DefArgs) { StepFunc<Y_Rot090_SA, Y_Rot090_TC>				(PassArgs); }; 
		INLINE void StepX_Rot090_SC_SA_TC	(DefArgs) { StepFunc<X_Rot090_SC, X_Rot090_SA, X_Rot090_TC>	(PassArgs); }; 
		INLINE void StepY_Rot090_SC_SA_TC	(DefArgs) { StepFunc<Y_Rot090_SC, Y_Rot090_SA, Y_Rot090_TC>	(PassArgs); }; 
		

		INLINE void StepX_Rot180_TC			(DefArgs) { StepFunc<X_Rot180_TC>							(PassArgs); }; 
		INLINE void StepY_Rot180_TC			(DefArgs) { StepFunc<Y_Rot180_TC>							(PassArgs); }; 
		INLINE void StepX_Rot180_TA			(DefArgs) { StepFunc<X_Rot180_TA>							(PassArgs); }; 
		INLINE void StepY_Rot180_TA			(DefArgs) { StepFunc<Y_Rot180_TA>							(PassArgs); }; 
		INLINE void StepX_Rot180_SC_TC		(DefArgs) { StepFunc<X_Rot180_SC, X_Rot180_TC>				(PassArgs); }; 
		INLINE void StepY_Rot180_SC_TC		(DefArgs) { StepFunc<Y_Rot180_SC, Y_Rot180_TC>				(PassArgs); }; 
		INLINE void StepX_Rot180_SA_TA		(DefArgs) { StepFunc<X_Rot180_SA, X_Rot180_TA>				(PassArgs); }; 
		INLINE void StepY_Rot180_SA_TA		(DefArgs) { StepFunc<Y_Rot180_SA, Y_Rot180_TA>				(PassArgs); }; 
		INLINE void StepX_Rot180_SA_TC		(DefArgs) { StepFunc<X_Rot180_SA, X_Rot180_TC>				(PassArgs); }; 
		INLINE void StepY_Rot180_SA_TC		(DefArgs) { StepFunc<Y_Rot180_SA, Y_Rot180_TC>				(PassArgs); }; 
		INLINE void StepX_Rot180_SC_SA_TC	(DefArgs) { StepFunc<X_Rot180_SC, X_Rot180_SA, X_Rot180_TC>	(PassArgs); }; 
		INLINE void StepY_Rot180_SC_SA_TC	(DefArgs) { StepFunc<Y_Rot180_SC, Y_Rot180_SA, Y_Rot180_TC>	(PassArgs); }; 
		
		INLINE void StepX_Rot270_TC			(DefArgs) { StepFunc<X_Rot270_TC>							(PassArgs); }; 
		INLINE void StepY_Rot270_TC			(DefArgs) { StepFunc<Y_Rot270_TC>							(PassArgs); }; 
		INLINE void StepX_Rot270_TA			(DefArgs) { StepFunc<X_Rot270_TA>							(PassArgs); }; 
		INLINE void StepY_Rot270_TA			(DefArgs) { StepFunc<Y_Rot270_TA>							(PassArgs); }; 
		INLINE void StepX_Rot270_SC_TC		(DefArgs) { StepFunc<X_Rot270_SC, X_Rot270_TC>				(PassArgs); }; 
		INLINE void StepY_Rot270_SC_TC		(DefArgs) { StepFunc<Y_Rot270_SC, Y_Rot270_TC>				(PassArgs); }; 
		INLINE void StepX_Rot270_SA_TA		(DefArgs) { StepFunc<X_Rot270_SA, X_Rot270_TA>				(PassArgs); }; 
		INLINE void StepY_Rot270_SA_TA		(DefArgs) { StepFunc<Y_Rot270_SA, Y_Rot270_TA>				(PassArgs); }; 
		INLINE void StepX_Rot270_SA_TC		(DefArgs) { StepFunc<X_Rot270_SA, X_Rot270_TC>				(PassArgs); }; 
		INLINE void StepY_Rot270_SA_TC		(DefArgs) { StepFunc<Y_Rot270_SA, Y_Rot270_TC>				(PassArgs); }; 
		INLINE void StepX_Rot270_SC_SA_TC	(DefArgs) { StepFunc<X_Rot270_SC, X_Rot270_SA, X_Rot270_TC>	(PassArgs); }; 
		INLINE void StepY_Rot270_SC_SA_TC	(DefArgs) { StepFunc<Y_Rot270_SC, Y_Rot270_SA, Y_Rot270_TC>	(PassArgs); }; 
		
		INLINE void StepX_Flip_X_TC			(DefArgs) { StepFunc<X_Flip_X_TC>							(PassArgs); }; 
		INLINE void StepY_Flip_X_TC			(DefArgs) { StepFunc<Y_Flip_X_TC>							(PassArgs); }; 
		INLINE void StepX_Flip_X_TA			(DefArgs) { StepFunc<X_Flip_X_TA>							(PassArgs); }; 
		INLINE void StepY_Flip_X_TA			(DefArgs) { StepFunc<Y_Flip_X_TA>							(PassArgs); }; 
		INLINE void StepX_Flip_X_SC_TC		(DefArgs) { StepFunc<X_Flip_X_SC, X_Flip_X_TC>				(PassArgs); }; 
		INLINE void StepY_Flip_X_SC_TC		(DefArgs) { StepFunc<Y_Flip_X_SC, Y_Flip_X_TC>				(PassArgs); }; 
		INLINE void StepX_Flip_X_SA_TA		(DefArgs) { StepFunc<X_Flip_X_SA, X_Flip_X_TA>				(PassArgs); }; 
		INLINE void StepY_Flip_X_SA_TA		(DefArgs) { StepFunc<Y_Flip_X_SA, Y_Flip_X_TA>				(PassArgs); }; 
		INLINE void StepX_Flip_X_SA_TC		(DefArgs) { StepFunc<X_Flip_X_SA, X_Flip_X_TC>				(PassArgs); }; 
		INLINE void StepY_Flip_X_SA_TC		(DefArgs) { StepFunc<Y_Flip_X_SA, Y_Flip_X_TC>				(PassArgs); }; 
		INLINE void StepX_Flip_X_SC_SA_TC	(DefArgs) { StepFunc<X_Flip_X_SC, X_Flip_X_SA, X_Flip_X_TC>	(PassArgs); }; 
		INLINE void StepY_Flip_X_SC_SA_TC	(DefArgs) { StepFunc<Y_Flip_X_SC, Y_Flip_X_SA, Y_Flip_X_TC>	(PassArgs); }; 
		
		INLINE void StepX_Flip_Y_TC			(DefArgs) { StepFunc<X_Flip_Y_TC>							(PassArgs); }; 
		INLINE void StepY_Flip_Y_TC			(DefArgs) { StepFunc<Y_Flip_Y_TC>							(PassArgs); }; 
		INLINE void StepX_Flip_Y_TA			(DefArgs) { StepFunc<X_Flip_Y_TA>							(PassArgs); }; 
		INLINE void StepY_Flip_Y_TA			(DefArgs) { StepFunc<Y_Flip_Y_TA>							(PassArgs); }; 
		INLINE void StepX_Flip_Y_SC_TC		(DefArgs) { StepFunc<X_Flip_Y_SC, X_Flip_Y_TC>				(PassArgs); }; 
		INLINE void StepY_Flip_Y_SC_TC		(DefArgs) { StepFunc<Y_Flip_Y_SC, Y_Flip_Y_TC>				(PassArgs); }; 
		INLINE void StepX_Flip_Y_SA_TA		(DefArgs) { StepFunc<X_Flip_Y_SA, X_Flip_Y_TA>				(PassArgs); }; 
		INLINE void StepY_Flip_Y_SA_TA		(DefArgs) { StepFunc<Y_Flip_Y_SA, Y_Flip_Y_TA>				(PassArgs); }; 
		INLINE void StepX_Flip_Y_SA_TC		(DefArgs) { StepFunc<X_Flip_Y_SA, X_Flip_Y_TC>				(PassArgs); }; 
		INLINE void StepY_Flip_Y_SA_TC		(DefArgs) { StepFunc<Y_Flip_Y_SA, Y_Flip_Y_TC>				(PassArgs); }; 
		INLINE void StepX_Flip_Y_SC_SA_TC	(DefArgs) { StepFunc<X_Flip_Y_SC, X_Flip_Y_SA, X_Flip_Y_TC>	(PassArgs); }; 
		INLINE void StepY_Flip_Y_SC_SA_TC	(DefArgs) { StepFunc<Y_Flip_Y_SC, Y_Flip_Y_SA, Y_Flip_Y_TC>	(PassArgs); }; 

		#undef DefArgs
		#undef PassArgs
		
	private:
		INLINE void X_Normal_TC(OpArgs& opArgs, StepArgs& stepArgs) { opArgs.targetColor++; };
		INLINE void Y_Normal_TC(OpArgs& opArgs, StepArgs& stepArgs) { opArgs.targetColor+=stepArgs.targetRowStep; };
		INLINE void X_Normal_TA(OpArgs& opArgs, StepArgs& stepArgs) { opArgs.targetAlpha++; };
		INLINE void Y_Normal_TA(OpArgs& opArgs, StepArgs& stepArgs) { opArgs.targetAlpha+=stepArgs.targetRowStep; };
		INLINE void X_Normal_SC(OpArgs& opArgs, StepArgs& stepArgs) { opArgs.sourceColor++; };
		INLINE void Y_Normal_SC(OpArgs& opArgs, StepArgs& stepArgs) { opArgs.sourceColor+=stepArgs.sourceRowStep; };
		INLINE void X_Normal_SA(OpArgs& opArgs, StepArgs& stepArgs) { opArgs.sourceAlpha++; };
		INLINE void Y_Normal_SA(OpArgs& opArgs, StepArgs& stepArgs) { opArgs.sourceAlpha+=stepArgs.sourceRowStep; };

		INLINE void X_Rot090_TC(OpArgs& opArgs, StepArgs& stepArgs) { opArgs.targetColor++; };
		INLINE void Y_Rot090_TC(OpArgs& opArgs, StepArgs& stepArgs) { opArgs.targetColor+=stepArgs.targetRowStep; };
		INLINE void X_Rot090_TA(OpArgs& opArgs, StepArgs& stepArgs) { opArgs.targetAlpha++; };
		INLINE void Y_Rot090_TA(OpArgs& opArgs, StepArgs& stepArgs) { opArgs.targetAlpha+=stepArgs.targetRowStep; };
		INLINE void X_Rot090_SC(OpArgs& opArgs, StepArgs& stepArgs) { opArgs.sourceColor-=stepArgs.sourceHPitch; };
		INLINE void Y_Rot090_SC(OpArgs& opArgs, StepArgs& stepArgs) { opArgs.sourceColor+=stepArgs.sourceRowStep; };
		INLINE void X_Rot090_SA(OpArgs& opArgs, StepArgs& stepArgs) { opArgs.sourceAlpha-=stepArgs.sourceHPitch; };
		INLINE void Y_Rot090_SA(OpArgs& opArgs, StepArgs& stepArgs) { opArgs.sourceAlpha+=stepArgs.sourceRowStep;};
		
		INLINE void X_Rot180_TC(OpArgs& opArgs, StepArgs& stepArgs) { opArgs.targetColor++; };
		INLINE void Y_Rot180_TC(OpArgs& opArgs, StepArgs& stepArgs) { opArgs.targetColor+=stepArgs.targetRowStep; };
		INLINE void X_Rot180_TA(OpArgs& opArgs, StepArgs& stepArgs) { opArgs.targetAlpha++; };
		INLINE void Y_Rot180_TA(OpArgs& opArgs, StepArgs& stepArgs) { opArgs.targetAlpha+=stepArgs.targetRowStep; };
		INLINE void X_Rot180_SC(OpArgs& opArgs, StepArgs& stepArgs) { opArgs.sourceColor--; };
		INLINE void Y_Rot180_SC(OpArgs& opArgs, StepArgs& stepArgs) { opArgs.sourceColor+=stepArgs.sourceRowStep; };
		INLINE void X_Rot180_SA(OpArgs& opArgs, StepArgs& stepArgs) { opArgs.sourceAlpha--; };
		INLINE void Y_Rot180_SA(OpArgs& opArgs, StepArgs& stepArgs) { opArgs.sourceAlpha+=stepArgs.sourceRowStep; };

		INLINE void X_Rot270_TC(OpArgs& opArgs, StepArgs& stepArgs) { opArgs.targetColor++; };
		INLINE void Y_Rot270_TC(OpArgs& opArgs, StepArgs& stepArgs) { opArgs.targetColor+=stepArgs.targetRowStep; };
		INLINE void X_Rot270_TA(OpArgs& opArgs, StepArgs& stepArgs) { opArgs.targetAlpha++; };
		INLINE void Y_Rot270_TA(OpArgs& opArgs, StepArgs& stepArgs) { opArgs.targetAlpha+=stepArgs.targetRowStep; };
		INLINE void X_Rot270_SC(OpArgs& opArgs, StepArgs& stepArgs) { opArgs.sourceColor+=stepArgs.sourceHPitch; };
		INLINE void Y_Rot270_SC(OpArgs& opArgs, StepArgs& stepArgs) { opArgs.sourceColor+=stepArgs.sourceRowStep; };
		INLINE void X_Rot270_SA(OpArgs& opArgs, StepArgs& stepArgs) { opArgs.sourceAlpha+=stepArgs.sourceHPitch; };
		INLINE void Y_Rot270_SA(OpArgs& opArgs, StepArgs& stepArgs) { opArgs.sourceAlpha+=stepArgs.sourceRowStep; };

		INLINE void X_Flip_X_TC(OpArgs& opArgs, StepArgs& stepArgs) { opArgs.targetColor++; };
		INLINE void Y_Flip_X_TC(OpArgs& opArgs, StepArgs& stepArgs) { opArgs.targetColor+=stepArgs.targetRowStep; };
		INLINE void X_Flip_X_TA(OpArgs& opArgs, StepArgs& stepArgs) { opArgs.targetAlpha++; };
		INLINE void Y_Flip_X_TA(OpArgs& opArgs, StepArgs& stepArgs) { opArgs.targetAlpha+=stepArgs.targetRowStep; };
		INLINE void X_Flip_X_SC(OpArgs& opArgs, StepArgs& stepArgs) { opArgs.sourceColor--; };
		INLINE void Y_Flip_X_SC(OpArgs& opArgs, StepArgs& stepArgs) { opArgs.sourceColor+=stepArgs.sourceRowStep; };
		INLINE void X_Flip_X_SA(OpArgs& opArgs, StepArgs& stepArgs) { opArgs.sourceAlpha--; };
		INLINE void Y_Flip_X_SA(OpArgs& opArgs, StepArgs& stepArgs) { opArgs.sourceAlpha+=stepArgs.sourceRowStep; };

		INLINE void X_Flip_Y_TC(OpArgs& opArgs, StepArgs& stepArgs) { opArgs.targetColor++; };
		INLINE void Y_Flip_Y_TC(OpArgs& opArgs, StepArgs& stepArgs) { opArgs.targetColor+=stepArgs.targetRowStep; };
		INLINE void X_Flip_Y_TA(OpArgs& opArgs, StepArgs& stepArgs) { opArgs.targetAlpha++; };
		INLINE void Y_Flip_Y_TA(OpArgs& opArgs, StepArgs& stepArgs) { opArgs.targetAlpha+=stepArgs.targetRowStep; };
		INLINE void X_Flip_Y_SC(OpArgs& opArgs, StepArgs& stepArgs) { opArgs.sourceColor++; };
		INLINE void Y_Flip_Y_SC(OpArgs& opArgs, StepArgs& stepArgs) { opArgs.sourceColor+=stepArgs.sourceRowStep; };
		INLINE void X_Flip_Y_SA(OpArgs& opArgs, StepArgs& stepArgs) { opArgs.sourceAlpha++; };
		INLINE void Y_Flip_Y_SA(OpArgs& opArgs, StepArgs& stepArgs) { opArgs.sourceAlpha+=stepArgs.sourceRowStep; };

		template<Step opA> INLINE void StepFunc(OpArgs& opArgs, StepArgs& stepArgs) { opA(opArgs,stepArgs); };
		template<Step opA, Step opB> INLINE void StepFunc(OpArgs& opArgs, StepArgs& stepArgs) { opA(opArgs,stepArgs); opB(opArgs,stepArgs); };
		template<Step opA, Step opB, Step opC> INLINE void StepFunc(OpArgs& opArgs, StepArgs& stepArgs) { opA(opArgs,stepArgs); opB(opArgs,stepArgs); opC(opArgs,stepArgs); };
		template<Step opA, Step opB, Step opC, Step opD> INLINE void StepFunc(OpArgs& opArgs, StepArgs& stepArgs) { opA(opArgs,stepArgs); opB(opArgs,stepArgs); opC(opArgs,stepArgs); opD(opArgs,stepArgs); };

	};

#include "Blitter.inl"

#endif /* __Blitter_H__ */
	