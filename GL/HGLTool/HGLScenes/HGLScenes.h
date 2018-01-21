#pragma once


#include <glm.hpp>
#include <vector>
#include <memory>

#include "../HGLModel/HGLModel.h"
#include "../HGLLight/HGLLight.h"
#include "../HGLLight/HGLParallelLight.h"
#include "../HGLLight/HGLPointLight.h"
#include "../HGLLight/HGLSpotLight.h"
#include "../HGLLight/HGLLight.h"
#include "../HGLLight/HGLAmbientLight.h"
#include "../HGLLight/HGLParallelLight.h"
#include "../HGLLight/HGLPointLight.h"
#include "../HGLLight/HGLSpotLight.h"
#include "../HGLCamera/HGLCamera.h"

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
		void DynamicCompileShader();
		void Draw(const HGLCamera & camaera);

	private:
		std::vector<std::shared_ptr<HGLModel>> Models;
		std::vector<std::shared_ptr<HGLLight>> Lights;
	};
}
