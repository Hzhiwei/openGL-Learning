#pragma once

#include "../GLModel/HGLModel.h"
#include "../GLLight/HGLLight.h"
#include "../GLLight/HGLParallelLight.h"
#include "../GLLight/HGLPointLight.h"
#include "../GLLight/HGLSpotLight.h"

#include <glm.hpp>
#include <vector>
#include <memory>

namespace HGLTool
{
	class HGLScenes
	{
	public:
		HGLScenes();
		~HGLScenes();

		void AddModel(std::shared_ptr<HGLModel> Model);
		void AddLight(std::shared_ptr<HGLLight> Light);

	private:
		std::vector<std::shared_ptr<HGLModel>> Models;
		std::vector<std::shared_ptr<HGLLight>> Lights;
	};
}
