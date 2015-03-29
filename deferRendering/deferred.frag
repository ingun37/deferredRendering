#version 430 core

in VS_OUT
{
    vec4 color;
	vec3 position;
	vec3 normal;
} fs_in;

layout(location = 0)out vec4 color;
layout(location = 1)out vec4 position;
layout(location = 2)out vec4 normal;
void main(void)
{
    color = fs_in.color;
	position = vec4(fs_in.position,1);
	normal = vec4( fs_in.normal,1);
}