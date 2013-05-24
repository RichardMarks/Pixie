/**
 * \class	Platform_Win32_3D_Technology
 * 
 * \ingroup	win32
 * \brief	
 * \author	Mattias Gustavsson	
 *
 */

#ifndef __Platform_Win32_3D_Technology_H__
#define __Platform_Win32_3D_Technology_H__

// Includes

// Forward declares
class Platform_3D_VertexBuffer;
class Platform_3D_IndexBuffer;
class Platform_3D_Texture;
class Image;
class Asset;


// Platform_Win32_3D_Technology
class Platform_Win32_3D_Technology
	{

	public:
		virtual ~Platform_Win32_3D_Technology() { };

		virtual bool Setup() = 0;

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
			void* data,
			int size
			) = 0;

		virtual Platform_3D_Texture* CreateTexture(
			int width,
			int height,
			void* data
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

		virtual void RenderStrip(
			int startVertex,
			int vertexCount,
			int startIndex,
			int indexCount
			) = 0;

		virtual void RenderStrip(
			int startVertex,
			int vertexCount
			) = 0;

		virtual void RenderFan(
			int startVertex,
			int vertexCount,
			int startIndex,
			int indexCount
			) = 0;

		virtual void RenderFan(
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


#endif /* __Platform_Win32_3D_Technology_H__ */
