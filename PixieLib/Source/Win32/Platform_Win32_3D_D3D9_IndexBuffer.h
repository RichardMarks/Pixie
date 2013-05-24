/**
 * \class	Platform_Win32_3D_D3D9_IndexBuffer
 * 
 * \ingroup	win32
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 *
 */

#ifndef __Platform_Win32_3D_D3D9_IndexBuffer_H__
#define __Platform_Win32_3D_D3D9_IndexBuffer_H__

// Includes
#include "Platform_3D_IndexBuffer.h"

// Forward declares
struct IDirect3DIndexBuffer9;
struct IDirect3DDevice9;

// Platform_Win32_3D_D3D9_IndexBuffer
class Platform_Win32_3D_D3D9_IndexBuffer: public Platform_3D_IndexBuffer
	{
	public:
		Platform_Win32_3D_D3D9_IndexBuffer(
			IDirect3DDevice9* device, 
			int indexCount,
			bool dynamic
			);

		virtual ~Platform_Win32_3D_D3D9_IndexBuffer();

		virtual void Lock(int startIndex = 0, int indexCount = 0);

		virtual void Unlock();

		virtual unsigned short* GetIndexData();

		virtual int GetIndexStride();

		virtual void Bind();

	private:
		int indexCount_;
		bool dynamic_;
		IDirect3DDevice9* device_;
		IDirect3DIndexBuffer9* d3dIndexBuffer_;
		unsigned char* lockedData_;
	};

#endif /* __Platform_Win32_3D_D3D9_IndexBuffer_H__ */
