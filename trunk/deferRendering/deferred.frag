#version 430 core

layout (location = 5) uniform sampler2D inTex;

layout (location = 6) uniform mat4 shadowPV;
layout (location = 7) uniform sampler2D shadowTex;

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
layout(location = 4)out vec4 shadow;
void main(void)
{
    color = fs_in.color;
	position = vec4(fs_in.position,1);
	normal = vec4( fs_in.normal,1);
	outTex = texture2D(inTex, fs_in.uv);

	vec4 fragPosByShadowPV;
	vec2 shadowMapUV;
	float depthFromShadowPV;
	float depthFromShadowTex;

	fragPosByShadowPV = shadowPV * position;
	shadowMapUV = (fragPosByShadowPV.xy + vec2(1,1))/2;
	depthFromShadowPV = (fragPosByShadowPV.z + 1)/2;
	depthFromShadowTex = texture2D( shadowTex, shadowMapUV ).x;

	shadow = vec4(0,0,0,1);
	if(abs(	depthFromShadowTex - depthFromShadowPV) < 0.001)
		shadow = vec4(1,1,1,1);
		

}