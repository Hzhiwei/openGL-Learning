#pragma once

#include "../../lib/glad/glad.h"
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "HGLMesh.h"
#include "../HGLShader/HGLShader.h"
#include "../HGLShader/HGLDynamicFragmentShader.h"
#include "../HGLShader/HGLShaderProgram.h"
#include "../HGLTexture2D/HGLTexture2D.h"
#include "../HGLCamera/HGLCamera.h"
#include "../HGLLight/HGLLight.h"
#include "../HGLLight/HGLAmbientLight.h"
#include "../HGLLight/HGLParallelLight.h"
#include "../HGLLight/HGLPointLight.h"
#include "../HGLLight/HGLSpotLight.h"


namespace HGLTool
{
	class HGLModel
	{
	public:
		HGLModel();
		~HGLModel();

		bool Load(const string & Path);
		void Draw(const HGLCamera & Camera) const;
		void SetModelMatrix(const glm::mat4 Matrix);
		void SetLightsList(const std::vector<std::shared_ptr<HGLLight>> & Lights);
		void CompileShaderWithLights();

	private:
		string IntToString(int Param) const;
		void processNode(aiNode *Node, const aiScene *Scene);
		void processMesh(aiMesh *Mesh, const aiScene *Scene);
		HGLMaterialStruct processMaterial(const aiMaterial * Mat);

	private:
		vector<std::shared_ptr<HGLMesh>> meshList;
		const std::vector<std::shared_ptr<HGLLight>> * lightsList = new std::vector<std::shared_ptr<HGLLight>>();
		string directory;
		std::map<std::string, std::shared_ptr<HGLTexture2D>> textures;
		//Ä£ÐÍ¾ØÕó
		glm::mat4 ModelMatrix;
		std::shared_ptr<HGLShaderProgram> ShaderProgram;
	};
}
