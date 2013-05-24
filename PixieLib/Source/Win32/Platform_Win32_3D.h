/**
 * \class	Platform_Win32_3D
 * 
 * \ingroup	win32
 * \brief	
 * \author	Mattias Gustavsson	
 *
 */

#ifndef __Platform_Win32_3D_H__
#define __Platform_Win32_3D_H__

// Includes
#include "Platform_3D.h"
#include "Array.h"

// Forward declares
class Platform_Win32_OS;
class Platform_Win32_3D_Technology;
class Platform_3D_VertexBuffer;
class Platform_3D_IndexBuffer;
class Platform_3D_Texture;
class Image;
class Platform_Win32_3D_IndexBuffer;
class Platform_Win32_3D_VertexBuffer;
class Platform_Win32_3D_Texture;

// Platform_Win32_3D
class Platform_Win32_3D:public Platform_3D
	{

	public:
		Platform_Win32_3D(Platform_Win32_OS* os);

		~Platform_Win32_3D();

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
			const Asset& asset
			);

		virtual Platform_3D_Texture* CreateTexture(
			const Image& image
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

		virtual void SetFullscreen(bool fullscreen);

		virtual bool GetFullscreen();

		virtual void SetSize(int width, int height);
		
		virtual int GetWidth();

		virtual int GetHeight();

		void OnWmSize(int width, int height);
		
		void OnMinimize();
		
		void OnLoseFocus();

		void OnGainFocus();

		void OnRestore();

		void VertexBufferBound(Platform_Win32_3D_VertexBuffer* vertexBuffer);
		void IndexBufferBound(Platform_Win32_3D_IndexBuffer* indexBuffer);
		void TextureBound(int stage, Platform_Win32_3D_Texture* texture);

		void VertexBufferDeleted(Platform_Win32_3D_VertexBuffer* vertexBuffer);
		void IndexBufferDeleted(Platform_Win32_3D_IndexBuffer* indexBuffer);
		void TextureDeleted(Platform_Win32_3D_Texture* texture);
	private:
		void SetWindowSize();

		enum Technology
			{
			Technology_D3D9,
			Technology_Undefined,
			};

		void SetTechnology(
			Technology technology
			);

		void DowngradeTechnology();

		bool IsRunningOnWine();

	private:
		struct HWND__* windowHandle_;
		Technology technology_;
		Platform_Win32_3D_Technology* technologyInstance_;
		bool fullscreen_;
		int windowedX_;
		int windowedY_;
		int windowedWidth_;
		int windowedHeight_;
		int fullscreenWidth_;
		int fullscreenHeight_;
		int disableOnWmSize_;
		bool showfps_;
		bool ignoreNextOnWmSize_;

		Platform_Win32_3D_VertexBuffer* lastBoundVertexBuffer_;
		Platform_Win32_3D_IndexBuffer* lastBoundIndexBuffer_;
		Platform_Win32_3D_Texture* lastBoundTextures_[8];
		Array<Platform_Win32_3D_VertexBuffer*> vertexBuffers_;
		Array<Platform_Win32_3D_IndexBuffer*> indexBuffers_;
		Array<Platform_Win32_3D_Texture*> textures_;
	};


#endif /* __Platform_Win32_3D_H__ */
