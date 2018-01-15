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
}
