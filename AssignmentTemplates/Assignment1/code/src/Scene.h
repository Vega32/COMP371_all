#ifndef SCENE_H
#define SCENE_H
#include <string>
#include <vector>
class Scene {
public:
	double modelViewMatrix[16] = {};
	double projectionMatrix[16] = {};
	double dimensions[2] = {};
	int numVertiecies = 0;
	std::vector<double> vertecies = {};
	int numTriangles = 0;
	std::vector<int> triangles = {};

	
	Scene(std::string s);
	void processFile(std::string s);
	void testFileProcessing();

};

#endif