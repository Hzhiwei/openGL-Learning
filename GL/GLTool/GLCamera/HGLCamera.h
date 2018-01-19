#pragma once

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>


namespace HGLTool
{
	//右，上，逆时针为正
	class HGLCamera
	{
	public:
		template<typename T>
		//透视投影
		//Fovy		: fovy angle
		//Aspect	: 宽高比
		//Near		: 近平面
		//Far		: 远平面
		HGLCamera(T Fovy, T Aspect, T Near, T Far);
		//正视投影
		//Left		: 左
		//Right		: 右
		//Bottom	: 底
		//Top		: 顶
		//Near		: 近平面
		//Far		: 远平面
		template<typename T>
		HGLCamera(T Left, T Right, T Bottom, T Top, T Near, T Far);

		~HGLCamera();

		void SetPos(glm::vec3 &Pos);
		void SetFront(glm::vec3 &Front);
		void SetUp(glm::vec3 &Up);
		void MoveHorizon(float Offset);
		void MoveVertical(float Offset);
		void MoveDeep(float Offset);
		void Move(const glm::vec3 &Offset);
		void RotateHorizon(float Angle);
		void RotateVertical(float Angle);
		void RotateDeep(float Angle);
		glm::vec3 GetFront();
		glm::vec3 GetPos();
		glm::mat4 Get() const;

		glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, 1.0f);
		glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	private:
		glm::mat4 projection;
	};

	template<typename T>
	HGLCamera::HGLCamera(T Fovy, T Aspect, T Near, T Far)
	{
		projection = glm::perspective(Fovy, Aspect, Near, Far);
	}

	template<typename T>
	HGLCamera::HGLCamera(T Left, T Right, T Bottom, T Top, T Near, T Far)
	{
		projection = glm::ortho(Left, Right, Bottom, Top, Near, Far);
	}

	inline void HGLCamera::SetPos(glm::vec3 & Pos)
	{
		cameraPos = Pos;
	}
	inline void HGLCamera::SetFront(glm::vec3 & Front)
	{
		cameraFront = Front;
	}
	inline void HGLCamera::SetUp(glm::vec3 & Up)
	{
		cameraUp = Up;
	}
	inline void HGLCamera::MoveHorizon(float Offset)
	{
		cameraPos = cameraPos + glm::normalize(glm::cross(cameraFront, cameraUp)) * Offset;
	}
	inline void HGLCamera::MoveVertical(float Offset)
	{
		cameraPos = cameraPos + cameraUp * Offset;
	}
	inline void HGLCamera::MoveDeep(float Offset)
	{
		cameraPos = cameraPos + cameraFront * Offset;
	}
	inline void HGLCamera::Move(const glm::vec3 & Offset)
	{
		cameraPos = cameraPos
			+ glm::normalize(glm::cross(cameraFront, cameraUp)) * Offset.x
			+ cameraUp * Offset.y
			+ cameraFront * Offset.z;
	}
	inline void HGLCamera::RotateHorizon(float Angle)
	{
		cameraFront = glm::normalize(cameraFront + tan(Angle) * glm::normalize(glm::cross(cameraFront, cameraUp)));
	}
	inline void HGLCamera::RotateVertical(float Angle)
	{
		cameraFront = glm::normalize(cameraFront + tan(Angle) * cameraUp);
		cameraUp = glm::normalize(cameraUp - tan(Angle) * cameraFront);
	}
	inline void HGLCamera::RotateDeep(float Angle)
	{
		cameraUp = glm::normalize(cameraUp - tan(Angle) * glm::normalize(glm::cross(cameraFront, cameraUp)));
	}
	inline glm::vec3 HGLCamera::GetFront()
	{
		return cameraFront;
	}
	inline glm::vec3 HGLCamera::GetPos()
	{
		return cameraPos;
	}
	inline glm::mat4 HGLCamera::Get() const
	{
		return projection * glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	}
}
