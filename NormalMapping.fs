#version 330 core

in vec2 UV;
in vec3 posWorld;
in vec3 eyeCam;
in vec3 lightCam;
in vec3 lightTan;
in vec3 eyeTan;
out vec3 color;
uniform sampler2D normalText;
uniform mat4 View;
uniform mat4 Model;
uniform mat3 ModelView2;
uniform vec3 lightWorld;

void main(){

	vec3 lightColor = vec3(1,1,1);
	float lightExponent = 30.0;	
	vec3 ambient = vec3(0.25,0.25,0.25);
	vec3 textNormTan = normalize(texture( normalText, vec2(UV.x,-UV.y) ).rgb*2.0 - 1.0);
	float distance = length( lightWorld - posWorld );
	vec3 normal = textNormTan;
	vec3 lightDir = normalize(lightTan);
	float cosTheta = clamp( dot( normal,lightDir ), 0,1 );
	vec3 eyeVec = normalize(eyeTan);
	vec3 reflecVec = reflect(-lightDir,normal);
	float cosAlpha = clamp( dot( eyeVec,reflecVec ), 0,1 );
	color = ambient + lightColor * lightExponent * cosTheta / (distance*distance) + lightColor * lightExponent * pow(cosAlpha,5) / (distance*distance);

}