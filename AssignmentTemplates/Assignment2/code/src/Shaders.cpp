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
layout(location = 2) in vec3 v1;
layout(location = 3) in vec3 v2;
layout(location = 4) in vec3 v3;

uniform mat4 modelview;
uniform mat4 projection;

out vec3 normalsFragment;
out vec3 positionFragment;
out vec3 op1;
out vec3 op2;
out vec3 op3;

void main(){
	vec4 cameraView = modelview * vec4(positionVector, 1.0);
	positionFragment = cameraView.xyz;

	normalsFragment = mat3(modelview) * normalsVector;

	op1 = (modelview * vec4(v1, 1.0)).xyz;
	op2 = (modelview * vec4(v2, 1.0)).xyz;
	op3 = (modelview * vec4(v3, 1.0)).xyz;
	
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
in vec3 op1;
in vec3 op2;
in vec3 op3;
out vec4 fragmentColor;

void main(){
	vec3 diffuse[3];
	diffuse[0] = vec3(1.0, 0.5, 0.5);
	diffuse[1] = vec3(0.5, 1.0, 0.5);
	diffuse[2] = vec3(0.5, 0.5, 1.0);

	vec3 diffuseColor;
	if (distance(positionFragment,op1)<=distance(positionFragment, op2) && distance(positionFragment, op1)<=distance(positionFragment, op3)){
		diffuseColor = diffuse[0];
	}else if (distance(positionFragment, op2)<=distance(positionFragment, op3)){
		diffuseColor = diffuse[1];
	}else {
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
	return R"(
#version 330 core
in vec3 normalsFragment;
in vec3 positionFragment;
in vec3 op1;
in vec3 op2;
in vec3 op3;
out vec4 fragmentColor;

void main(){
    float e1 = distance(op2, op3); 
    float e2 = distance(op1, op3); 
    float e3 = distance(op1, op2); 

    vec3 center = (e1 * op1 + e2 * op2 + e3 * op3) / (e1 + e2 + e3);

    float s = (e1 + e2 + e3) / 2.0;
    float area = length(cross(op2 - op1, op3 - op1)) / 2.0;
    float radius = area / s;

    float distToCenter = distance(positionFragment, center);

    vec3 diffuseColor;
    vec3 ambientColor;
    vec3 specularColor;

    if (distToCenter < radius) {
        diffuseColor  = vec3(0.5, 0.5, 1.0);
        ambientColor  = vec3(0.05, 0.05, 0.1);
        specularColor = vec3(0.0, 0.0, 0.0);
    } else {
        diffuseColor  = vec3(1.0, 0.5, 0.5);
        ambientColor  = vec3(0.1, 0.05, 0.05);
        specularColor = vec3(0.3, 0.3, 0.3);
    }

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
    vec3 specularVector = spec * specularColor * light;

    fragmentColor = vec4(ambientColor + diffuseVector + specularVector, 1.0);
}
)";
}
