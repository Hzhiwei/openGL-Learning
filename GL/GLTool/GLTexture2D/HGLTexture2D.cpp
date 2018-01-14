#include "HGLTexture2D.h"


namespace HGLTool
{
	HGLTexture2D::HGLTexture2D()
	{

	}

	HGLTexture2D::HGLTexture2D(const HGLTexture2D & Param)
	{
		CounterAndTextureManger(Param);
	}

	HGLTexture2D::HGLTexture2D(GLuint InternalFormate, GLuint W, GLuint H, GLuint Border, GLenum Type, const void * const Ptr, HTextureMode M)
	{
		CounterAndTextureManger();
		glBindTexture(GL_TEXTURE_2D, ID);
		LoadFromData(InternalFormate, W, H, Border, Type, Ptr);
		Mode = M;
	}

	HGLTexture2D::HGLTexture2D(string Path, HTextureMode M)
	{
		CounterAndTextureManger();
		LoadFromFile(Path);
		Mode = M;
	}

	HGLTexture2D::~HGLTexture2D()
	{
		if (Counter != NULL)
		{
			if (Counter->Decrease())
			{
				glDeleteTextures(1, &ID);
				delete Counter;
			}
		}
	}

	HGLTexture2D & HGLTexture2D::operator=(const HGLTexture2D & Param)
	{
		CounterAndTextureManger(Param);
		return *this;
	}

	void HGLTexture2D::LoadFromData(GLuint InternalFormate, GLuint W, GLuint H, GLuint Border, GLenum Type, const void * const Ptr)
	{
		CounterAndTextureManger();

		glBindTexture(GL_TEXTURE_2D, ID);
		glTexImage2D(GL_TEXTURE_2D, 0, InternalFormate, W, H, 0, Border, Type, Ptr);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	bool HGLTexture2D::LoadFromFile(string Path)
	{
		int width, height, nrComponents;
		unsigned char *data = stbi_load(Path.c_str(), &width, &height, &nrComponents, 0);

		CounterAndTextureManger();

		glBindTexture(GL_TEXTURE_2D, ID);

		if (data)
		{
			GLenum format;
			if (nrComponents == 1)
			{
				format = GL_RED;
			}
			else if (nrComponents == 3)
			{
				format = GL_RGB;
			}
			else if (nrComponents == 4)
			{
				format = GL_RGBA;
			}

			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			stbi_image_free(data);
			return true;
		}
		else
		{
			return false;
		}
	}

	void HGLTexture2D::Bind()
	{
		glBindTexture(GL_TEXTURE_2D, ID);
	}

	void HGLTexture2D::ActiveAndBind(GLenum Unit) const
	{
		glActiveTexture(Unit);
		glBindTexture(GL_TEXTURE_2D, ID);
	}

	void HGLTexture2D::UnBind()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void HGLTexture2D::Parameteri(GLenum Pname, GLint Param)
	{
		glTexParameteri(GL_TEXTURE_2D, Pname, Param);
	}
	GLuint HGLTexture2D::GetID()
	{
		return ID;
	}

	void HGLTexture2D::CounterAndTextureManger()
	{
		if (Counter == NULL)
		{
			Counter = new HGLReferenceCounter();
			glGenTextures(1, &ID);

			Counter->Increase();
		}
		else
		{
			if (!Counter->IsOnlyOne())
			{
				Counter->Decrease();

				Counter = new HGLReferenceCounter();
				glGenTextures(1, &ID);

				Counter->Increase();
			}
			//如果此对象当前维护的纹理单元在内存中只有一个，则不需要Counter->Decrease()后再新建，
			//可以直接修改当前纹理单元和Counter
		}
		SourceObjects = true;
	}

	void HGLTexture2D::CounterAndTextureManger(const HGLTexture2D & Param)
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
				glDeleteTextures(1, &ID);
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
		ID = Param.ID;
		Counter->Increase();

		SourceObjects = false;
	}
}

