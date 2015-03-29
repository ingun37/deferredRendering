#version 330 core

uniform Uniforms
{
	mat4 mvp;
};

in VS_OUT
{
    vec4 color;
} fs_in;

out vec4 color;

void main(void)
{
    color = fs_in.color;
}