#include "HGLScenes.h"


namespace HGLTool
{
	HGLScenes::HGLScenes()
	{
	}

	HGLScenes::~HGLScenes()
	{
	}

	void HGLScenes::AddModel(std::shared_ptr<HGLModel> Model)
	{
		Models.push_back(Model);
	}

	void HGLScenes::AddLight(std::shared_ptr<HGLLight> Light)
	{
		Lights.push_back(Light);
	}

	void HGLScenes::DynamicCompileShader()
	{
		for (int i = Models.size() - 1; i >= 0; --i)
		{
			Models[i]->SetLightsList(Lights);
			Models[i]->CompileShaderWithLights();
		}
	}
	void HGLScenes::Draw(const HGLCamera & camaera)
	{
		for (int i = Models.size() - 1; i >= 0; --i)
		{
			Models[i]->Draw(camaera);
		}
	}
}
