#pragma once

#include "HGLLight.h"

namespace HGLTool
{
	class HGLAmbientLight : public HGLLight
	{
	public :
		HGLAmbientLight() : HGLLight(HGLLightMode::AmbientLight), Intensity(0.02f) {}
		~HGLAmbientLight() {}

	public:
		float Intensity;
	};
}
