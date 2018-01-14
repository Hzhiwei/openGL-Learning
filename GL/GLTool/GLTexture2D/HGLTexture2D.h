#pragma once

#include "../../GLAD/glad.h"
#include <stb_image.h>
#include <iostream>
#include "../GLReferenceCounter/HGLReferenceCounter.h"

using namespace std;

namespace HGLTool
{
	typedef enum HTextureMode
	{
		ambient,
		diffuse,
		specular,
	}HTextureMode;

	/*
	默认不进行glGenTextures,防止在赋值时频繁生成删除，导致性能降低，只有
	真正需要时才会生成纹理单元
	*/
	class HGLTexture2D
	{
	public:
		HGLTexture2D();
		HGLTexture2D(const HGLTexture2D & Param);
		HGLTexture2D(GLuint InternalFormate, GLuint W, GLuint H, GLuint Border, GLenum Type, const void * const Ptr, HTextureMode M = HTextureMode::ambient);
		HGLTexture2D(string Path, HTextureMode M = HTextureMode::ambient);
		~HGLTexture2D();
		HGLTexture2D & operator=(const HGLTexture2D & Param);

		void LoadFromData(GLuint InternalFormate, GLuint W, GLuint H, GLuint Border, GLenum Type, const void * const Ptr);
		bool LoadFromFile(string Path);
		void Bind();
		void ActiveAndBind(GLenum Unit) const;
		void UnBind();
		void Parameteri(GLenum Pname, GLint Param);
		GLuint GetID();

	private:
		//新建Texture时的ID与Counter处理
		void CounterAndTextureManger();
		//从Param拷贝Texture时的ID与Counter处理
		void CounterAndTextureManger(const HGLTexture2D & Param);

	public:
		HTextureMode Mode = HTextureMode::ambient;

	private:
		GLuint ID = 0;
		HGLReferenceCounter *Counter = NULL;
		bool SourceObjects = false;
	};
}
