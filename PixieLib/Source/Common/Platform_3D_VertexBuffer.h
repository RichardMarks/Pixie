/**
 * \class	Platform_3D_VertexBuffer
 *
 * \ingroup	platform
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 * \todo	Documentation
 */

#ifndef __Platform_3D_VertexBuffer_H__
#define __Platform_3D_VertexBuffer_H__

// Includes

// Forward declares


// Platform_3D_VertexBuffer
class Platform_3D_VertexBuffer
	{
	public:

		enum VertexFormat
			{
			VertexFormat_Position		=	0x0001,
			VertexFormat_ScreenPosition	=	0x0002,
			VertexFormat_Normal			=	0x0004,
			VertexFormat_Diffuse		=	0x0008,
			VertexFormat_Specular		=	0x0010,
			VertexFormat_1TextureSets	=	0x0020,
			VertexFormat_2TextureSets	=	0x0040,
			VertexFormat_3TextureSets	=	0x0080,
			VertexFormat_4TextureSets	=	0x0100,
			VertexFormat_5TextureSets	=	0x0200,
			VertexFormat_6TextureSets	=	0x0400,
			VertexFormat_7TextureSets	=	0x0800,
			VertexFormat_8TextureSets	=	0x1000,
			};

		virtual ~Platform_3D_VertexBuffer() { };

		virtual void Bind() = 0;

		virtual void Lock(
			int startVertex = 0, 
			int vertexCount = 0
			) = 0;

		virtual void Unlock() = 0;

		virtual float* GetPositionData() = 0;

		virtual int GetPositionStride() = 0;

		virtual float* GetScreenPositionData() = 0;

		virtual int GetScreenPositionStride() = 0;

		virtual float* GetNormalData() = 0;

		virtual int GetNormalStride() = 0;

		virtual unsigned int* GetDiffuseData() = 0;

		virtual int GetDiffuseStride() = 0;

		virtual unsigned int* GetSpecularData() = 0;

		virtual int  GetSpecularStride() = 0;

		virtual float* GetUVData(
			int textureSet
			) = 0;

		virtual int GetUVStride(
			int textureSet
			) = 0;



	};

#endif /* __Platform_3D_VertexBuffer_H__ */
