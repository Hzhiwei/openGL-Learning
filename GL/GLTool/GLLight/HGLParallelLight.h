#pragma once

#include "HGLLight.h"
#include <glm.hpp>

namespace HGLTool
{
	class HGLParallelLight : public HGLLight
	{
	public:
		HGLParallelLight(glm::vec3 Dir = glm::vec3(0.0f, 0.0f, 0.0f)) : HGLLight(HGLLightMode::ParallelLight) , Direction(Dir) {}
		~HGLParallelLight() {}

		void SetDir(const glm::vec3 D)
		{
			Direction = D;
		}

	public:
		glm::vec3 Direction;
	};
}

