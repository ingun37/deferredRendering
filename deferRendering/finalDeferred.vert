#version 430 core

layout (location = 4) uniform mat4 mvp;

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;
layout (location = 3) in vec4 diffuse;
layout (location = 4) in mat4 skinmat;

out VS_OUT
{
	vec2 uv;
} vs_out;



void main(void)
{
	gl_Position = mvp * vec4(position,1);
	vs_out.uv = uv;
}