#pragma once

#include "../../GLAD/glad.h"
#include <iostream>
#include <string>
#include <map>
#include <fstream> 
#include <stdexcept>
#include <glm.hpp>

#include "../GLReferenceCounter/HGLReferenceCounter.h"


#define INFOLENGTH 1024



namespace HGLTool
{
	class HGLShaderProgram
	{
	public:
		//默认构造函数
		HGLShaderProgram();
		//输入顶点着色器ID和片段着色器ID的构造函数，内部直接链接
		HGLShaderProgram(GLuint Vertex, GLuint Fragment);
		//析构
		~HGLShaderProgram();

	private:
		HGLShaderProgram(const HGLShaderProgram & Param) {}
		HGLShaderProgram & operator=(const HGLShaderProgram & Param) {}

	public:
		//设置顶点着色器，片段着色器，内部直接链接
		bool SetShader(GLuint Vertex, GLuint Fragment);
		//设置单个着色器
		void SetShader(GLuint ShaderID);
		//获取链接信息
		std::string GetLinkInfo();
		//链接程序
		bool Link();
		//使用此着色器
		void Use() const;
		//获取全局变量地址
		GLint GetUniform(const char * const Name) const;

		void SetBool(const char * const Name, bool value) const;
		void SetInt(const char * const Name, int value) const;
		void SetFloat(const char * const Name, float value) const;
		void SetMat4fv(const char * const Name, const GLfloat *value) const;
		void SetMat4fv(const char * const Name, const glm::mat4 &value) const;

	protected:
		GLuint ID = 0;
		char *Info;
	};


	class HGLShaderFileSource
	{
	public:
		HGLShaderFileSource();
		HGLShaderFileSource(std::string Path);
		~HGLShaderFileSource();

		bool LoadFile(std::string Path);
		bool SetHeader(std::string Head);
		bool SetEnder(std::string End);
		std::string HGLShaderFileSource::GetShaderCode(std::string Name);
		void Clear();

	private:
		void FunctionAnalyze(std::ifstream &stream);

	private:
		std::string header = "##start";
		std::string ender = "##end";
		std::map<std::string, std::string> FunctionMap;
	};
}




