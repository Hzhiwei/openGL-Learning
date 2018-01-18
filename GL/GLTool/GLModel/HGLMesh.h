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
#	pragma pack(1)	//防止字节对齐引起问题（但由于都是float，即使不使用也不会有问题）
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


	//Mesh类
	class HGLMesh
	{
	public:
		HGLMesh();
		~HGLMesh();

	private:
		HGLMesh(const HGLMesh & Param) {};
		HGLMesh & operator=(const HGLMesh & Param) {}

	public:
		//加载顶点列表，三角形顶点索引
		void Load(const std::vector<HGLMeshVertexStruct> &vertex, const std::vector<GLuint> &index);
		//加载顶点列表，三角形顶点索引，纹理
		void Load(const std::vector<HGLMeshVertexStruct> &vertex, const std::vector<GLuint> &index, const HGLMaterialStruct & mate);
		//加载材质
		void Load(const HGLMaterialStruct & mate);
		//设置着色器
		void AttachShaderProgram(const std::shared_ptr<HGLShaderProgram> & ImportShader);
		//绘制网格
		void Draw(const HGLCamera & Camera) const;
		//清除数据
		void Clear();
		//设置数据至GPU
		void SetupMesh();

	private:
		//顶点列表
		std::vector<HGLMeshVertexStruct> vertices;
		//三角形定点索引
		std::vector<GLuint> indices;
		//此网格使用的着色器
		std::shared_ptr<HGLShaderProgram> ShaderProgram;
		//此网格材质
		HGLMaterialStruct Material;

		GLuint VAO = 0;
		GLuint VBO = 0;
		GLuint EBO = 0;
		GLuint PointNum = 0;
	};
}

