#pragma once


#include "HGLLight.h"
#include <glm.hpp>

namespace HGLTool
{
	class SpotLight : HGLLight
	{
	public:
		SpotLight() : 
			HGLLight(HGLLightMode::SpotLight),
			Direction(0.0f, 0.0f, 0.0f),
			Position(0.0f, 0.0f, -1.0f),
			InnerCutOffCos(cos(glm::radians(45.0f))),
			OuterCutOffCos(cos(glm::radians(60.0f))){}
		~SpotLight() {}

		void SetDirectionWithoutNormalize(const glm::vec3 D) {Direction = D; }
		void SetDirectionWithNormalize(const glm::vec3 D) {Direction = glm::normalize(D); }
		void SetPosition(const glm::vec3 P) {Position = P; }
		void SetInnerCutOff(float CutOff)
		{
			InnerCutOffCos = cos(CutOff);
			if (InnerCutOffCos < OuterCutOffCos)
			{
				OuterCutOffCos = InnerCutOffCos;
			}
		}
		void SetOuterCutOff(float CutOff)
		{
			OuterCutOffCos = cos(CutOff);
			if (OuterCutOffCos > InnerCutOffCos)
			{
				InnerCutOffCos = OuterCutOffCos;
			}
		}


	public:
		glm::vec3 Position;
		glm::vec3 Direction;

	private:
		float InnerCutOffCos;
		float OuterCutOffCos;
	};
}

