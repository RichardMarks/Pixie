/**
 * \class	Platform_Win32_3D_IndexBuffer
 * 
 * \ingroup	win32
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 *
 */

#ifndef __Platform_Win32_3D_IndexBuffer_H__
#define __Platform_Win32_3D_IndexBuffer_H__

// Includes
#include "Platform_3D_IndexBuffer.h"

// Forward declares
class Platform_Win32_3D;
class Platform_Win32_3D_Technology;

// Platform_Win32_3D_IndexBuffer
class Platform_Win32_3D_IndexBuffer: public Platform_3D_IndexBuffer
	{
	public:
		Platform_Win32_3D_IndexBuffer(
			Platform_Win32_3D* win32_3d,
			int indexCount,
			bool dynamic
			);

		void Reset(Platform_Win32_3D_Technology* technology);

		virtual ~Platform_Win32_3D_IndexBuffer();

		virtual void Lock(int startIndex = 0, int indexCount = 0);

		virtual void Unlock();

		virtual unsigned short* GetIndexData();

		virtual int GetIndexStride();

		virtual void Bind();

	private:
		Platform_Win32_3D* win32_3d_;
		int indexCount_;
		bool dynamic_;
		unsigned short* indexData_;
		bool locked_;
		Platform_3D_IndexBuffer* technologyIndexBuffer_;
	};

#endif /* __Platform_Win32_3D_IndexBuffer_H__ */
