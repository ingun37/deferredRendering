#version 430 core

layout (location = 5) uniform sampler2D tex;


in VS_OUT
{
    vec2 uv;
} fs_in;

out vec4 color;

void main(void)
{
	//color = vec4(0,1,1,1);
    color = texture2D(tex, fs_in.uv);
}