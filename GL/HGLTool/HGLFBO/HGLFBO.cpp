#include "HGLFBO.h"


namespace HGLTool
{
	HGLFBO::HGLFBO(unsigned int W, unsigned int H)
	{
		Width = W;
		Height = H;

		Data = new unsigned char[W * H * 3];
		DataSize = Width * Height;

		glGenFramebuffers(1, &ID);
		glBindFramebuffer(GL_FRAMEBUFFER, ID);
		glGenTextures(1, &TextureColorbuffer);
		glBindTexture(GL_TEXTURE_2D, TextureColorbuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TextureColorbuffer, 0);
		glGenRenderbuffers(1, &RBO);
		glBindRenderbuffer(GL_RENDERBUFFER, RBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Width, Height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
	}


	HGLFBO::~HGLFBO()
	{
	}

	bool HGLFBO::Check()
	{
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	void HGLFBO::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, ID);
	}
	void HGLFBO::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void HGLFBO::Update()
	{
		if (Width * Height > DataSize)
		{
			delete Data;
			Data = new unsigned char[Width * Height * 3];
			DataSize = Width * Height;
		}
		glBindTexture(GL_TEXTURE_2D, TextureColorbuffer);
		glGetTexImage(GL_TEXTURE_2D,
			0,
			GL_RGB,
			GL_UNSIGNED_BYTE,
			(GLvoid *)Data
		);
	}

	void HGLFBO::Resize(unsigned int W, unsigned int H)
	{
		Width = W;
		Height = H;
		glBindFramebuffer(GL_FRAMEBUFFER, ID);
		glBindTexture(GL_TEXTURE_2D, TextureColorbuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TextureColorbuffer, 0);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Width, Height);
	}
}
