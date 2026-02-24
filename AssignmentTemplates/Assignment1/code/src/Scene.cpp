#include "Scene.h"
#include <fstream>
#include <iostream>
#include <cstdlib>

using namespace std;

Scene::Scene(string s) {
	processFile(s);
	getNormals();
	cerr << "First 3 normals:" << endl;
	for (int i = 0; i < 3; i++) {
		// each vertex is 6 floats, normal starts at offset 3
		int base = i * 6;
		cerr << flatData[base + 3] << " " << flatData[base + 4] << " " << flatData[base + 5] << endl;
	}
	cerr << "ModelView:" << endl;
	for (int i = 0; i < 16; i++) {
		cerr << modelViewMatrix[i] << " ";
		if ((i + 1) % 4 == 0) cerr << endl;
	}
	//testFileProcessing();
}


void Scene::processFile(string s)
{
	fstream file(s);
	if (!file.is_open()) {
		std::cerr << "Unable to open file\n";
		exit(1);
	}
	//reading modelview matrix
		
	for (int i = 0; i < 16; i++) {
		file >> modelViewMatrix[i];
	}

	//reading projection matrix
		
	for (int i = 0; i < 16; i++) {
		file >> projectionMatrix[i];
	}

	//reading dimentions
		
	file >> dimensions[0];
	file >> dimensions[1];

	//reading numVertecies
		
	file >> numVertiecies;

	//reading vertecies

	vertecies.resize(numVertiecies * 3);
	for (int i=0;i<numVertiecies*3;i++){
		file>>vertecies[i];
	}

	//reading numTriangles
		
		
	file >> numTriangles;
		

	//reading triangles

	triangles.resize(numTriangles * 3);
	for (int i = 0; i < numTriangles * 3; i++) {
		file >> triangles[i];
	}

}

void Scene::testFileProcessing() {

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			cout << modelViewMatrix[i * 4 + j] << " | ";
		}
		cout << endl;
	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			cout << projectionMatrix[i * 4 + j] << " | ";
		}
		cout << endl;
	}

	cout << dimensions[0] << " " << dimensions[1] << endl;

	cout << "numVertecies: " << numVertiecies << " | numTriangles: " << numTriangles << endl;

	cout << vertecies.size() << endl;
	cout << triangles.size() << endl;

}

glm::vec3 Scene::getVertexByIndex(int index)
{
	return glm::vec3(
		vertecies[index * 3],
		vertecies[index * 3 + 1],
		vertecies[index * 3 + 2]
	);
}

