#include "HGLDynamicFragmentShader.h"



namespace HGLTool
{
	HGLDynamicFragmentShader::HGLDynamicFragmentShader()
		: HGLShader<GL_FRAGMENT_SHADER>()
	{
	}

	HGLDynamicFragmentShader::HGLDynamicFragmentShader(const std::vector<std::shared_ptr<HGLLight>>& Lights)
	{
		DynamicCompile(Lights);
	}


	HGLDynamicFragmentShader::~HGLDynamicFragmentShader()
	{
	}

	bool HGLDynamicFragmentShader::DynamicCompile(const std::vector<std::shared_ptr<HGLLight>> & Lights)
	{
		sourceCode = "";
		sourceCode += string(
			"#version 450 core\n"
			"in vec3 FragPos;\n"
			"in vec3 Normal;\n"
			"in vec2 TexCoords;\n"
			"out vec4 FragColor;\n"
			"struct Material_Struct\n"
			"{\n"
			"	sampler2D diffuseTexture;\n"
			"	sampler2D specularTexture;\n"
			"	float shininess;\n"
			"};\n"
			"struct AmbientLight_Struct\n"
			"{\n"
			"	vec3 color;\n"
			"	float intensity;\n"
			"	float diffuse;\n"
			"	float specular;\n"
			"};\n"
			"struct ParallelLight_Struct\n"
			"{\n"
			"	vec3 color;\n"
			"	vec3 direction;\n"
			"	float diffuse;\n"
			"	float specular;\n"
			"};\n"
			"struct PointLight_Struct\n"
			"{\n"
			"	vec3 color;\n"
			"	vec3 position;\n"
			"	float constant;\n"
			"	float linear;\n"
			"	float quadratic;\n"
			"	float diffuse;\n"
			"	float specular;\n"
			"};\n"
			"struct SpotLight_Struct"
			"{\n"
			"	vec3 color;\n"
			"	vec3 position;\n"
			"	vec3 direction;\n"
			"	float innercutOff;\n"
			"	float outerCutOff;\n"
			"	float constant;\n"
			"	float linear;\n"
			"	float quadratic;\n"
			"	float diffuse;\n"
			"	float specular;\n"
			"};\n"
			"uniform vec3 viewPos;\n"
			"uniform Material_Struct Material;\n" );

		unsigned int ambientLightNum = 0;
		unsigned int parallelLightNum = 0;
		unsigned int pointLightNum = 0;
		unsigned int spotLightNum = 0;

		//计算各类光照数量
		for (int i = Lights.size() - 1; i >= 0; --i)
		{
			switch (Lights[i]->Mode)
			{
				case HGLLightMode::AmbientLight : ++ambientLightNum; break;
				case HGLLightMode::ParallelLight: ++parallelLightNum; break;
				case HGLLightMode::PointLight: ++pointLightNum; break;
				case HGLLightMode::SpotLight: ++spotLightNum; break;
				default : break;
			}
		}

		//增加光照uniform
		if (ambientLightNum != 0)
		{
			sourceCode += string("#define NUM_OF_AMBIENTLIGHT ") + IntToString(ambientLightNum) + string("\n");
			sourceCode = sourceCode + string("uniform AmbientLight_Struct AmbientLight[NUM_OF_AMBIENTLIGHT];\n");
		}
		if (parallelLightNum != 0)
		{
			sourceCode += string("#define NUM_OF_PARALLELLIGHT ") + IntToString(parallelLightNum) + string("\n");
			sourceCode = sourceCode + string("uniform ParallelLight_Struct ParallelLight[NUM_OF_PARALLELLIGHT];\n");
		}
		if (pointLightNum != 0)
		{
			sourceCode += string("#define NUM_OF_POINTLIGHT ") + IntToString(pointLightNum) + string("\n");
			sourceCode = sourceCode + string("uniform PointLight_Struct PointLight[NUM_OF_POINTLIGHT];\n");
		}
		if (spotLightNum != 0)
		{
			sourceCode += string("#define NUM_OF_SPOTLIGHT ") + IntToString(spotLightNum) + string("\n");
			sourceCode = sourceCode + string("uniform SpotLight_Struct SpotLight[NUM_OF_SPOTLIGHT];\n");
		}

		//增加光照计算函数声明
		sourceCode += string(
			"vec3 CalcAmbientLight(AmbientLight_Struct light);\n"
			"vec3 CalcParallelLight(ParallelLight_Struct light, vec3 normal, vec3 viewDir);\n"
			"vec3 CalcPointLight(PointLight_Struct light, vec3 normal, vec3 fragPos, vec3 viewDir);\n"
			"vec3 CalcSpotLight(SpotLight_Struct light, vec3 normal, vec3 fragPos, vec3 viewDir);\n");

		//主函数开头部分
		sourceCode += string(
			"void main()\n"
			"{\n"
			"	vec3 norm = normalize(Normal);\n"
			"	vec3 viewDir = normalize(viewPos - FragPos);\n"
			"	vec3 result = vec3(0.0f, 0.0f, 0.0f);\n");

		//环境光计算
		if (ambientLightNum > 0)
		{
			sourceCode += string(
				"	for(int i = 0; i < NUM_OF_AMBIENTLIGHT; ++i)\n"
				"	{\n"
				"		result += CalcAmbientLight(AmbientLight[i]);\n"
				"	}\n");
		}

		//平行光计算
		if (parallelLightNum > 0)
		{
			sourceCode += string(
				"	for(int i = 0; i < NUM_OF_PARALLELLIGHT; ++i)\n"
				"	{\n"
				"		result += CalcParallelLight(ParallelLight[i], norm, viewDir);\n"
				"	}\n");
		}

		//点光源计算
		if (pointLightNum > 0)
		{
			sourceCode += string(
				"	for(int i = 0; i < NUM_OF_POINTLIGHT; ++i)\n"
				"	{\n"
				"		result += CalcPointLight(PointLight[i], norm, FragPos, viewDir);\n"
				"	}\n");
		}

		//聚光灯计算
		if (spotLightNum > 0)
		{
			sourceCode += string(
				"	for(int i = 0; i < NUM_OF_SPOTLIGHT; ++i)\n"
				"	{\n"
				"		result += CalcSpotLight(SpotLight[i], norm, FragPos, viewDir);\n"
				"	}\n");
		}

		//颜色合成输出
		sourceCode += string(
			"	FragColor = vec4(result, 1.0);\n"
			"}\n");

		//子函数
		sourceCode += string(
			"vec3 CalcAmbientLight(AmbientLight_Struct light)\n"
			"{\n"
			"	vec3 diffuse = light.diffuse * vec3(texture(Material.diffuseTexture, TexCoords)) * light.color;\n"
			"	vec3 specular = light.specular * vec3(texture(Material.specularTexture, TexCoords)) * light.color;\n"
			"	return (diffuse + specular) * light.intensity;\n"
			"	//return vec3(texture(Material.diffuseTexture, TexCoords)) * light.color;\n"
			"}\n"
			"vec3 CalcParallelLight(ParallelLight_Struct light, vec3 normal, vec3 viewDir)\n"
			"{\n"
			"	vec3 lightUnDir = normalize(-light.direction);\n"
			"	float diff = max(dot(normal, lightUnDir), 0.0f);\n"
			"	vec3 reflectDir = reflect(-lightUnDir, normal);\n"
			"	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), Material.shininess);\n"
			"	vec3 diffuse = light.diffuse * diff * vec3(texture(Material.diffuseTexture, TexCoords)) * light.color;\n"
			"	vec3 specular = light.specular * spec * vec3(texture(Material.specularTexture, TexCoords)) * light.color;\n"
			"	return (diffuse + specular);\n"
			"}\n"
			"vec3 CalcPointLight(PointLight_Struct light, vec3 normal, vec3 fragPos, vec3 viewDir)\n"
			"{\n"
			"	vec3 lightUnDir = normalize(light.position - fragPos);\n"
			"	float diff = max(dot(normal, lightUnDir), 0.0f);\n"
			"	vec3 reflectDir = reflect(-lightUnDir, normal);\n"
			"	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), Material.shininess);\n"
			"	float distance = length(light.position - fragPos);\n"
			"	float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * distance * distance);\n"
			"	vec3 diffuse = light.diffuse * diff * vec3(texture(Material.diffuseTexture, TexCoords)) * light.color;\n"
			"	vec3 specular = light.specular * spec * vec3(texture(Material.specularTexture, TexCoords)) * light.color;\n"
			"	diffuse *= attenuation;\n"
			"	specular *= attenuation;\n"
			"	return (diffuse + specular);\n"
			"}\n"
			"vec3 CalcSpotLight(SpotLight_Struct light, vec3 normal, vec3 fragPos, vec3 viewDir)\n"
			"{\n"
			"	vec3 lightUnDir = normalize(light.position - fragPos);\n"
			"	float diff = max(dot(normal, lightUnDir), 0.0f);\n"
			"	vec3 reflectDir = reflect(-lightUnDir, normal);\n"
			"	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), Material.shininess);\n"
			"	float distance = length(light.position - fragPos);\n"
			"	float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * distance * distance);\n"
			"	float cosTheta = dot(lightUnDir, normalize(light.direction));\n"
			"	float epsilon = light.innercutOff - light.outerCutOff;\n"
			"	float intensity = clamp((cosTheta - light.outerCutOff) / epsilon, 0.0, 1.0);\n"
			"	vec3 diffuse = light.diffuse * diff * vec3(texture(Material.diffuseTexture, TexCoords)) * light.color;\n"
			"	vec3 specular = light.specular * spec * vec3(texture(Material.specularTexture, TexCoords)) * light.color;\n"
			"	diffuse *= attenuation * intensity;\n"
			"	specular *= attenuation * intensity;\n"
			"	//return (diffuse + specular);\n"
			"	return vec3(cosTheta, 0, 0);\n"
			"}\n");

		cout << sourceCode << endl;

		return Source(sourceCode);
	}
	string HGLDynamicFragmentShader::GetSourceCode()
	{
		return sourceCode;
	}

	string HGLDynamicFragmentShader::IntToString(int Param) const
	{
		stringstream ss;
		ss << Param;
		return ss.str();
	}
}
