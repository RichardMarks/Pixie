
/**
 * \class	QuadBlitter
 * 
 * \ingroup	graphics
 * \brief	Helper class for bitmap blitting
 * \author	Mattias Gustavsson	
 * 
 * \todo	Add support for clipping
 * \todo	Add support for modulate blitting
 * \todo	Add support for additive blitting
 * \todo	Add copper functionality (for copying alpha channels and filling alpha) 
 */

#ifndef __QuadBlitter_H__
#define __QuadBlitter_H__

// Includes
#include "StandardLibrary.h"

// Forward declares


// QuadBlitter
class QuadBlitter
	{
	public:
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
			int targetVPitch;
			int sourceX1;
			int sourceY1;
			int sourceX2;
			int sourceY2;
			int sourceX3;
			int sourceY3;
			int sourceX4;
			int sourceY4;
			int targetX1;
			int targetY1;
			int targetX2;
			int targetY2;
			int targetX3;
			int targetY3;
			int targetX4;
			int targetY4;
			};

        template<typename Edge> struct RowArgsUV
			{
            RowArgsUV(unsigned short* data, Edge** edgePtr)
                {
		        int xstart=(*edgePtr)->x;
		        u=(*edgePtr)->u;
		        v=(*edgePtr)->v;
		        (*edgePtr)++;

                int xend=(*edgePtr)->x;
		        int u1=(*edgePtr)->u;
		        int v1=(*edgePtr)->v;
		        (*edgePtr)++;

                pdata=data+xstart;

                width=xend-xstart+1;

                stepU=(u1-u)/width;
	            stepV=(v1-v)/width;
                }

            void Step()
                {   
			    pdata++;
			    u+=stepU;
			    v+=stepV;
                }

            unsigned short* pdata;
            int width;
            int u;
		    int v;
            int stepU;
            int stepV;
			};


//        template<Blitter::Operation operation, typename Edge, typename RowArgs> static void Blit(BlitArgs& blitArgs);



		// DstColor fylls med fillColor
		static void Blit(unsigned short fillColor, unsigned short* targetColor, int targetHPitch, int targetVPitch, int targetX1, int targetY1, int targetX2, int targetY2, int targetX3, int targetY3, int targetX4, int targetY4);

		static void Blit(unsigned short fillColor, unsigned short* targetColor, int targetHPitch, int targetVPitch, int targetX1, int targetY1, int targetX2, int targetY2, int targetX3, int targetY3, int targetX4, int targetY4, int clipX1, int clipY1, int clipX2, int clipY2);

		// SrcColor kopieras till DstColor
		static void Blit(unsigned short* sourceColor, int sourceHPitch, int sourceVPitch, unsigned short* targetColor, int targetHPitch, int targetVPitch, int sourceX1, int sourceY1, int sourceX2, int sourceY2, int sourceX3, int sourceY3, int sourceX4, int sourceY4, int targetX1, int targetY1, int targetX2, int targetY2, int targetX3, int targetY3, int targetX4, int targetY4);
		
		// SrcColor blendas (SrcAlpha) med DstColor
		static void Blit(unsigned short* sourceColor, unsigned char* sourceAlpha, int sourceHPitch, int sourceVPitch, unsigned short* targetColor, int targetHPitch, int targetVPitch, int sourceX1, int sourceY1, int sourceX2, int sourceY2, int sourceX3, int sourceY3, int sourceX4, int sourceY4, int targetX1, int targetY1, int targetX2, int targetY2, int targetX3, int targetY3, int targetX4, int targetY4);

		static void Blit(unsigned short* sourceColor, unsigned char* sourceAlpha, int sourceHPitch, int sourceVPitch, unsigned short* targetColor, int targetHPitch, int targetVPitch, int sourceX1, int sourceY1, int sourceX2, int sourceY2, int sourceX3, int sourceY3, int sourceX4, int sourceY4, int targetX1, int targetY1, int targetX2, int targetY2, int targetX3, int targetY3, int targetX4, int targetY4, int clipX1, int clipY1, int clipX2, int clipY2);


		// SrcColor blendas (SrcAlpha*BlitAlpha) med DstColor
		static void Blit(unsigned char alpha, unsigned short* sourceColor, unsigned char* sourceAlpha, int sourceHPitch, int sourceVPitch, unsigned short* targetColor, int targetHPitch, int targetVPitch, int sourceX1, int sourceY1, int sourceX2, int sourceY2, int sourceX3, int sourceY3, int sourceX4, int sourceY4, int targetX1, int targetY1, int targetX2, int targetY2, int targetX3, int targetY3, int targetX4, int targetY4);

	private:
		struct EdgeX
			{
			int x;
			};

		struct EdgeXUV
			{
			int u;
			int v;
			int x;
			};



		class EdgeBuffer
			{
			public:
				EdgeBuffer():
					buffer_(0),
					size_(512)
					{

					}

				~EdgeBuffer() 
					{ 
					if (buffer_) 
						{
						Free(buffer_); 
						buffer_=0;
						size_=512;
						}
					}

				void SetMinSize(int size) 
					{
					if (buffer_ && size_>size)
						{
						return;
						}

					while (size_<size)
						{
						size_*=2;
						}

					if (!buffer_) 
						{ 
						buffer_=Malloc(size_);  
						}
					else
						{ 
						Free(buffer_);
						buffer_=Malloc(size_);  
						} 
					}
				
				void* buffer_;
				int size_;
			};

		static EdgeBuffer edgeBuffer_;
		
	};
/*
template<Blitter::Operation operation, typename Edge, typename RowArgs> 
static void QuadBlitter::Blit(BlitArgs& blitArgs)
	{
    blitArgs.sourceX2++;
	blitArgs.sourceX3++;
	blitArgs.sourceY3++;
	blitArgs.sourceY4++;
    int sourceX1=blitArgs.sourceX1;
    int sourceY1=blitArgs.sourceY1;
    int sourceX2=blitArgs.sourceX2;
    int sourceY2=blitArgs.sourceY2;
    int sourceX3=blitArgs.sourceX3;
    int sourceY3=blitArgs.sourceY3;
    int sourceX4=blitArgs.sourceX4;
    int sourceY4=blitArgs.sourceY4;
    int targetX1=blitArgs.targetX1;
    int targetY1=blitArgs.targetY1;
    int targetX2=blitArgs.targetX2;
    int targetY2=blitArgs.targetY2;
    int targetX3=blitArgs.targetX3;
    int targetY3=blitArgs.targetY3;
    int targetX4=blitArgs.targetX4;
    int targetY4=blitArgs.targetY4;


	// *** START EDGE SCANNING ***

	// To avoid massive amounts of code duplication, the edge scanning code have been
	// written to cope with multiple scenarios, with features determined by #defines

	#define EDGE_TYPE Edge
	#define USE_UV
	
	#include "QuadBlitter_scanning.inl"
	
	#undef USE_UV
	#undef EDGE_TYPE

	// *** END EDGE SCANNING ***


	// Draw rows
	unsigned short* data=blitArgs.targetColor+p0y*blitArgs.targetHPitch;
	Edge* edgePtr=static_cast<Edge*>(edgeBuffer_.buffer_);
    
    for (int y=p0y; y<p3y; y++)
		{
        RowArgs row(data,&edgePtr);
		
		for (int x=0; x<row.width; x++)
			{
			int offset=(row.u>>16)+blitArgs.sourceHPitch*(row.v>>16);
    		*(row.pdata)=blitArgs.sourceColor[offset];
	        row.Step();
			}

		// Advance to next rows
		data+=blitArgs.targetHPitch;
		}

	}


*/
#endif /* __QuadBlitter_H__ */
		