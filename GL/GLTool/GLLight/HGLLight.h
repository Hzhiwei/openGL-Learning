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
		glm::vec3 Color = glm::vec3(1.0f, 1.0f, 1.0f);
		float Diffuse = 1.0f;
		float Specular = 1.0f;
		const HGLLightMode Mode;
	};
}
