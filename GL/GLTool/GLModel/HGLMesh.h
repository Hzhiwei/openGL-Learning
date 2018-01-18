#pragma once

#include "../../GLAD/glad.h"
#include <glm.hpp>
#include <vector>
#include <memory>

#include "HGLMesh.h"
#include "../GLShader/HGLShader.h"
#include "../GLShader/HGLDynamicFragmentShader.h"
#include "../GLShader/HGLShaderProgram.h"
#include "../GLTexture2D/HGLTexture2D.h"
#include "../GLCamera/HGLCamera.h"


namespace HGLTool
{
	//mesh vertex struct
#	pragma pack(1)	//��ֹ�ֽڶ����������⣨�����ڶ���float����ʹ��ʹ��Ҳ���������⣩
	typedef struct HGLMeshVertexStruct
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
	}HGLMeshVertexStruct;

	typedef struct HGLMaterialStruct
	{
		std::shared_ptr<HGLTexture2D> diffuseTexture;
		std::shared_ptr<HGLTexture2D> specularTexture;
		float shininess;
	}HGLMaterialStruct;
#	pragma pack()


	//Mesh��
	class HGLMesh
	{
	public:
		HGLMesh();
		~HGLMesh();

	private:
		HGLMesh(const HGLMesh & Param) {};
		HGLMesh & operator=(const HGLMesh & Param) {}

	public:
		//���ض����б������ζ�������
		void Load(const std::vector<HGLMeshVertexStruct> &vertex, const std::vector<GLuint> &index);
		//���ض����б������ζ�������������
		void Load(const std::vector<HGLMeshVertexStruct> &vertex, const std::vector<GLuint> &index, const HGLMaterialStruct & mate);
		//���ز���
		void Load(const HGLMaterialStruct & mate);
		//������ɫ��
		void AttachShaderProgram(const std::shared_ptr<HGLShaderProgram> & ImportShader);
		//��������
		void Draw(const HGLCamera & Camera) const;
		//�������
		void Clear();
		//����������GPU
		void SetupMesh();

	private:
		//�����б�
		std::vector<HGLMeshVertexStruct> vertices;
		//�����ζ�������
		std::vector<GLuint> indices;
		//������ʹ�õ���ɫ��
		std::shared_ptr<HGLShaderProgram> ShaderProgram;
		//���������
		HGLMaterialStruct Material;

		GLuint VAO = 0;
		GLuint VBO = 0;
		GLuint EBO = 0;
		GLuint PointNum = 0;
	};
}

