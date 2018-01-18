#pragma once

#include "HGLShader.h"

#include <memory>
#include <vector>
#include <sstream>

#include "../../GLAD/glad.h"
#include "../GLLight/HGLLight.h"
#include "../GLLight/HGLAmbientLight.h"
#include "../GLLight/HGLParallelLight.h"
#include "../GLLight/HGLPointLight.h"
#include "../GLLight/HGLSpotLight.h""
#include "../GLTexture2D/HGLTexture2D.h"


namespace HGLTool
{
	//��̬������ɫ��
	/*
	��������Ĺ��ա�����vector��̬������ɫ��
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
