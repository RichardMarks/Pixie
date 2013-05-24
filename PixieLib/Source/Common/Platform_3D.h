/**
 * \class	Platform_3D
 * 
 * \ingroup	platform
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 * \todo	Documentation
 */

#ifndef __Platform_3D_H__
#define __Platform_3D_H__

// Includes
#include "Platform.h"
#include "Platform_3D_VertexBuffer.h"

// Forward declares
class Platform_3D_IndexBuffer;
class Platform_3D_Texture;
class Platform_3D_VertexBuffer;
class Asset;
class Image;

// Platform_3D
class Platform_3D
	{
	public:
		virtual ~Platform_3D() { };

		virtual void SetFullscreen(bool fullscreen) = 0;

		virtual bool GetFullscreen() = 0;

		virtual void SetSize(int width, int height) = 0;
		
		virtual int GetWidth() = 0;

		virtual int GetHeight() = 0;

		virtual void BeginScene(
			unsigned int color, 
			float z, 
			unsigned int stencil
			) = 0;

		virtual void EndScene() = 0;

		virtual Platform_3D_VertexBuffer* CreateVertexBuffer(
			int vertexFormat, 
			int vertexCount, 
			bool dynamic			
			) = 0;

		virtual Platform_3D_IndexBuffer* CreateIndexBuffer(
			int indexCount, 
			bool dynamic			
			) = 0;

		virtual Platform_3D_Texture* CreateTexture(
			const Asset& asset
			) = 0;

		virtual Platform_3D_Texture* CreateTexture(
			const Image& image
			) = 0;

		virtual void Render(
			int startVertex,
			int vertexCount,
			int startIndex,
			int indexCount
			) = 0;


		virtual void Render(
			int startVertex,
			int vertexCount
			) = 0;


		virtual void SetWorldMatrix(
			const float worldMatrix[16]
			) = 0;

		virtual void SetViewMatrix(
			const float viewMatrix[16]
			) = 0;

		virtual void SetProjectionMatrix(
			const float projectionMatrix[16]
			) = 0;

		virtual int GetLightCount() = 0;

		virtual void EnableDirectionalLight(
			int lightIndex,
			float colorR,
			float colorG,
			float colorB,
			float directionX,
			float directionY,
			float directionZ
			) = 0;

		virtual void DisableLight(
			int lightIndex
			) = 0;

		virtual void SetAmbientLight(
			unsigned int color
			) = 0;

		virtual void EnableLighting(
			bool enabled
			) = 0;

		virtual void EnableZRead(
			bool enabled
			) = 0;

		virtual void EnableZWrite(
			bool enabled
			) = 0;


	private:
	};


#endif /* __Platform_3D_H__ */
