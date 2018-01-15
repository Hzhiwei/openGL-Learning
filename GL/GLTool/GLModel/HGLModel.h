#pragma once

#include "../../GLAD/glad.h"
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include <map>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../GLShader/HGLShader.h"
#include "../GLTexture2D/HGLTexture2D.h"
#include "../GLReferenceCounter/HGLReferenceCounter.h"
#include "../GLCamera/HGLCamera.h"
#include "../GLShader/HGLShaderProgram.h"


namespace HGLTool
{
	//mesh vertex struct
#	pragma pack(1)	//防止字节对齐引起问题（但由于都是float，即使不使用也不会有问题）
	typedef struct HMeshVertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
	}HMeshVertex;
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
		void Load(const vector<HMeshVertex> &vertex, const vector<GLuint> &index);
		//加载顶点列表，三角形顶点索引，纹理
		void Load(const vector<HMeshVertex> &vertex, const vector<GLuint> &index, const std::shared_ptr<HGLTexture2D> & tex);
		//加载纹理
		void Load(const std::shared_ptr<HGLTexture2D> & tex);
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
		vector<HMeshVertex> vertices;
		//三角形定点索引
		vector<GLuint> indices;
		//纹理
		std::shared_ptr<HGLTexture2D> texture;
		//此网格使用的着色器
		std::shared_ptr<HGLShaderProgram> ShaderProgram;

		GLuint VAO = 0;
		GLuint VBO = 0;
		GLuint EBO = 0;
		GLuint PointNum = 0;
	};

	class HGLModel
	{
	public:
		HGLModel();
		~HGLModel();

		bool Load(const string & Path);
		void Draw(const HGLCamera & Camera) const;
		void SetModelMatrix(const glm::mat4 Matrix);

	private:
		void processNode(aiNode *Node, const aiScene *Scene);
		void processMesh(aiMesh *Mesh, const aiScene *Scene);
		std::shared_ptr<HGLTexture2D> & processFristTexture(aiMaterial * Mat);

	private:
		vector<std::shared_ptr<HGLMesh>> meshList;
		string directory;
		std::map<std::string, std::shared_ptr<HGLTexture2D>> textures;
		//模型矩阵
		glm::mat4 ModelMatrix;
		std::shared_ptr<HGLShaderProgram> ShaderProgram;
	};
}
