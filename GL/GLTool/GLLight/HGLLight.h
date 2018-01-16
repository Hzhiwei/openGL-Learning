#pragma once

#include <glm.hpp>

namespace HGLTool
{
	enum HGLLightMode
	{
		AmbientLight = 0,
		PointLight,
		ParallelLight,
		SpotLight
	};

	class HGLLight
	{
	public:
		HGLLight(HGLLightMode LightMode) : Mode(LightMode) {};
		~HGLLight() {};

	public:
		glm::u8vec3 Color;
		const HGLLightMode Mode;
	};
}
