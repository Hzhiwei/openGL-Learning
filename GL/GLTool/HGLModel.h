#pragma once

#include "../GLAD/glad.h"
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <map>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "HGLShader.h"
#include "HGLTexture2D.h"
#include "HGLReferenceCounter.h"
#include "HGLCamera.h"


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
		HGLMesh::HGLMesh(const HGLMesh & Param);
		~HGLMesh();

		//���ض����б������ζ�������
		void Load(const vector<HMeshVertex> &vertex, const vector<GLuint> &index);
		//���ض����б������ζ�������������
		void Load(const vector<HMeshVertex> &vertex, const vector<GLuint> &index, const HGLTexture2D &tex);
		//��������
		void Load(const HGLTexture2D & tex);
		//������ɫ��
		void AttachShaderProgram(HGLShaderProgram & ImportShader);
		//��������
		void Draw(const HGLCamera & Camera);
		//�������
		void Clear();
		//����������GPU
		void SetupMesh();

	private:
		//�½�
		void CounterAndBufferManger();
		//��Param����
		void CounterAndBufferManger(const HGLMesh & Param);

	private:
		//�����б�
		vector<HMeshVertex> vertices;
		//�����ζ�������
		vector<GLuint> indices;
		//����
		HGLTexture2D texture;
		//������ʹ�õ���ɫ��
		HGLShaderProgram ShaderProgram;
		HGLReferenceCounter *Counter = NULL;

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
		void Draw(const HGLCamera & Camera);

	private:
		void processNode(aiNode *Node, const aiScene *Scene);
		void processMesh(aiMesh *Mesh, const aiScene *Scene);
		HGLTexture2D & processFristTexture(aiMaterial * Mat);

	private:
		vector<HGLMesh> meshList;
		string directory;
		std::map<std::string, HGLTexture2D> textures;
		HGLShaderProgram *ShaderProgram;
	};
}
