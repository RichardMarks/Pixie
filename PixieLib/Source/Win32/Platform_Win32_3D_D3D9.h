/**
 * \class	Platform_Win32_3D_D3D9
 * 
 * \ingroup	win32
 * \brief	
 * \author	Mattias Gustavsson	
 *
 */

#ifndef __Platform_Win32_3D_D3D9_H__
#define __Platform_Win32_3D_D3D9_H__

// Includes
#include "Platform_Win32_3D_Technology.h"

// Forward declares
struct IDirect3D9;
struct IDirect3DDevice9;
struct IDirect3DTexture9;
//struct ID3DXEffect;


// Platform_Win32_3D_D3D9
class Platform_Win32_3D_D3D9:public Platform_Win32_3D_Technology
	{

	public:
		Platform_Win32_3D_D3D9(struct HWND__* windowHandle, bool fullscreen, int screenWidth, int screenHeight);
		virtual ~Platform_Win32_3D_D3D9();

		virtual bool Setup();

		virtual void BeginScene(
			unsigned int color, 
			float z, 
			unsigned int stencil
			);

		virtual void EndScene();

		virtual Platform_3D_VertexBuffer* CreateVertexBuffer(
			int vertexFormat, 
			int vertexCount, 
			bool dynamic			
			);

		virtual Platform_3D_IndexBuffer* CreateIndexBuffer(
			int indexCount, 
			bool dynamic			
			);

		virtual Platform_3D_Texture* CreateTexture(
			void* data,
			int size
			);

		virtual Platform_3D_Texture* CreateTexture(
			int width,
			int height,
			void* data
			);

		virtual void Render(
			int startVertex,
			int vertexCount,
			int startIndex,
			int indexCount
			);

		virtual void Render(
			int startVertex,
			int vertexCount
			);

		virtual void RenderStrip(
			int startVertex,
			int vertexCount,
			int startIndex,
			int indexCount
			);

		virtual void RenderStrip(
			int startVertex,
			int vertexCount
			);

		virtual void RenderFan(
			int startVertex,
			int vertexCount,
			int startIndex,
			int indexCount
			);

		virtual void RenderFan(
			int startVertex,
			int vertexCount
			);

		virtual void SetWorldMatrix(
			const float worldMatrix[16]
			);

		virtual void SetViewMatrix(
			const float viewMatrix[16]
			);

		virtual void SetProjectionMatrix(
			const float projectionMatrix[16]
			);

		virtual int GetLightCount();

		virtual void EnableDirectionalLight(
			int lightIndex,
			float colorR,
			float colorG,
			float colorB,
			float directionX,
			float directionY,
			float directionZ
			);

		virtual void DisableLight(
			int lightIndex
			);

		virtual void SetAmbientLight(
			unsigned int color
			);

		virtual void EnableLighting(
			bool enabled
			);

		virtual void EnableZRead(
			bool enabled
			);

		virtual void EnableZWrite(
			bool enabled
			);

	private: 
//		bool IsCompressedTextureFormatOk( D3DFORMAT TextureFormat,  D3DFORMAT AdapterFormat ) ;

	private:
		struct HWND__* windowHandle_;
		bool fullscreen_;
		int screenWidth_;
		int screenHeight_;
		void* d3dDLL_;
		IDirect3D9* direct3D_;
		IDirect3DDevice9* device_;
		int lightCount_;
	};
		

#endif /* __Platform_Win32_3D_D3D9_H__ */
