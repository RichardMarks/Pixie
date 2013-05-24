/**
 * \class	Platform_Win32_3D_D3D9_Texture
 * 
 * \ingroup	win32
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 *
 */

#ifndef __Platform_Win32_3D_D3D9_Texture_H__
#define __Platform_Win32_3D_D3D9_Texture_H__

// Includes
#include "Platform_3D_Texture.h"

// Forward declares
struct IDirect3DTexture9;
struct IDirect3DDevice9;
struct IDirect3DSurface9;
class Asset;

// Platform_Win32_3D_D3D9_Texture
class Platform_Win32_3D_D3D9_Texture: public Platform_3D_Texture
	{
	public:
		Platform_Win32_3D_D3D9_Texture(
			IDirect3DDevice9* device, 
			int width,
			int height,
			void* data
			);

		Platform_Win32_3D_D3D9_Texture(
			IDirect3DDevice9* device, 
			void* data,
			int size
			);

		virtual ~Platform_Win32_3D_D3D9_Texture();

		virtual void Bind(int stage);

		virtual int GetWidth();
		virtual int GetHeight();
		virtual bool HasAlphaChannel();

		virtual void Lock();
		virtual void Unlock();
		virtual unsigned char* GetTexelData();
		virtual int GetPitch();

	private:
		IDirect3DDevice9* device_;
		IDirect3DTexture9* d3dTexture_;
		IDirect3DSurface9* lockedSurface_;
		unsigned char* lockedData_;
		int lockedPitch_;
		int width_;
		int height_;
		int depth_;
		int mipLevels_;
		D3DFORMAT format_;
	};

#endif /* __Platform_Win32_3D_D3D9_Texture_H__ */
