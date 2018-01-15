#include "HGLModel.h"
#include <sstream>

namespace HGLTool
{
	static const char *vectorShader =
		"#version 450 core\n"
		"layout(location = 0) in vec3 aPos;\n"
		"layout(location = 1) in vec3 aNormal;\n"
		"layout(location = 2) in vec2 aTex;\n"
		"out vec2 Tex;\n"
		"uniform mat4 view;\n"
		"uniform mat4 modelMatrix;\n"
		"void main()\n"
		"{\n"
		"	gl_Position = view * modelMatrix * vec4(aPos, 1.0f);\n"
		"	Tex = aTex;\n"
		"}\n\0";
	
	static const char *fragmentShader =
		"#version 450 core\n"
		"in vec2 Tex;\n"
		"out vec4 FragColor;\n"
		"uniform sampler2D textureParam;\n"
		"void main()\n"
		"{\n"
		"	//FragColor = texture(textureParam, Tex);\n"
		"	FragColor = vec4(1.0f, 1.0f, 1.0f,1.0f);\n"
		"}\n\0";


	HGLMesh::HGLMesh()
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
	}

	HGLMesh::~HGLMesh()
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}

	void HGLMesh::Load(const vector<HMeshVertex>& vertex, const vector<GLuint>& index)
	{
		vertices = vertex;
		indices = index;
		PointNum = indices.size();
	}

	void HGLMesh::Load(const vector<HMeshVertex>& vertex, const vector<GLuint>& index, const std::shared_ptr<HGLTexture2D> & tex)
	{
		vertices = vertex;
		indices = index;
		texture = tex;
		PointNum = indices.size();
	}

	void HGLMesh::Load(const std::shared_ptr<HGLTexture2D> & tex)
	{
		texture = tex;
	}

	void HGLMesh::AttachShaderProgram(const std::shared_ptr<HGLShaderProgram> & ImportShader)
	{
		ShaderProgram = ImportShader;
	}

	void HGLMesh::Draw(const HGLCamera & Camera) const
	{
		glActiveTexture(GL_TEXTURE0);

		texture->ActiveAndBind(GL_TEXTURE0);
		ShaderProgram->Use();
		glBindVertexArray(VAO);

		glDrawElements(GL_TRIANGLES, PointNum, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glActiveTexture(GL_TEXTURE0);
	}

	void HGLMesh::Clear()
	{
		vertices.clear();
		indices.clear();
	}

	void HGLMesh::SetupMesh()
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(HMeshVertex), vertices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(HMeshVertex), (void *)offsetof(HMeshVertex, Position));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(HMeshVertex), (void *)offsetof(HMeshVertex, Normal));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(HMeshVertex), (void *)offsetof(HMeshVertex, TexCoords));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

		glBindVertexArray(0);
	}


	HGLModel::HGLModel()
	{
		HGLShader<GL_VERTEX_SHADER> vertex(vectorShader);
		cout << "vertex:" << endl << vertex.GetCompileInfo() << endl;
		HGLShader<GL_FRAGMENT_SHADER> fragment(fragmentShader);
		cout << "fragment:" << endl << fragment.GetCompileInfo() << endl;
		ShaderProgram = std::make_shared<HGLShaderProgram>(vertex.GetID(), fragment.GetID());
		cout << "program:" << endl << ShaderProgram->GetLinkInfo() << endl;
	}
	HGLModel::~HGLModel()
	{
		ShaderProgram.reset();
	}
	bool HGLModel::Load(const string & Path)
	{
		Assimp::Importer importer;
		const aiScene *scene = importer.ReadFile(Path, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
			return false;
		}

		textures.clear();
		meshList.clear();

		directory = Path.substr(0, Path.find_last_of('/'));

		processNode(scene->mRootNode, scene);

		return true;
	}
	void HGLModel::Draw(const HGLCamera & Camera) const
	{
		ShaderProgram->SetInt("textureParam", 0);
		ShaderProgram->SetMat4fv("view", Camera.Get());
		ShaderProgram->SetMat4fv("modelMatrix", ModelMatrix);
		for (int i = 0; i < meshList.size(); ++i)
		{
			meshList[i]->Draw(Camera);
		}
	}

	void HGLModel::SetModelMatrix(const glm::mat4 Matrix)
	{
		ModelMatrix = Matrix;
	}

	void HGLModel::processNode(aiNode * Node, const aiScene * Scene)
	{
		for (int i = 0; i < Node->mNumMeshes; ++i)
		{
			processMesh(Scene->mMeshes[Node->mMeshes[i]], Scene);
		}
		for (int i = 0; i < Node->mNumChildren; ++i)
		{
			processNode(Node->mChildren[i], Scene);
		}
	}

	void HGLModel::processMesh(aiMesh * Mesh, const aiScene * Scene)
	{
		vector<HMeshVertex> meshVertexTemp;
		vector<GLuint> meshIndexTemp;
		std::shared_ptr<HGLTexture2D> textureTemp(new HGLTexture2D);

		for (int i = 0; i < Mesh->mNumVertices; ++i)
		{
			HMeshVertex vertexTemp;
			if (Mesh->HasPositions())
			{
				vertexTemp.Position.x = Mesh->mVertices[i].x;
				vertexTemp.Position.y = Mesh->mVertices[i].y;
				vertexTemp.Position.z = Mesh->mVertices[i].z;
			}
			else
			{
				vertexTemp.Position.x = 0;
				vertexTemp.Position.y = 0;
				vertexTemp.Position.z = 0;
			}
			if (Mesh->HasNormals())
			{
				vertexTemp.Normal.x = Mesh->mNormals[i].x;
				vertexTemp.Normal.y = Mesh->mNormals[i].y;
				vertexTemp.Normal.z = Mesh->mNormals[i].z;
			}
			else
			{
				vertexTemp.Normal.x = 0;
				vertexTemp.Normal.y = 0;
				vertexTemp.Normal.z = 0;
			}
			if (Mesh->mTextureCoords[0])
			{
				vertexTemp.TexCoords.x = Mesh->mTextureCoords[0][i].x;
				vertexTemp.TexCoords.y = Mesh->mTextureCoords[0][i].y;
			}
			else
			{
				vertexTemp.TexCoords.x = 0;
				vertexTemp.TexCoords.y = 0;
			}
			meshVertexTemp.push_back(vertexTemp);
		}

		for (int i = 0; i < Mesh->mNumFaces; ++i)
		{
			aiFace face = Mesh->mFaces[i];
			for (int j = 0; j < face.mNumIndices; ++j)
			{
				meshIndexTemp.push_back(face.mIndices[j]);
			}
		}

		aiMaterial * material = Scene->mMaterials[Mesh->mMaterialIndex];
		textureTemp = processFristTexture(material);

		std::shared_ptr<HGLMesh> meshTemp(new HGLMesh);
		meshTemp->Load(meshVertexTemp, meshIndexTemp, textureTemp);
		meshTemp->SetupMesh();
		meshTemp->AttachShaderProgram(ShaderProgram);
		meshList.push_back(meshTemp);
	}

	std::shared_ptr<HGLTexture2D> & HGLModel::processFristTexture(aiMaterial * Mat)
	{
		aiString str;
		std::shared_ptr<HGLTexture2D> temp(new HGLTexture2D);
		Mat->GetTexture(aiTextureType_DIFFUSE, 0, &str);
		string texturePath(str.C_Str());
		if (textures.find(texturePath) == textures.end())
		{
			temp->LoadFromFile(directory + "/" + texturePath);
			textures[texturePath] = temp;
		}
		return textures[texturePath];
	}
}
