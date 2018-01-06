#pragma once

#include "../GLAD/glad.h"
#include <iostream>
#include <string>
#include <map>
#include <fstream> 
#include <stdexcept>
#include <glm.hpp>

#include "HGLReferenceCounter.h"


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
		bool SourceObject = false;
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
		CounterAndShaderManger();
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
		CounterAndShaderManger();
		return *this;
	}

	template<int ShaderType>
	bool HGLShader<ShaderType>::Source(GLchar const * const Code)
	{
		if (SourceObject)
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
		else
		{
			return false;
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
			{
				Counter->Decrease();

				Counter = new HGLReferenceCounter();
				ID = glCreateShader(ShaderType);
				Info = new char[INFOLENGTH];

				Counter->Increase();
			}
			//如果此对象当前维护的纹理单元在内存中只有一个，则不需要Counter->Decrease()后再新建，
			//可以直接修改当前纹理单元和Counter
		}
		SourceObject = true;
	}

	template<int ShaderType>
	inline void HGLShader<ShaderType>::CounterAndShaderManger(const HGLShader<ShaderType> & Param)
	{
		if (Counter == NULL)
		{
			Counter = Param.Counter;
		}
		else
		{
			if (Counter->IsOnlyOne())
				//如果此对象当前维护的纹理单元在内存中只有一个，则需要删除当前的纹理单元和Counter，并指向指定纹理单元和Counter
			{
				glDeleteShader(ID);
				delete Info;
				delete Counter;

				Counter = Param.Counter;
			}
			else
				//如果此对象当前维护的纹理单元在内存中不止一个，则需要删除Counter->Decrease()后指向指定Counter
			{
				Counter->Decrease();
				Counter = Param.Counter;
			}
		}
		Info = Param.Info;
		ID = Param.ID;
		Counter->Increase();

		SourceObject = false;
	}



	class HGLShaderProgram
	{
	public:
		//默认构造函数
		HGLShaderProgram();
		//拷贝构造函数
		HGLShaderProgram(const HGLShaderProgram & Param);
		//输入顶点着色器ID和片段着色器ID的构造函数，内部直接链接
		HGLShaderProgram(GLuint Vertex, GLuint Fragment);
		//析构
		~HGLShaderProgram();
		HGLShaderProgram & operator=(const HGLShaderProgram & Param);

		//设置顶点着色器，片段着色器
		void SetShader(GLuint Vertex, GLuint Fragment);
		//获取链接信息
		std::string GetLinkInfo();
		//使用此着色器
		void Use();
		//获取全局变量地址
		GLint GetUniform(const char * const Name) const;
		void SetBool(const char * const Name, bool value) const;
		void SetInt(const char * const Name, int value) const;
		void SetFloat(const char * const Name, float value) const;
		void SetMat4fv(const char * const Name, const GLfloat *value) const;
		void SetMat4fv(const char * const Name, const glm::mat4 &value) const;

	private:
		//链接程序
		bool Link();
		void MangerCounterAndShader();
		void MangerCounterAndShader(const HGLShaderProgram & Param);

	private:
		GLuint ID = 0;
		GLuint VertexShaderID = 0;
		GLuint FragmentShaderID = 0;
		char *Info;

		HGLReferenceCounter *Counter = NULL;
		bool SourceObject = false;
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




