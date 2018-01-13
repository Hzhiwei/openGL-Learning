
#include "HGLShaderProgram.h"

namespace HGLTool
{

	HGLShaderProgram::HGLShaderProgram()
	{

	}

	HGLShaderProgram::HGLShaderProgram(const HGLShaderProgram & Param)
	{
		MangerCounterAndShader(Param);
	}

	HGLShaderProgram::HGLShaderProgram(GLuint Vertex, GLuint Fragment)
	{
		MangerCounterAndShader();
		SetShader(Vertex, Fragment);
		Link();
	}


	HGLShaderProgram::~HGLShaderProgram()
	{
		if (Counter != NULL)
		{
			if (Counter->Decrease())
			{
				glDeleteProgram(ID);
				delete Info;
				delete Counter;
			}
		}
	}

	HGLShaderProgram & HGLShaderProgram::operator=(const HGLShaderProgram & Param)
	{
		MangerCounterAndShader(Param);
		return *this;
	}

	bool HGLShaderProgram::SetShader(GLuint Vertex, GLuint Fragment)
	{
		MangerCounterAndShader();
		glAttachShader(ID, Vertex);
		glAttachShader(ID, Fragment);
		return Link();
	}

	void HGLShaderProgram::SetShader(GLuint ShaderID)
	{
		MangerCounterAndShader();
		glAttachShader(ID, ShaderID);
	}

	bool HGLShaderProgram::Link()
	{
		GLint success;
		glLinkProgram(ID);
		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success)
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	std::string HGLShaderProgram::GetLinkInfo()
	{
		glGetProgramInfoLog(ID, INFOLENGTH, NULL, Info);
		return std::string(Info);
	}

	void HGLShaderProgram::Use()
	{
		glUseProgram(ID);
	}

	GLint HGLShaderProgram::GetUniform(const char * const Name) const
	{
		return glGetUniformLocation(ID, Name);
	}

	void HGLShaderProgram::SetBool(const char * const Name, bool value) const
	{
		glUniform1i(glGetUniformLocation(ID, Name), (int)value);
	}

	void HGLShaderProgram::SetInt(const char * const Name, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, Name), value);
	}

	void HGLShaderProgram::SetFloat(const char * const Name, float value) const
	{
		glUniform1f(glGetUniformLocation(ID, Name), value);
	}

	void HGLShaderProgram::SetMat4fv(const char * const Name, const GLfloat * value) const
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, Name), 1, GL_FALSE, value);
	}

	void HGLShaderProgram::SetMat4fv(const char * const Name, const glm::mat4 & value) const
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, Name), 1, GL_FALSE, &value[0][0]);
	}

	void HGLShaderProgram::MangerCounterAndShader()
	{
		if (Counter == NULL)
		{
			Counter = new HGLReferenceCounter();
			Counter->Increase();
			ID = glCreateProgram();
			Info = new char[INFOLENGTH];
		}
		else
		{
			if (!Counter->IsOnlyOne())
			{
				Counter->Decrease();
				Counter = new HGLReferenceCounter();
				Counter->Increase();
				ID = glCreateProgram();
				Info = new char[INFOLENGTH];
			}
		}
	}

	void HGLShaderProgram::MangerCounterAndShader(const HGLShaderProgram & Param)
	{
		if (Counter == NULL)
		{
			Counter = Param.Counter;
		}
		else
		{
			if (Counter->IsOnlyOne())
			{
				glDeleteProgram(ID);
				delete Info;
				delete Counter;

				Counter = Param.Counter;
			}
			else
			{
				Counter->Decrease();
				Counter = Param.Counter;
			}
		}
		Counter->Increase();
		ID = Param.ID;
		Info = Param.Info;
	}


	HGLShaderFileSource::HGLShaderFileSource()
	{
	}

	HGLShaderFileSource::HGLShaderFileSource(std::string Path)
	{
		LoadFile(Path);
	}


	HGLShaderFileSource::~HGLShaderFileSource()
	{
	}

	bool HGLShaderFileSource::LoadFile(std::string Path)
	{
		std::ifstream str(Path);

		if (!str.is_open())
		{
			return false;
		}

		FunctionAnalyze(str);

		str.close();

		return false;
	}

	bool HGLShaderFileSource::SetHeader(std::string Head)
	{
		if ((Head == "") || (Head == header))
		{
			return false;
		}
		else
		{
			header = Head;
			return true;
		}
	}

	bool HGLShaderFileSource::SetEnder(std::string End)
	{
		if ((End == "") || (End == ender))
		{
			return false;
		}
		else
		{
			ender = End;
			return true;
		}
	}

	std::string HGLShaderFileSource::GetShaderCode(std::string Name)
	{
		std::map<std::string, std::string>::iterator itor = FunctionMap.find(Name);
		if (itor == FunctionMap.end())
		{
			return "";
		}
		else
		{
			return FunctionMap[Name];
		}
	}

	void HGLShaderFileSource::Clear()
	{
		FunctionMap.clear();
	}

	void HGLShaderFileSource::FunctionAnalyze(std::ifstream &stream)
	{
		std::string line, key, source;

		FunctionMap.clear();

		while (true)
		{
			if (!getline(stream, line))
				//end of file
			{
				break;
			}

			int headerStartInLine = line.find(header);

			if (headerStartInLine != std::string::npos)
				//found header
			{
				std::string temp = line.substr(headerStartInLine + header.length());
				key = "";
				for (int i = 0; i < temp.length(); ++i)
				{
					if (temp[i] != ' ')
					{
						key += temp[i];
					}
				}

				source = "";
				while (true)
				{
					if (!getline(stream, line))
						//end of file
					{
						break;
					}

					if (line.find(ender) != std::string::npos)
						//found ender
					{
						FunctionMap[key] = source;
						break;
					}
					else
						// contious source code
					{
						source = source + '\n' + line;
					}
				}
			}
		}
	}

}

