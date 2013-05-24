/**
 * \class	Platform_Win32_3D_Texture
 * 
 * \ingroup	win32
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 *
 */

#ifndef __Platform_Win32_3D_Texture_H__
#define __Platform_Win32_3D_Texture_H__

// Includes
#include "Platform_3D_Texture.h"

// Forward declares
class Platform_Win32_3D;
class Platform_Win32_3D_Technology;
class Image;
class Asset;

// Platform_Win32_3D_Texture
class Platform_Win32_3D_Texture: public Platform_3D_Texture
	{
	public:
		Platform_Win32_3D_Texture(
			Platform_Win32_3D* win32_3d,
			const Asset& asset
			);

		Platform_Win32_3D_Texture(
			Platform_Win32_3D* win32_3d,
			const Image& image
			);

		virtual ~Platform_Win32_3D_Texture();

		void Reset(Platform_Win32_3D_Technology* technology);

		virtual void Bind(int stage);

		virtual int GetWidth();
		virtual int GetHeight();
		virtual bool HasAlphaChannel();

		virtual void Lock();
		virtual void Unlock();
		virtual unsigned char* GetTexelData();
		virtual int GetPitch();

	private:
		Platform_Win32_3D* win32_3d_;
		int width_;
		int height_;
		int size_;
		unsigned char* textureData_;
		Platform_3D_Texture* technologyTexture_;
	};

#endif /* __Platform_Win32_3D_Texture_H__ */
