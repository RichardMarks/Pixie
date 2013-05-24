
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
		// DstColor fylls med fillColor
		static void Blit(unsigned short fillColor, unsigned short* targetColor, int targetHPitch, int targetVPitch, int targetX1, int targetY1, int targetX2, int targetY2, int targetX3, int targetY3, int targetX4, int targetY4);

		// SrcColor kopieras till DstColor
		static void Blit(unsigned short* sourceColor, int sourceHPitch, int sourceVPitch, unsigned short* targetColor, int targetHPitch, int targetVPitch, int sourceX1, int sourceY1, int sourceX2, int sourceY2, int sourceX3, int sourceY3, int sourceX4, int sourceY4, int targetX1, int targetY1, int targetX2, int targetY2, int targetX3, int targetY3, int targetX4, int targetY4);
		
		// SrcColor blendas (SrcAlpha) med DstColor
		static void Blit(unsigned short* sourceColor, unsigned char* sourceAlpha, int sourceHPitch, int sourceVPitch, unsigned short* targetColor, int targetHPitch, int targetVPitch, int sourceX1, int sourceY1, int sourceX2, int sourceY2, int sourceX3, int sourceY3, int sourceX4, int sourceY4, int targetX1, int targetY1, int targetX2, int targetY2, int targetX3, int targetY3, int targetX4, int targetY4);

		// SrcColor blendas (SrcAlpha*BlitAlpha) med DstColor
		static void Blit(unsigned char alpha, unsigned short* sourceColor, unsigned char* sourceAlpha, int sourceHPitch, int sourceVPitch, unsigned short* targetColor, int targetHPitch, int targetVPitch, int sourceX1, int sourceY1, int sourceX2, int sourceY2, int sourceX3, int sourceY3, int sourceX4, int sourceY4, int targetX1, int targetY1, int targetX2, int targetY2, int targetX3, int targetY3, int targetX4, int targetY4);
	private:
		struct EdgeX
			{
			unsigned short x;
			};

		struct EdgeXUV
			{
			int u;
			int v;
			unsigned short x;
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

#endif /* __QuadBlitter_H__ */
		