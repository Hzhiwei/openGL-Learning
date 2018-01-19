#pragma once

#include "HGLLight.h"
#include <glm.hpp>

namespace HGLTool
{
	class HGLParallelLight : public HGLLight
	{
	public:
		HGLParallelLight(glm::vec3 Dir) : HGLLight(HGLLightMode::ParallelLight) , Direction(Dir) {}
		~HGLParallelLight() {}

	public :
		virtual glm::vec3 GetDir() 
		{
			return Direction;
		}

	public:
		glm::vec3 Direction;
	};
}

