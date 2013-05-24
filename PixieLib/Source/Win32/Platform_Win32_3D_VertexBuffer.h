/**
 * \class	Platform_Win32_3D_VertexBuffer
 * 
 * \ingroup	win32
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 *
 */

#ifndef __Platform_Win32_3D_VertexBuffer_H__
#define __Platform_Win32_3D_VertexBuffer_H__

// Includes
#include "Platform_3D_VertexBuffer.h"

// Forward declares
class Platform_Win32_3D;
class Platform_Win32_3D_Technology;

// Platform_Win32_3D_VertexBuffer
class Platform_Win32_3D_VertexBuffer: public Platform_3D_VertexBuffer
	{
	public:
		Platform_Win32_3D_VertexBuffer(
			Platform_Win32_3D* win32_3d,
			int vertexFormat, 
			int vertexCount,
			bool dynamic
			);

		virtual ~Platform_Win32_3D_VertexBuffer();

		void Reset(Platform_Win32_3D_Technology* technology);

		virtual void Lock(int startVertex = 0, int vertexCount = 0);

		virtual void Unlock();

		virtual float* GetPositionData();

		virtual int GetPositionStride();

		virtual float* GetScreenPositionData();

		virtual int GetScreenPositionStride();

		virtual float* GetNormalData();

		virtual int GetNormalStride();

		virtual unsigned int* GetDiffuseData();

		virtual int GetDiffuseStride();

		virtual unsigned int* GetSpecularData();

		virtual int GetSpecularStride();

		virtual float* GetUVData(int textureSet);

		virtual int GetUVStride(int textureSet);

		virtual void Bind();

	private:
		bool ValidateVertexFormat(
			int vertexFormat
			);
		
		unsigned long CalculateFVF(
			int vertexFormat
			);

		int CalculateSize(
			int vertexFormat
			);

		int GetNumberOfTextureSets(
			int vertexFormat
			);

	private:
		Platform_Win32_3D* win32_3d_;
		int vertexFormat_;
		int vertexCount_;
		bool dynamic_;
		int vertexSize_;
		unsigned char* vertexData_;
		bool locked_;
		Platform_3D_VertexBuffer* technologyVertexBuffer_;
	};

#endif /* __Platform_Win32_3D_VertexBuffer_H__ */
