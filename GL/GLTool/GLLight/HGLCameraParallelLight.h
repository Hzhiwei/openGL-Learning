#pragma once

#include "HGLParallelLight.h"
#include "../GLCamera/HGLCamera.h"


namespace HGLTool
{
	class HGLCameraParallelLight : public HGLParallelLight
	{
	public:
		HGLCameraParallelLight(HGLCamera * Camera) : HGLParallelLight(Camera->GetFront()), Fulcrum(Camera){}
		~HGLCameraParallelLight() {}

	public:
		virtual glm::vec3 GetDir()
		{
			return Fulcrum->GetFront();
		}

	private:
		HGLCamera * Fulcrum;
	};
}

