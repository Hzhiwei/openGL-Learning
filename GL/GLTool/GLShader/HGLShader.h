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
		HGLShader(const HGLShader & Param);
		HGLShader(const GLchar * const Code);
		HGLShader(const std::string &Code);
		~HGLShader();
		HGLShader & operator=(const HGLShader & Param);

	public:
		bool Source(const GLchar * const Code);
		bool Source(const std::string &Code);
		std::string GetCompileInfo();
		GLuint GetID();

	private:
		//新建Shader时的ID与Counter处理
		void CounterAndShaderManger();
		//从Param拷贝Shader时的ID与Counter处理
		void CounterAndShaderManger(const HGLShader & Param);


	protected:
		GLuint ID = 0;
		char * Info = NULL;

		HGLReferenceCounter * Counter = NULL;
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
	}

	template<int ShaderType>
	inline HGLShader<ShaderType>::HGLShader(const HGLShader & Param)
	{
		CounterAndShaderManger(Param);
	}

	template<int ShaderType>
	HGLShader<ShaderType>::HGLShader(const GLchar * const Code)
	{
		CounterAndShaderManger();
		Source(Code);
	}

	template<int ShaderType>
	HGLShader<ShaderType>::HGLShader(const std::string & Code)
	{
		CounterAndShaderManger();
		Source(Code);
	}

	template<int ShaderType>
	HGLShader<ShaderType>::~HGLShader()
	{
		if (Counter != NULL)
		{
			if (Counter->Decrease())
			{
				glDeleteShader(ID);
				delete Info;
				delete Counter;
			}
		}
	}

	template<int ShaderType>
	inline HGLShader<ShaderType> & HGLShader<ShaderType>::operator=(const HGLShader<ShaderType> & Param)
	{
		CounterAndShaderManger(Param);
		return *this;
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
		if (SourceObject)
		{
			return Source(Code.c_str());
		}
		else
		{
			return false;
		}
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

	template<int ShaderType>
	inline void HGLShader<ShaderType>::CounterAndShaderManger()
	{
		if (Counter == NULL)
		{
			Counter = new HGLReferenceCounter();
			ID = glCreateShader(ShaderType);
			Info = new char[INFOLENGTH];

			Counter->Increase();
		}
		else
		{
			if (!Counter->IsOnlyOne())
			//如果此对象当前维护的纹理单元在内存中只有一个，则不需要Counter->Decrease()后再新建，
			//可以直接修改当前纹理单元和Counter
			{
				Counter->Decrease();

				Counter = new HGLReferenceCounter();
				ID = glCreateShader(ShaderType);
				Info = new char[INFOLENGTH];

				Counter->Increase();
			}
		}
	}

	template<int ShaderType>
	inline void HGLShader<ShaderType>::CounterAndShaderManger(const HGLShader<ShaderType> & Param)
	{
		if (Counter->IsOnlyOne())
			//如果此对象当前维护的纹理单元在内存中只有一个，则需要删除当前的纹理单元和Counter，并指向指定纹理单元和Counter
		{
			glDeleteShader(ID);
			delete Info;
			delete Counter;
		}
		else
			//如果此对象当前维护的纹理单元在内存中不止一个，则需要Counter->Decrease()
		{
			Counter->Decrease();
		}
		Counter = Param.Counter;
		Info = Param.Info;
		ID = Param.ID;
		Counter->Increase();

		SourceObject = false;
	};

}




