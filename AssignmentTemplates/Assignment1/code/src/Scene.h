#ifndef SCENE_H
#define SCENE_H
#include <string>
#include <vector>
#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
class Scene {
public:
	float modelViewMatrix[16] = {};
	float projectionMatrix[16] = {};
	int dimensions[2] = {};
	int numVertiecies = 0;
	std::vector<float> vertecies = {};
	int numTriangles = 0;
	std::vector<unsigned int> triangles = {};

	unsigned int VAOf, VBOf, EBOf, VAOp, VBOp, EBOp, VAOb, VBOb, EBOb;

	std::vector<float> phongData;
	std::vector<float> flatData;
	std::vector<unsigned int> flatIndices;
	std::vector<float> beryData;
	std::vector<int> beryIndices;

	
	Scene(std::string s);
	void processFile(std::string s);
	void testFileProcessing();
	glm::vec3 getVertexByIndex(int index);
	void getNormals();
	void createBuffers();
	int compileAndLinkShaders(const char* vertexShader, const char* fragmentShader);
};

#endif