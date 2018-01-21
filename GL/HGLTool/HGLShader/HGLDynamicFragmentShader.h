#pragma once

#include "../../lib/glad/glad.h"

#include "HGLShader.h"

#include <memory>
#include <vector>
#include <sstream>

#include "../HGLLight/HGLLight.h"
#include "../HGLLight/HGLAmbientLight.h"
#include "../HGLLight/HGLParallelLight.h"
#include "../HGLLight/HGLPointLight.h"
#include "../HGLLight/HGLSpotLight.h""
#include "../HGLTexture2D/HGLTexture2D.h"


namespace HGLTool
{
	//动态编译着色器
	/*
	根据输入的光照、纹理vector动态编译着色器
	*/
	class HGLDynamicFragmentShader : public HGLShader<GL_FRAGMENT_SHADER>
	{
	public:
		HGLDynamicFragmentShader();
		HGLDynamicFragmentShader(const std::vector<std::shared_ptr<HGLLight>> & Lights);
		~HGLDynamicFragmentShader();

	public:
		bool DynamicCompile(const std::vector<std::shared_ptr<HGLLight>> & Lights);
		string GetSourceCode();

	private:
		string IntToString(int Param) const;
		string sourceCode;
	};
}
