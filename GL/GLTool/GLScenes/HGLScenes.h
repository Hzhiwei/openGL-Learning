#pragma once

#include "../GLModel/HGLModel.h"
#include "../GLLight/HGLLight.h"
#include "../GLLight/HGLParallelLight.h"
#include "../GLLight/HGLPointLight.h"
#include "../GLLight/HGLSpotLight.h"

#include <glm.hpp>
#include <vector>
#include <memory>

#include "../GLLight/HGLLight.h"
#include "../GLLight/HGLAmbientLight.h"
#include "../GLLight/HGLParallelLight.h"
#include "../GLLight/HGLPointLight.h"
#include "../GLLight/HGLSpotLight.h"

namespace HGLTool
{
	class HGLScenes
	{
	public:
		HGLScenes();
		~HGLScenes();

	private:
		HGLScenes(const HGLScenes &) {}
		HGLScenes & operator=(const HGLScenes &) {}

	public:             
		
	public:
		void AddModel(std::shared_ptr<HGLModel> Model);
		void AddLight(std::shared_ptr<HGLLight> Light);

	private:
		std::vector<std::shared_ptr<HGLModel>> Models;
		std::vector<std::shared_ptr<HGLLight>> Lights;
	};
}
