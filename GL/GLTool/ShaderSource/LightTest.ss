
HGLParallelLight Test Shader

##start ParallelLightVertexShader
#version 450 core
layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;
out vec2 TexCoord;
uniform mat4 view;
uniform mat4 


##end

##start test

#version 450 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 

struct AmbientLight
{
    vec3 color;
	float intensity;
};

struct ParallelLight
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight
{
    vec3 position;
    float constant;
    float linear;
    float quadratic;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};
void main()
{
	FragColor = texture(textureParam, Tex);
}


vec3 CalcAmbientLight(AmbientLight_Struct light)
{
	return light.color * light.intensity;
}
vec3 CalcParallelLight(ParallelLight_Struct light, vec3 normal, vec3 viewDir)
{
	vec3 lightUnDir = normalize(-light.direction);
	float diff = max(dot(normal, lightUnDir), 0.0f);
	vec3 reflectDir = reflect(-lightUnDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), Material.shininess);
	vec3 diffuse = light.diffuse * diff * vec3(texture(Material.diffuse, TexCoords));
	vec3 specular = light.diffuse * spec * vec3(texture(Material.specular, TexCoords));
	return (diffuse + specular);
}
vec3 CalcPointLight(PointLight_Struct light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightUnDir = normalize(light.position - fragPos);
	float diff = max(dot(normal, lightUnDir), 0.0f);
	vec3 reflectDir = reflect(-lightUnDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), Material.shininess);
	float distance = length(light.position - fragPos);
	float attenuation = 1.0f / (light.constant + light.liner * distance + light.quadratic * distance * distance));
	vec3 diffuse = light.diffuse * diff * vec3(texture(Material.diffuse, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(Material.specular, TexCoords));
    diffuse *= attenuation;
    specular *= attenuation;
	return (diffuse + specular);
}
vec3 CalcSpotLight(SpotLight_Struct light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightUnDir = normalize(light.position - fragPos);
	float diff = max(dot(normal, lightUnDir), 0.0f);
	vec3 reflectDir = reflect(-lightUnDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), Material.shininess);
	float distance = length(light.position - fragPos);
	float attenuation = 1.0f / (light.constant + light.liner * distance + light.quadratic * distance * distance));
	float cosTheta = dot(lightUnDir, normalize(-light.direction));
    float epsilon = light.innercutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(Material.diffuse, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(Material.specular, TexCoords));
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (diffuse + specular);
}









##end

