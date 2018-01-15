#pragma once

#include "HGLShader.h"
#include "../../GLAD/glad.h"


namespace HGLTool
{
	class HGLDynamicLightFragmentShader : public HGLShader<GL_FRAGMENT_SHADER>
	{
	public:
		HGLDynamicLightFragmentShader();
		~HGLDynamicLightFragmentShader();
	};
}
