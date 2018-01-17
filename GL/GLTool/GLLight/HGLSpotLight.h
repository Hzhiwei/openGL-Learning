#pragma once


#include "HGLLight.h"
#include <glm.hpp>

namespace HGLTool
{
	class HGLSpotLight : public HGLLight
	{
	public:
		HGLSpotLight(glm::vec3 Pos = glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3 Dir = glm::vec3(0.0f, 0.0f, -1.0f),
			float ICOC = cos(glm::radians(45.0f)),
			float OCOC = cos(glm::radians(60.0f))) :
			HGLLight(HGLLightMode::SpotLight),
			Position(Pos),
			Direction(Dir),
			InnerCutOffCos(ICOC),
			OuterCutOffCos(OCOC){}
		~HGLSpotLight() {}

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