void Scene::getNormals()
{
	vector<glm::vec3> triangleNormals(numTriangles);

	for (int i = 0; i < numTriangles; i++) {
		unsigned int i1 = triangles[i * 3];
		unsigned int i2 = triangles[i * 3 + 1];
		unsigned int i3 = triangles[i * 3 + 2];


		glm::vec3 v1 = getVertexByIndex(i1);
		glm::vec3 v2 = getVertexByIndex(i2);
		glm::vec3 v3 = getVertexByIndex(i3);

		glm::vec3 e1 = v2 - v1;
		glm::vec3 e2 = v3 - v1;
		triangleNormals[i] = glm::normalize(glm::cross(e1, e2));
	}

	vector<glm::vec3> vertexNormals(numVertiecies, glm::vec3(0.0f));

	for (int i = 0; i < numTriangles; i++) {

		unsigned int i1 = triangles[i * 3];
		unsigned int i2 = triangles[i * 3 + 1];
		unsigned int i3 = triangles[i * 3 + 2];
		glm::vec3 v1 = getVertexByIndex(i1);
		glm::vec3 v2 = getVertexByIndex(i2);
		glm::vec3 v3 = getVertexByIndex(i3);

		vertexNormals[i1] += triangleNormals[i];
		vertexNormals[i2] += triangleNormals[i];
		vertexNormals[i3] += triangleNormals[i];

	}

	for (int i = 0; i < numVertiecies; i++) {
		vertexNormals[i] = glm::normalize(vertexNormals[i]);
	}

	for (int i = 0; i < numVertiecies; i++) {
		phongData.push_back(vertecies[i * 3]);
		phongData.push_back(vertecies[i * 3 + 1]);
		phongData.push_back(vertecies[i * 3 + 2]);
		phongData.push_back(vertexNormals[i].x);
		phongData.push_back(vertexNormals[i].y);
		phongData.push_back(vertexNormals[i].z);
	}

	for (int i = 0; i < numTriangles; i++) {
		unsigned int i1 = triangles[i * 3];
		unsigned int i2 = triangles[i * 3 + 1];
		unsigned int i3 = triangles[i * 3 + 2];

		glm::vec3 v1 = getVertexByIndex(i1);
		glm::vec3 v2 = getVertexByIndex(i2);
		glm::vec3 v3 = getVertexByIndex(i3);
		glm::vec3 normal = triangleNormals[i];

		//Flat Shader
		flatData.push_back(v1.x);
		flatData.push_back(v1.y);
		flatData.push_back(v1.z);
		flatData.push_back(normal.x);
		flatData.push_back(normal.y);
		flatData.push_back(normal.z);

		flatData.push_back(v2.x);
		flatData.push_back(v2.y);
		flatData.push_back(v2.z);
		flatData.push_back(normal.x);
		flatData.push_back(normal.y);
		flatData.push_back(normal.z);

		flatData.push_back(v3.x);
		flatData.push_back(v3.y);
		flatData.push_back(v3.z);
		flatData.push_back(normal.x);
		flatData.push_back(normal.y);
		flatData.push_back(normal.z);

		flatIndices.push_back(i * 3);
		flatIndices.push_back(i * 3 + 1);
		flatIndices.push_back(i * 3 + 2);

		//Bery shaders
		beryData.push_back(v1.x);
		beryData.push_back(v1.y);
		beryData.push_back(v1.z);
		beryData.push_back(normal.x);
		beryData.push_back(normal.y);
		beryData.push_back(normal.z);
		beryData.push_back(1.0f);
		beryData.push_back(0.0f);
		beryData.push_back(0.0f);

		beryData.push_back(v2.x);
		beryData.push_back(v2.y);
		beryData.push_back(v2.z);
		beryData.push_back(normal.x);
		beryData.push_back(normal.y);
		beryData.push_back(normal.z);
		beryData.push_back(0.0f);
		beryData.push_back(1.0f);
		beryData.push_back(0.0f);

		beryData.push_back(v3.x);
		beryData.push_back(v3.y);
		beryData.push_back(v3.z);
		beryData.push_back(normal.x);
		beryData.push_back(normal.y);
		beryData.push_back(normal.z);
		beryData.push_back(0.0f);
		beryData.push_back(0.0f);
		beryData.push_back(1.0f);

		beryIndices.push_back(i * 3);
		beryIndices.push_back(i * 3 + 1);
		beryIndices.push_back(i * 3 + 2);
	}
}

void Scene::createBuffers()
{
	// Flat Buffer setup
	glGenVertexArrays(1, &VAOf);
	glGenBuffers(1, &VBOf);
	glGenBuffers(1, &EBOf);

	glBindVertexArray(VAOf);

	glBindBuffer(GL_ARRAY_BUFFER, VBOf);
	glBufferData(GL_ARRAY_BUFFER, flatData.size() * sizeof(float), flatData.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOf);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, flatIndices.size() * sizeof(unsigned int), flatIndices.data(), GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Phong Buffer setup
	glGenVertexArrays(1, &VAOp);
	glGenBuffers(1, &VBOp);
	glGenBuffers(1, &EBOp);

	glBindVertexArray(VAOp);

	glBindBuffer(GL_ARRAY_BUFFER, VBOp);
	glBufferData(GL_ARRAY_BUFFER, phongData.size() * sizeof(float), phongData.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOp);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangles.size() * sizeof(unsigned int), triangles.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Berycentric Buffer
	glGenVertexArrays(1, &VAOb);
	glGenBuffers(1, &VBOb);
	glGenBuffers(1, &EBOb);

	glBindVertexArray(VAOb);

	glBindBuffer(GL_ARRAY_BUFFER, VBOb);
	glBufferData(GL_ARRAY_BUFFER, beryData.size() * sizeof(float), beryData.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOb);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, beryIndices.size() * sizeof(unsigned int), beryData.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}

int Scene::compileAndLinkShaders(const char* vertexShader, const char* fragmentShader)
{
	int Vshader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(Vshader, 1, &vertexShader, NULL);
	glCompileShader(Vshader);

	int success;
	char log[512];
	glGetShaderiv(Vshader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(Vshader, 512, NULL, log);
		cerr << "VERTEX SHADER ERROR:\n" << log << endl;
	}

	int Fshader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(Fshader, 1, &fragmentShader, NULL);
	glCompileShader(Fshader);

	glGetShaderiv(Fshader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(Fshader, 512, NULL, log);
		cerr << "FRAGMENT SHADER ERROR:\n" << log << endl;
	}

	int program = glCreateProgram();
	glAttachShader(program, Vshader);
	glAttachShader(program, Fshader);
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, NULL, log);
		cerr << "PROGRAM LINK ERROR:\n" << log << endl;
	}

	return program;

}
