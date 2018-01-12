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
		"void main()\n"
		"{\n"
		"	gl_Position = view * vec4(aPos, 1.0f);\n"
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
		CounterAndBufferManger();
	}

	HGLMesh::HGLMesh(const HGLMesh & Param)
	{
		CounterAndBufferManger(Param);
	}


	HGLMesh::~HGLMesh()
	{
		if (Counter != NULL)
		{
			if (Counter->Decrease())
			{
				if (VAO != 0)
				{
					glDeleteVertexArrays(1, &VAO);
				}
				if (VBO != 0)
				{
					glDeleteBuffers(1, &VBO);
				}
				if (EBO != 0)
				{
					glDeleteBuffers(1, &EBO);
				}
			}
		}
	}

	void HGLMesh::Load(const vector<HMeshVertex>& vertex, const vector<GLuint>& index)
	{
		vertices = vertex;
		indices = index;
		PointNum = indices.size();
	}

	void HGLMesh::Load(const vector<HMeshVertex>& vertex, const vector<GLuint>& index, const HGLTexture2D & tex)
	{
		vertices = vertex;
		indices = index;
		texture = tex;
		PointNum = indices.size();
	}

	void HGLMesh::Load(const HGLTexture2D & tex)
	{
		CounterAndBufferManger();
		texture = tex;
	}

	void HGLMesh::AttachShaderProgram(HGLShaderProgram & ImportShader)
	{
		ShaderProgram = ImportShader;
	}

	void HGLMesh::Draw(const HGLCamera & Camera)
	{
		glActiveTexture(GL_TEXTURE0);

		texture.ActiveAndBind(GL_TEXTURE0);
		ShaderProgram.SetInt("textureParam", 0);
		ShaderProgram.SetMat4fv("view", Camera.Get());
		ShaderProgram.Use();
		glBindVertexArray(VAO);

		glDrawElements(GL_TRIANGLES, PointNum, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glActiveTexture(GL_TEXTURE0);
	}

	void HGLMesh::Clear()
	{
		if (VAO != 0)
		{
			glDeleteVertexArrays(1, &VAO);
			VAO = 0;
		}
		if (VBO == 0)
		{
			glDeleteBuffers(1, &VBO);
			VBO = 0;
		}
		if (EBO == 0)
		{
			glDeleteBuffers(1, &EBO);
			EBO = 0;
		}
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

	void HGLMesh::CounterAndBufferManger()
	{
		if (Counter == NULL)
		{
			Counter = new HGLReferenceCounter();
			Counter->Increase();
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			glGenBuffers(1, &EBO);
		}
		else
		{
			if (!Counter->IsOnlyOne())
			{
				Counter->Decrease();
				Counter = new HGLReferenceCounter();
				Counter->Increase();
				glGenVertexArrays(1, &VAO);
				glGenBuffers(1, &VBO);
				glGenBuffers(1, &EBO);
			}
		}
	}

	void HGLMesh::CounterAndBufferManger(const HGLMesh & Param)
	{
		if (Counter == NULL)
		{
			Counter = Param.Counter;
			Counter->Increase();
			VAO = Param.VAO;
			VBO = Param.VBO;
			EBO = Param.EBO;
			ShaderProgram = Param.ShaderProgram;
			texture = Param.texture;
			PointNum = Param.PointNum;
		}
		else
		{
			if (!Counter->IsOnlyOne())
			{
				Counter->Decrease();
				Counter = new HGLReferenceCounter();
				Counter->Increase();
				VAO = Param.VAO;
				VBO = Param.VBO;
				EBO = Param.EBO;
				ShaderProgram = Param.ShaderProgram;
				texture = Param.texture;
				PointNum = Param.PointNum;
			}
		}
	}


	HGLModel::HGLModel()
	{
		HGLShader<GL_VERTEX_SHADER> vertex(vectorShader);
		cout << "vertex:" << endl << vertex.GetCompileInfo() << endl;
		HGLShader<GL_FRAGMENT_SHADER> fragment(fragmentShader);
		cout << "fragment:" << endl << fragment.GetCompileInfo() << endl;
		ShaderProgram = new HGLShaderProgram(vertex.GetID(), fragment.GetID());
		cout << "program:" << endl << ShaderProgram->GetLinkInfo() << endl;
	}
	HGLModel::~HGLModel()
	{
		if (ShaderProgram != NULL)
		{
			delete ShaderProgram;
		}
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
	void HGLModel::Draw(const HGLCamera & Camera)
	{
		for (int i = 0; i < meshList.size(); ++i)
		{
			meshList[i].Draw(Camera);
		}
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
		HGLTexture2D textureTemp;

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

		HGLMesh meshTemp;
		meshTemp.Load(meshVertexTemp, meshIndexTemp, textureTemp);
		meshTemp.SetupMesh();
		meshTemp.AttachShaderProgram(*ShaderProgram);
		meshList.push_back(meshTemp);
	}

	HGLTexture2D & HGLModel::processFristTexture(aiMaterial * Mat)
	{
		aiString str;
		HGLTexture2D temp;
		Mat->GetTexture(aiTextureType_DIFFUSE, 0, &str);
		string texturePath(str.C_Str());
		if (textures.find(texturePath) == textures.end())
		{
			temp.LoadFromFile(directory + "/" + texturePath);
			textures[texturePath] = temp;
		}
		return textures[texturePath];
	}
}
