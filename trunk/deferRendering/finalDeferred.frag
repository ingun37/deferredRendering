#version 430 core

layout (location = 5) uniform vec4 lightDir;

layout (location = 6) uniform sampler2D diffuseMap;
layout (location = 7) uniform sampler2D normalMap;
layout (location = 8) uniform sampler2D positionMap;
layout (location = 9) uniform sampler2D textureMap;
layout (location = 10) uniform sampler2D shadowMap;

layout (location = 11) uniform vec4 eyePos;

in VS_OUT
{
	vec2 uv;
} fs_in;

out vec4 color;
void main(void)
{
	vec3 normedNorm;
	vec3 normedLightDir;
	vec4 texColor;
	vec3 rawNorm;
	float cosNL;
	float cosHN;
	vec3 Lcos;
	vec3 Lirradiance = vec3(0.5,0.5,0.5);
	vec3 vertexPos;
	vec3 viewV;
	float constM = 20.0;
	vec3 halfV;
	float specIntense;
	vec3 specc = vec3( 1,1,1 );
	vec3 finalSpecColor;
	float diffIntense = 1.0/3.141592;
	vec3 finaldiffcolor;
	vec3 finalColor;
	vec4 shadowColor;

	texColor = texture2D(textureMap,fs_in.uv);
	vertexPos = texture2D(positionMap, fs_in.uv).xyz;

	///lighting........
	rawNorm = texture2D(normalMap, fs_in.uv).xyz;
	normedNorm = normalize( rawNorm );
	normedLightDir = normalize( lightDir.xyz );
	cosNL = clamp( dot(normedLightDir,normedNorm),0.0,1.0);
	
	Lcos = cosNL * Lirradiance;
	viewV = eyePos.xyz - vertexPos;
	viewV = normalize(viewV);
	halfV = viewV + normedLightDir;
	halfV = normalize(halfV);
	cosHN = dot(normedNorm,halfV);
	specIntense = (constM/7.0 + 1.0)/8.0*3.141592*pow(cosHN,constM);
	finalSpecColor = specIntense * specc;
	finaldiffcolor = diffIntense * texColor.xyz;

	finalColor = (Lcos * (finaldiffcolor.xyz + finalSpecColor.xyz)) + (0.5 * texColor.xyz);
	
	shadowColor = texture2D(shadowMap,fs_in.uv);

	if(shadowColor.x < 0.5)
		finalColor = 0.4*finalColor + 0.6*shadowColor.xyz;
	
	color = vec4(finalColor,1);
}