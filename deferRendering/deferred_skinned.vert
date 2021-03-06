#version 430 core

layout (location = 0) uniform mat4 mvp;
layout (location = 1) uniform mat4 trans;
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;
layout (location = 3) in vec4 diffuse;
layout (location = 4) in mat4 skinmat1;
layout (location = 8) in mat4 skinmat2;
layout (location = 12) in mat4 skinmat3;

out VS_OUT
{
	vec4 color;
	vec3 position;
	vec3 normal;
	vec2 uv;
} vs_out;



void main(void)
{
	vec4 worldpos = (
		(transpose(skinmat1) * vec4(position,1))+
		(transpose(skinmat2) * vec4(position,1))+
		(transpose(skinmat3) * vec4(position,1))
	);
	gl_Position = mvp * worldpos;
	vs_out.color = diffuse;
	vs_out.position = (trans * worldpos).xyz;
	//vs_out.position = (trans * vec4(position,1)).xyz;
	vs_out.normal = normal;
	vs_out.uv = uv;
}