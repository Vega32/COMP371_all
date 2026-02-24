#include "Shaders.h"

const char* getFlatPhongVertexShader()
{
	return R"(
#version 330 core
layout(location = 0) in vec3 positionVector;
layout(location = 1) in vec3 normalsVector;

uniform mat4 modelview;
uniform mat4 projection;

out vec3 normalsFragment;
out vec3 positionFragment;

void main() {
	vec4 cameraView = modelview * vec4(positionVector, 1.0);
	positionFragment = cameraView.xyz;

	normalsFragment = mat3(modelview) * normalsVector;
	
	gl_Position = projection * cameraView;
}

)";;
}

const char* getFlatPhongFragmentShader()
{
	return R"(
#version 330 core
in vec3 normalsFragment;
in vec3 positionFragment;
out vec4 fragmentColor;

void main(){
	vec3 diffuse = vec3(1.0, 0.5, 0.5);
	vec3 ambient = vec3(0.1, 0.05, 0.05);
	vec3 specular = vec3(0.3, 0.3, 0.3);
	vec3 light = vec3(1.0, 1.0, 1.0);
	float specExp = 5.0;

	vec3 positionLight = vec3(0.0, 0.0, 5.0);
	vec3 norm = normalize(normalsFragment);
	vec3 directionLight = normalize(positionLight - positionFragment);
	vec3 directionView = normalize(-positionFragment);
	vec3 directionReflection = reflect(-directionLight, norm);

	float relativeDiffuse = max(dot(norm, directionLight), 0.0);
	vec3 diffuseVector = relativeDiffuse * diffuse * light;
	
	float spec = pow(max(dot(directionView, directionReflection), 0.0), specExp);
	vec3 specularVector = spec * specular * light;

	fragmentColor = vec4(ambient + diffuseVector + specularVector, 1.0);
}
)";
}

const char* getBeryVertexShader()
{
	return R"(
#version 330 core
layout(location = 0) in vec3 positionVector;
layout(location = 1) in vec3 normalsVector;
layout(location = 2) in vec3 beryVector;

uniform mat4 modelview;
uniform mat4 projection;

out vec3 normalsFragment;
out vec3 positionFragment;
out vec3 beryFragment;

void main(){
	vec4 cameraView = modelview * vec4(positionVector, 1.0);
	positionFragment = cameraView.xyz;

	normalsFragment = mat3(modelview) * normalsVector;

	beryFragment = beryVector;
	
	gl_Position = projection * cameraView;
}	
)";
}

const char* getVoronoiFragmentShader()
{
	return R"(
#version 330 core
in vec3 normalsFragment;
in vec3 positionFragment;
in vec3 beryFragment;
out vec4 fragmentColor;

void main(){
	vec3 diffuse[3];
	diffuse[0] = vec3(1.0, 0.5, 0.5);
	diffuse[1] = vec3(0.5, 1.0, 0.5);
	diffuse[2] = vec3(0.5, 0.5, 1.0);

	vec3 diffuseColor;
	if (beryFragment.x >= beryFragment.y && beryFragment.x >= beryFragment.z){
		diffuseColor = diffuse[0];
	}else if (beryFragment.y >= beryFragment.x && beryFragment.y >= beryFragment.z){
		diffuseColor = diffuse[1];
	}else if (beryFragment.z >= beryFragment.y && beryFragment.z >= beryFragment.x){
		diffuseColor = diffuse[2];
	}

	vec3 ambient = vec3(0.1, 0.05, 0.05);
	vec3 specular = vec3(0.3, 0.3, 0.3);
	vec3 light = vec3(1.0, 1.0, 1.0);
	float specExp = 5.0;

	vec3 positionLight = vec3(0.0, 0.0, 5.0);
	vec3 norm = normalize(normalsFragment);
	vec3 directionLight = normalize(positionLight - positionFragment);
	vec3 directionView = normalize(-positionFragment);
	vec3 directionReflection = reflect(-directionLight, norm);

	float relativeDiffuse = max(dot(norm, directionLight), 0.0);
	vec3 diffuseVector = relativeDiffuse * diffuseColor * light;
	
	float spec = pow(max(dot(directionView, directionReflection), 0.0), specExp);
	vec3 specularVector = spec * specular * light;

	fragmentColor = vec4(ambient + diffuseVector + specularVector, 1.0);
}
)";
}

const char* getCircleFragmentShader()
{
	return nullptr;
}
