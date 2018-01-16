
##start vertexShader
#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTex;
out vec2 Tex;

uniform mat4 view;

void main()
{
   gl_Position = view * vec4(aPos, 1.0f);
   Tex = aTex;
}
##end


##start fragmentShader
#version 450 core

in vec2 Tex;

out vec4 FragColor;

uniform sampler2D texture_ambien0;

void main()
{
	FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}
##end
