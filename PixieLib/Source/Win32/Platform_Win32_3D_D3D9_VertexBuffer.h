/**
 * \class	Platform_Win32_3D_D3D9_VertexBuffer
 * 
 * \ingroup	win32
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 *
 */

#ifndef __Platform_Win32_3D_D3D9_VertexBuffer_H__
#define __Platform_Win32_3D_D3D9_VertexBuffer_H__

// Includes
#include "Platform_3D_VertexBuffer.h"

// Forward declares
struct IDirect3DVertexBuffer9;
struct IDirect3DDevice9;

// Platform_Win32_3D_D3D9_VertexBuffer
class Platform_Win32_3D_D3D9_VertexBuffer: public Platform_3D_VertexBuffer
	{
	public:
		Platform_Win32_3D_D3D9_VertexBuffer(
			IDirect3DDevice9* device, 
			int vertexFormat, 
			int vertexCount,
			bool dynamic
			);

		virtual ~Platform_Win32_3D_D3D9_VertexBuffer();

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
		int vertexFormat_;
		int  vertexSize_;
		int vertexCount_;
		bool dynamic_;
		IDirect3DDevice9* device_;
		unsigned long d3dFVF_; 
		IDirect3DVertexBuffer9* d3dVertexBuffer_;
		unsigned char* lockedData_;
	};

#endif /* __Platform_Win32_3D_D3D9_VertexBuffer_H__ */
