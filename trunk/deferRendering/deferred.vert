#version 330 core

uniform Uniforms
{
	mat4 mvp;
};

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;
layout (location = 3) in vec4 diffuse;

out VS_OUT
{
	vec4 color;
} vs_out;



void main(void)
{
	mat4 tmp;
	tmp[0] = vec4(1.5,0,0,0);
	tmp[1] = vec4(0,2,0,0);
	tmp[2] = vec4(0,0,-1.004008,8.03607);
	tmp[3] = vec4(0,0,-1,10);

	gl_Position = mvp * vec4(position,1);
	vs_out.color = diffuse;
}