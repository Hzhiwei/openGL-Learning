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
#	pragma pack(1)	//��ֹ�ֽڶ����������⣨�����ڶ���float����ʹ��ʹ��Ҳ���������⣩
	typedef struct HMeshVertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
	}HMeshVertex;
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
		void Load(const vector<HMeshVertex> &vertex, const vector<GLuint> &index);
		//���ض����б������ζ�������������
		void Load(const vector<HMeshVertex> &vertex, const vector<GLuint> &index, const std::shared_ptr<HGLTexture2D> & tex);
		//��������
		void Load(const std::shared_ptr<HGLTexture2D> & tex);
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
		vector<HMeshVertex> vertices;
		//�����ζ�������
		vector<GLuint> indices;
		//����
		std::shared_ptr<HGLTexture2D> texture;
		//������ʹ�õ���ɫ��
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
		//ģ�;���
		glm::mat4 ModelMatrix;
		std::shared_ptr<HGLShaderProgram> ShaderProgram;
	};
}
