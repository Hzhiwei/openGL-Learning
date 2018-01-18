
#include "HGLMesh.h"


namespace HGLTool
{
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

	void HGLMesh::Load(const std::vector<HGLMeshVertexStruct>& vertex, const std::vector<GLuint>& index)
	{
		vertices = vertex;
		indices = index;
		PointNum = indices.size();
	}

	void HGLMesh::Load(const std::vector<HGLMeshVertexStruct> &vertex, const std::vector<GLuint> &index, const HGLMaterialStruct & mate)
	{
		vertices = vertex;
		indices = index;
		Material = mate;
		PointNum = indices.size();
	}

	void HGLMesh::Load(const HGLMaterialStruct & mate)
	{
		Material = mate;
	}

	void HGLMesh::AttachShaderProgram(const std::shared_ptr<HGLShaderProgram> & ImportShader)
	{
		ShaderProgram = ImportShader;
	}

	void HGLMesh::Draw(const HGLCamera & Camera) const
	{
		glActiveTexture(GL_TEXTURE0);
		Material.diffuseTexture->ActiveAndBind(GL_TEXTURE0);
		glActiveTexture(GL_TEXTURE1);
		Material.specularTexture->ActiveAndBind(GL_TEXTURE1);

		ShaderProgram->Use();

		ShaderProgram->SetInt("Material.diffuseTexture", 0);
		ShaderProgram->SetInt("Material.specularTexture", 1);
		ShaderProgram->SetFloat("Material.shininess", Material.shininess);

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
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(HGLMeshVertexStruct), vertices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(HGLMeshVertexStruct), (void *)offsetof(HGLMeshVertexStruct, Position));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(HGLMeshVertexStruct), (void *)offsetof(HGLMeshVertexStruct, Normal));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(HGLMeshVertexStruct), (void *)offsetof(HGLMeshVertexStruct, TexCoords));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

		glBindVertexArray(0);
	}
}
