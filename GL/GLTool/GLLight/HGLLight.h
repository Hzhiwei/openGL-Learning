#pragma once

#include <glm.hpp>

namespace HGLTool
{
	enum HGLLightMode
	{
		PointLight = 0,
		ParallelLight,
		Spotlight
	};

	class HGLLight
	{
	public:
		HGLLight();
		~HGLLight();

	public:
		glm::u8vec3 Color;
	};
}
