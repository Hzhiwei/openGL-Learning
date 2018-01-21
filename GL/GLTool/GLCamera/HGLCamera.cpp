#include "HGLCamera.h"

namespace HGLTool
{

	HGLCamera::HGLCamera(float Fovy, float Aspect, float Near, float Far)
	{
		projection = glm::perspective(Fovy, Aspect, Near, Far);
	}

	HGLCamera::HGLCamera(float Left, float Right, float Bottom, float Top, float Near, float Far)
	{
		projection = glm::ortho(Left, Right, Bottom, Top, Near, Far);
	}

	HGLCamera::~HGLCamera()
	{
	}
}

