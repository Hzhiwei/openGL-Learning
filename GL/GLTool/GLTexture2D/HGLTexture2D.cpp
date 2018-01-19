#include "HGLTexture2D.h"


namespace HGLTool
{
	HGLTexture2D::HGLTexture2D()
	{
		glGenTextures(1, &ID);
	}

	HGLTexture2D::HGLTexture2D(const glm::vec3  & Color)
	{
		PureColor(Color);
	}

	HGLTexture2D::HGLTexture2D(GLuint InternalFormate, GLuint W, GLuint H, GLuint Border, GLenum Type, const void * const Ptr, HTextureMode M)
	{
		glGenTextures(1, &ID);
		glBindTexture(GL_TEXTURE_2D, ID);
		LoadFromData(InternalFormate, W, H, Border, Type, Ptr);
		Mode = M;
	}

	HGLTexture2D::HGLTexture2D(string Path, HTextureMode M)
	{
		glGenTextures(1, &ID);
		LoadFromFile(Path);
		Mode = M;
	}

	HGLTexture2D::~HGLTexture2D()
	{
		glDeleteTextures(1, &ID);
	}

	void HGLTexture2D::LoadFromData(GLuint InternalFormate, GLuint W, GLuint H, GLuint Border, GLenum Type, const void * const Ptr)
	{
		glBindTexture(GL_TEXTURE_2D, ID);
		glTexImage2D(GL_TEXTURE_2D, 0, InternalFormate, W, H, 0, Border, Type, Ptr);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	bool HGLTexture2D::LoadFromFile(string Path)
	{
		int width, height, nrComponents;
		unsigned char *data = stbi_load(Path.c_str(), &width, &height, &nrComponents, 0);

		glBindTexture(GL_TEXTURE_2D, ID);

		if (data)
		{
			GLenum format;

			switch (nrComponents)
			{
				case 1: format = GL_RED; break;
				case 2: format = GL_RG; break;
				case 3: format = GL_RGB; break;
				case 4: format = GL_RGBA; break;
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

	void HGLTexture2D::PureColor(const glm::vec3 & Color)
	{
		LoadFromData(GL_RGB, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &Color);
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
}

