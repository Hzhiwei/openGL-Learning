#include "HGLModel.h"
#include <sstream>

namespace HGLTool
{
	static const char *vectorShader =
		"#version 450 core\n"
		"layout(location = 0) in vec3 aPos;\n"
		"layout(location = 1) in vec3 aNormal;\n"
		"layout(location = 2) in vec2 aTex;\n"
		"out vec3 FragPos;\n"
		"out vec3 Normal;\n"
		"out vec2 TexCoords;\n"
		"uniform mat4 view;\n"
		"uniform mat4 modelMatrix;\n"
		"void main()\n"
		"{\n"
		"	gl_Position = view * modelMatrix * vec4(aPos, 1.0f);\n"
		"	FragPos = (modelMatrix * vec4(aPos, 1.0f)).xyz;\n"
		"	Normal = aNormal;\n"
		"	TexCoords = aTex;\n"
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



	HGLModel::HGLModel()
	{
		glm::vec3 dataTemp(0.6f, 0.6f, 0.6f);
		std::shared_ptr<HGLTexture2D> tempDefaultTexture = std::shared_ptr<HGLTexture2D>(new HGLTexture2D(GL_RGB, 1, 1, 0, GL_FLOAT, &dataTemp));
		textures[""] = tempDefaultTexture;
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
		ShaderProgram->SetMat4fv("view", Camera.Get());
		ShaderProgram->SetMat4fv("modelMatrix", ModelMatrix);

		unsigned int ambientLightNum = 0;
		unsigned int parallelLightNum = 0;
		unsigned int pointLightNum = 0;
		unsigned int spotLightNum = 0;

		for (int i = lightsList->size() - 1; i >= 0; --i)
		{
			switch ((*lightsList)[i]->Mode)
			{
				case HGLLightMode::AmbientLight :
				{
					string numStringTemp = IntToString(ambientLightNum);
					ShaderProgram->SetVec3fv(string("AmbientLight[" + numStringTemp + "].color").c_str(), static_cast<HGLAmbientLight *>((*lightsList)[i].get())->Color);
					ShaderProgram->SetFloat(string("AmbientLight[" + numStringTemp + "].intensity").c_str(), static_cast<HGLAmbientLight *>((*lightsList)[i].get())->Intensity);
					ShaderProgram->SetFloat(string("AmbientLight[" + numStringTemp + "].diffuse").c_str(), static_cast<HGLAmbientLight *>((*lightsList)[i].get())->Diffuse);
					ShaderProgram->SetFloat(string("AmbientLight[" + numStringTemp + "].specular").c_str(), static_cast<HGLAmbientLight *>((*lightsList)[i].get())->Specular);
					++ambientLightNum;
					break;
				}
				case HGLLightMode::ParallelLight :
				{
					string numStringTemp = IntToString(parallelLightNum);
					ShaderProgram->SetVec3fv(string("ParallelLight[" + numStringTemp + "].color").c_str(), static_cast<HGLParallelLight *>((*lightsList)[i].get())->Color);
					ShaderProgram->SetVec3fv(string("ParallelLight[" + numStringTemp + "].direction").c_str(), static_cast<HGLParallelLight *>((*lightsList)[i].get())->Direction);
					ShaderProgram->SetFloat(string("ParallelLight[" + numStringTemp + "].diffuse").c_str(), static_cast<HGLParallelLight *>((*lightsList)[i].get())->Diffuse);
					ShaderProgram->SetFloat(string("ParallelLight[" + numStringTemp + "].specular").c_str(), static_cast<HGLParallelLight *>((*lightsList)[i].get())->Specular);
					++ambientLightNum;
					break;
				}
				case HGLLightMode::PointLight:
				{
					string numStringTemp = IntToString(pointLightNum);
					ShaderProgram->SetVec3fv(string("PointLight[" + numStringTemp + "].color").c_str(), static_cast<HGLPointLight *>((*lightsList)[i].get())->Color);
					ShaderProgram->SetVec3fv(string("PointLight[" + numStringTemp + "].position").c_str(), static_cast<HGLPointLight *>((*lightsList)[i].get())->Position);
					ShaderProgram->SetFloat(string("PointLight[" + numStringTemp + "].constant").c_str(), static_cast<HGLPointLight *>((*lightsList)[i].get())->Kc);
					ShaderProgram->SetFloat(string("PointLight[" + numStringTemp + "].linear").c_str(), static_cast<HGLPointLight *>((*lightsList)[i].get())->Kl);
					ShaderProgram->SetFloat(string("PointLight[" + numStringTemp + "].quadratic").c_str(), static_cast<HGLPointLight *>((*lightsList)[i].get())->Kd);
					ShaderProgram->SetFloat(string("PointLight[" + numStringTemp + "].diffuse").c_str(), static_cast<HGLPointLight *>((*lightsList)[i].get())->Diffuse);
					ShaderProgram->SetFloat(string("PointLight[" + numStringTemp + "].specular").c_str(), static_cast<HGLPointLight *>((*lightsList)[i].get())->Specular);
					++pointLightNum;
					break;
				}
				case HGLLightMode::SpotLight:
				{
					string numStringTemp = IntToString(spotLightNum);
					ShaderProgram->SetVec3fv(string("SpotLight[" + numStringTemp + "].color").c_str(), static_cast<HGLSpotLight *>((*lightsList)[i].get())->Color);
					ShaderProgram->SetVec3fv(string("SpotLight[" + numStringTemp + "].position").c_str(), static_cast<HGLSpotLight *>((*lightsList)[i].get())->Position);
					ShaderProgram->SetVec3fv(string("SpotLight[" + numStringTemp + "].direction").c_str(), static_cast<HGLSpotLight *>((*lightsList)[i].get())->Direction);
					ShaderProgram->SetFloat(string("SpotLight[" + numStringTemp + "].innercutOff").c_str(), static_cast<HGLSpotLight *>((*lightsList)[i].get())->InnerCutOffCos);
					ShaderProgram->SetFloat(string("SpotLight[" + numStringTemp + "].outerCutOff").c_str(), static_cast<HGLSpotLight *>((*lightsList)[i].get())->OuterCutOffCos);
					ShaderProgram->SetFloat(string("SpotLight[" + numStringTemp + "].constant").c_str(), static_cast<HGLSpotLight *>((*lightsList)[i].get())->Kc);
					ShaderProgram->SetFloat(string("SpotLight[" + numStringTemp + "].linear").c_str(), static_cast<HGLSpotLight *>((*lightsList)[i].get())->Kl);
					ShaderProgram->SetFloat(string("SpotLight[" + numStringTemp + "].quadratic").c_str(), static_cast<HGLSpotLight *>((*lightsList)[i].get())->Kd);
					ShaderProgram->SetFloat(string("SpotLight[" + numStringTemp + "].diffuse").c_str(), static_cast<HGLSpotLight *>((*lightsList)[i].get())->Diffuse);
					ShaderProgram->SetFloat(string("SpotLight[" + numStringTemp + "].specular").c_str(), static_cast<HGLSpotLight *>((*lightsList)[i].get())->Specular);
					++spotLightNum;
					break;
				}
			}
		}

		for (int i = 0; i < meshList.size(); ++i)
		{
			meshList[i]->Draw(Camera);
		}
	}

