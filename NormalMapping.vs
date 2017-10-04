#version 330 core

layout(location = 0) in vec3 vertexM;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 vertexN;
layout(location = 3) in vec3 vertexT;
layout(location = 4) in vec3 vertexBi;
out vec2 UV;
out vec3 vertexWorld;
out vec3 eyeDirectionCam;
out vec3 lightCam;
out vec3 lightTan;
out vec3 eyeTan;
uniform mat4 MVP;
uniform mat4 Model;
uniform mat4 View;
uniform mat3 ModelView2;
uniform vec3 lightWorld;

void main(){

	gl_Position =  MVP * vec4(vertexM,1);
	vertexWorld = (Model * vec4(vertexM,1)).xyz;
	vec3 vertexCamera = ( View * Model * vec4(vertexM,1)).xyz;
	eyeDirectionCam = vec3(0,0,0) - vertexCamera;
	vec3 lightPosCam = ( View * vec4(lightWorld,1)).xyz;
	lightCam = lightPosCam + eyeDirectionCam;
	UV = vertexUV;
	vec3 vertexTanCam = ModelView2 * vertexT;
	vec3 vertexBitanCam = ModelView2 * vertexBi;
	vec3 vertexNormCam = ModelView2 * vertexN;
	mat3 TBN = transpose(mat3(vertexTanCam,vertexBitanCam,vertexNormCam)); 
	lightTan = TBN * lightCam;
	eyeTan =  TBN * eyeDirectionCam;
		
}

