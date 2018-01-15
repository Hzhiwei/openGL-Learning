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
	Ĭ�ϲ�����glGenTextures,��ֹ�ڸ�ֵʱƵ������ɾ�����������ܽ��ͣ�ֻ��
	������Ҫʱ�Ż���������Ԫ
	*/
	class HGLTexture2D
	{
	public:
		HGLTexture2D();
		HGLTexture2D(GLuint InternalFormate, GLuint W, GLuint H, GLuint Border, GLenum Type, const void * const Ptr, HTextureMode M = HTextureMode::ambient);
		HGLTexture2D(string Path, HTextureMode M = HTextureMode::ambient);
		~HGLTexture2D();

	private:
		//���ÿ�������ֵ���캯��
		HGLTexture2D(const HGLTexture2D & Param) {};
		HGLTexture2D & operator=(const HGLTexture2D & Param) {};

	public:
		void LoadFromData(GLuint InternalFormate, GLuint W, GLuint H, GLuint Border, GLenum Type, const void * const Ptr);
		bool LoadFromFile(string Path);
		void Bind();
		void ActiveAndBind(GLenum Unit) const;
		void UnBind();
		void Parameteri(GLenum Pname, GLint Param);
		GLuint GetID();

	private:

	public:
		HTextureMode Mode = HTextureMode::ambient;

	private:
		GLuint ID = 0;
	};
}
