/**
 * \class	Platform_3D_IndexBuffer
 *
 * \ingroup	platform
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 * \todo	Documentation
 */

#ifndef __Platform_3D_IndexBuffer_H__
#define __Platform_3D_IndexBuffer_H__

// Includes

// Forward declares


// Platform_3D_IndexBuffer
class Platform_3D_IndexBuffer
	{
	public:
		virtual ~Platform_3D_IndexBuffer() { };

		virtual void Bind() = 0;

		virtual void Lock(
			int startIndex = 0, 
			int indexCount = 0
			) = 0;

		virtual void Unlock() = 0;

		virtual unsigned short* GetIndexData() = 0;

		virtual int GetIndexStride() = 0;

	};

#endif /* __Platform_3D_IndexBuffer_H__ */
