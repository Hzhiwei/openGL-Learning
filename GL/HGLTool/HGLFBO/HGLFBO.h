#pragma once

#include "../../lib/glad/glad.h"

namespace HGLTool
{
	class HGLFBO
	{
	public:
		HGLFBO(unsigned int W, unsigned int H);
		~HGLFBO();

	private:
		HGLFBO(const HGLFBO &) {}
		HGLFBO & operator=(const HGLFBO &) {}

	public:
		bool Check();
		void Bind();
		void Unbind();
		void Update();
		void Resize(unsigned int W, unsigned int H);

	private:
		GLuint ID;
		GLuint TextureColorbuffer;
		GLuint RBO;
		unsigned int Width = 512;
		unsigned int Height = 512;
		unsigned char * Data = NULL;
		unsigned int DataSize;
	};
}
