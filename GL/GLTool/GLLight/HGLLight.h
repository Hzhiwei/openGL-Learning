#pragma once

#include <glm.hpp>

namespace HGLTool
{
	enum HGLLightMode
	{
		PointLight = 0,
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

	protected:
		const HGLLightMode Mode;
	};
}
