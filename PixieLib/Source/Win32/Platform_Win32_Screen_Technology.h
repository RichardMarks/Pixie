/**
 * \class	Platform_Win32_Screen_Technology
 * 
 * \ingroup	win32
 * \brief	
 * \author	Mattias Gustavsson	
 *
 * \todo	Make RGBModulate16 and RGB16TO32 non-inline
 */

#ifndef __Platform_Win32_Screen_Technology_H__
#define __Platform_Win32_Screen_Technology_H__

// Includes

// Forward declares

// Platform_Win32_Screen_Technology
class Platform_Win32_Screen_Technology
	{

	public:
		virtual ~Platform_Win32_Screen_Technology() { };

		virtual bool Setup() = 0;

		virtual bool Present(
			unsigned short* bitmapData,
			int bitmapWidth,
			int bitmapHeight,
			unsigned short modulate,
			unsigned short backgroundColor
			) = 0;

		virtual bool Present(
			unsigned int* bitmapData,
			int bitmapWidth,
			int bitmapHeight,
			unsigned int modulate,
			unsigned int backgroundColor
			) = 0;

		virtual void SetInterpolationMode(bool enabled) = 0;

	
		virtual void TransformCursorCoordinates(float& x, float& y) = 0;


		inline unsigned short RGBModulate16(
			unsigned short color1, 
			unsigned short color2
			)
			{
			return
				((unsigned short)(
					((((((unsigned int)color1 & 0xf800)>>11)*(((unsigned int)color2 & 0xf800)>>11)))>>5) <<11 |
					((((((unsigned int)color1 & 0x07e0)>>5 )*(((unsigned int)color2 & 0x07e0)>>5 )))>>6) <<5  |
					((((((unsigned int)color1 & 0x001f)    )*(((unsigned int)color2 & 0x001f)    )))>>5)
				));
			}


		inline unsigned int RGB16TO32(
			unsigned short color
			)
			{
			return 
				(((unsigned int)color & 0xf800)<<8) |
				(((unsigned int)color & 0x07e0)<<5) |
				(((unsigned int)color & 0x001f)<<3) |
				(0xff000000);
			}
	private:
	};


#endif /* __Platform_Win32_Screen_Technology_H__ */
