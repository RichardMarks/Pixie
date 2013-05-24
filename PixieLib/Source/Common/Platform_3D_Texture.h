/**
 * \class	Platform_3D_Texture
 *
 * \ingroup	platform
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 * \todo	Documentation
 */

#ifndef __Platform_3D_Texture_H__
#define __Platform_3D_Texture_H__

// Includes

// Forward declares


// Platform_3D_Texture
class Platform_3D_Texture
	{
	public:

		virtual ~Platform_3D_Texture() { };

		virtual void Bind(
			int stage
			) = 0;

		virtual int GetWidth() = 0;
		virtual int GetHeight() = 0;
		virtual bool HasAlphaChannel() = 0;

		virtual void Lock() = 0;
		virtual void Unlock() = 0;
		virtual unsigned char* GetTexelData() = 0;
		virtual int GetPitch() = 0;
	};

#endif /* __Platform_3D_Texture_H__ */
