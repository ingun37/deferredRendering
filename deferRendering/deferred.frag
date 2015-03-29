#version 430 core

in VS_OUT
{
    vec4 color;
	vec3 normal;
} fs_in;

layout(location = 0)out vec4 color;
layout(location = 0)out vec4 normal;
void main(void)
{
    color = fs_in.color;
	normal = vec4( fs_in.normal, 1 );
}