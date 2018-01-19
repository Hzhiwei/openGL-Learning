#pragma once

#include "HGLLight.h"

namespace HGLTool
{
	class HGLAmbientLight : public HGLLight
	{
	public :
		HGLAmbientLight() : HGLLight(HGLLightMode::AmbientLight) {}
		~HGLAmbientLight() {}

	public:
		float Intensity = 1.0f;
	};
}
