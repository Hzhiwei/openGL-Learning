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
	template<int ShaderType>
	class HGLShader
	{
	public:
		HGLShader();
		HGLShader(const GLchar * const Code);
		HGLShader(const std::string &Code);
		~HGLShader();

	private:
		HGLShader(const HGLShader &) {}
		HGLShader & operator=(const HGLShader &) {}

	public:
		bool Source(const GLchar * const Code);
		bool Source(const std::string & Code);
		std::string GetCompileInfo();
		GLuint GetID();

	protected:
		GLuint ID = 0;
		char * Info = NULL;
	};


	class ShaderTypeErrorException : public std::invalid_argument
	{
	public:
		explicit ShaderTypeErrorException(const std::string& what_arg) : invalid_argument(what_arg) {}
		explicit ShaderTypeErrorException(const char* what_arg) : invalid_argument(what_arg) {}
	};


	template<int ShaderType>
	HGLShader<ShaderType>::HGLShader()
	{
		ID = glCreateShader(ShaderType);
		Info = new char[INFOLENGTH];
	}

	template<int ShaderType>
	HGLShader<ShaderType>::HGLShader(const GLchar * const Code)
	{
		ID = glCreateShader(ShaderType);
		Info = new char[INFOLENGTH];
		Source(Code);
	}

	template<int ShaderType>
	HGLShader<ShaderType>::HGLShader(const std::string & Code)
	{
		ID = glCreateShader(ShaderType);
		Info = new char[INFOLENGTH];
		Source(Code);
	}

	template<int ShaderType>
	HGLShader<ShaderType>::~HGLShader()
	{
		glDeleteShader(ID);
		delete Info;
	}

	template<int ShaderType>
	bool HGLShader<ShaderType>::Source(GLchar const * const Code)
	{
		int success;
		glShaderSource(ID, 1, &Code, NULL);
		glCompileShader(ID);
		glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	template<int ShaderType>
	bool HGLShader<ShaderType>::Source(const std::string & Code)
	{
		return Source(Code.c_str());
	}

	template<int ShaderType>
	std::string HGLShader<ShaderType>::GetCompileInfo()
	{
		glGetShaderInfoLog(ID, INFOLENGTH, NULL, Info);
		return std::string(Info);
	}

	template<int ShaderType>
	GLuint HGLShader<ShaderType>::GetID()
	{
		return ID;
	}
}




