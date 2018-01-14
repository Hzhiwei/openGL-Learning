#pragma once

#include "HGLLight.h"
#include <glm.hpp>

namespace HGLTool
{
	class HGLParallelLight : HGLLight
	{
	public:
		HGLParallelLight() : HGLLight() , Direction(0.0f, 0.0f, 0.0f) {}
		~HGLParallelLight() {}

		void SetDir(const glm::vec3 D)
		{
			Direction = D;
		}

	public:
		glm::vec3 Direction;
	};
}

