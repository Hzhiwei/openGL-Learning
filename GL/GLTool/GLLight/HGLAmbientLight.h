#pragma once

#include "HGLLight.h"

namespace HGLTool
{
	class HGLAmbientLight : public HGLLight
	{
	public :
		HGLAmbientLight(float Intens = 0.02f) : HGLLight(HGLLightMode::AmbientLight), Intensity(Intens) {}
		~HGLAmbientLight() {}

	public:
		float Intensity;
	};
}
