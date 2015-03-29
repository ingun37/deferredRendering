#version 430 core

layout (location = 5) uniform sampler2D inTex;

in VS_OUT
{
    vec4 color;
	vec3 position;
	vec3 normal;
	vec2 uv;
} fs_in;

layout(location = 0)out vec4 color;
layout(location = 1)out vec4 position;
layout(location = 2)out vec4 normal;
layout(location = 3)out vec4 outTex;
void main(void)
{
    color = fs_in.color;
	position = vec4(fs_in.position,1);
	normal = vec4( fs_in.normal,1);
	outTex = texture2D(inTex, fs_in.uv);
}