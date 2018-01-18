#pragma once

#include "../../GLAD/glad.h"
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "HGLMesh.h"
#include "../GLShader/HGLShader.h"
#include "../GLShader/HGLDynamicFragmentShader.h"
#include "../GLShader/HGLShaderProgram.h"
#include "../GLTexture2D/HGLTexture2D.h"
#include "../GLCamera/HGLCamera.h"
#include "../GLLight/HGLLight.h"
#include "../GLLight/HGLAmbientLight.h"
#include "../GLLight/HGLParallelLight.h"
#include "../GLLight/HGLPointLight.h"
#include "../GLLight/HGLSpotLight.h"


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
		const std::vector<std::shared_ptr<HGLLight>> * lightsList = NULL;
		string directory;
		std::map<std::string, std::shared_ptr<HGLTexture2D>> textures;
		//Ä£ÐÍ¾ØÕó
		glm::mat4 ModelMatrix;
		std::shared_ptr<HGLShaderProgram> ShaderProgram;
	};
}