	void HGLModel::SetModelMatrix(const glm::mat4 Matrix)
	{
		ModelMatrix = Matrix;
	}

	void HGLModel::CompileShaderWithLights()
	{
		std::shared_ptr<HGLShader<GL_VERTEX_SHADER>> vertex = std::make_shared<HGLShader<GL_VERTEX_SHADER>>(vectorShader);
		//cout << "vertex:" << endl << vertex->GetCompileInfo() << endl;
		std::shared_ptr<HGLDynamicFragmentShader> fragment = std::make_shared<HGLDynamicFragmentShader>(*lightsList);
		//cout << "fragment:" << endl << fragment->GetCompileInfo() << endl;
		ShaderProgram = std::make_shared<HGLShaderProgram>(vertex->GetID(), fragment->GetID());
		cout << "program:" << endl << ShaderProgram->GetLinkInfo() << endl;
	}

	void HGLModel::SetLightsList(const std::vector<std::shared_ptr<HGLLight>> & Lights)
	{
		lightsList = &Lights;
	}

	string HGLModel::IntToString(int Param) const
	{
		stringstream ss;
		ss << Param;
		return ss.str();
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
		vector<HGLMeshVertexStruct> meshVertexTemp;
		vector<GLuint> meshIndexTemp;
		std::shared_ptr<HGLTexture2D> * diffTextureTemp;
		std::shared_ptr<HGLTexture2D> * specTextureTemp;
		HGLMaterialStruct materialTemp;

		for (int i = 0; i < Mesh->mNumVertices; ++i)
		{
			HGLMeshVertexStruct vertexTemp;
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
		materialTemp = processMaterial(material);

		std::shared_ptr<HGLMesh> meshTemp(new HGLMesh);
		meshTemp->Load(meshVertexTemp, meshIndexTemp, materialTemp);
		meshTemp->SetupMesh();
		meshTemp->AttachShaderProgram(ShaderProgram);
		meshList.push_back(meshTemp);
	}

	//aiTextureType_SPECULAR
	HGLMaterialStruct HGLModel::processMaterial(const aiMaterial * Mat)
	{
		aiString str;
		HGLMaterialStruct temp;
		std::shared_ptr<HGLTexture2D> tempTexture;

		Mat->GetTexture(aiTextureType_DIFFUSE, 0, &str);
		string diffTexturePath(str.C_Str());
		if (textures.find(diffTexturePath) == textures.end())
		{
			tempTexture = make_shared<HGLTexture2D>(directory + "/" + diffTexturePath);
			textures[diffTexturePath] = tempTexture;
			temp.diffuseTexture = tempTexture;
		}
		else
		{
			temp.diffuseTexture = textures[diffTexturePath];
		}

		Mat->GetTexture(aiTextureType_SPECULAR, 0, &str);
		string specTexturePath(str.C_Str());
		if (textures.find(specTexturePath) == textures.end())
		{
			tempTexture = make_shared<HGLTexture2D>(directory + "/" + specTexturePath);
			textures[specTexturePath] = tempTexture;
			temp.specularTexture = tempTexture;
		}
		else
		{
			temp.specularTexture = textures[diffTexturePath];
		}

		temp.shininess = 32;

		return temp;
	}
}
