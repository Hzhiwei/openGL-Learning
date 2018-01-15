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
		//Ĭ�Ϲ��캯��
		HGLShaderProgram();
		//���붥����ɫ��ID��Ƭ����ɫ��ID�Ĺ��캯�����ڲ�ֱ������
		HGLShaderProgram(GLuint Vertex, GLuint Fragment);
		//����
		~HGLShaderProgram();

	private:
		HGLShaderProgram(const HGLShaderProgram & Param) {}
		HGLShaderProgram & operator=(const HGLShaderProgram & Param) {}

	public:
		//���ö�����ɫ����Ƭ����ɫ�����ڲ�ֱ������
		bool SetShader(GLuint Vertex, GLuint Fragment);
		//���õ�����ɫ��
		void SetShader(GLuint ShaderID);
		//��ȡ������Ϣ
		std::string GetLinkInfo();
		//���ӳ���
		bool Link();
		//ʹ�ô���ɫ��
		void Use() const;
		//��ȡȫ�ֱ�����ַ
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




