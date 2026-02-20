#include "Scene.h"
#include <fstream>
#include <iostream>
#include <cstdlib>

using namespace std;

Scene::Scene(string s) {
	processFile(s);
	testFileProcessing();
}


void Scene::processFile(string s)
{
	fstream file(s);
	if (!file.is_open()) {
		std::cerr << "Unable to open file\n";
		exit(1);
	}

	double number;
	int counter = 0;
	while (file >> number) {
		//reading modelview matrix
		
		if (counter < 16) {
			modelViewMatrix[counter] = number;
		}

		//reading projection matrix
		
		else if (counter >= 16 && counter <= 32) {
			projectionMatrix[counter - 16] = number;
		}

		//reading dimentions
		
		else if (counter == 32 || counter == 33) {
			dimensions[counter - 32] = number;
		}

		//reading numVertecies
		
		else if (counter == 34) {
			numVertiecies = number;
		}

		//reading vertecies
		
		else if (counter > 34 && counter < 35 + numVertiecies * 3) {
			vertecies.push_back(number);
		}

		//reading numTriangles
		
		else if (counter == 35 + numVertiecies * 3) {
			numTriangles = number;
		}

		//reading triangles

		else if (counter > 35 + numVertiecies * 3) {
			triangles.push_back(number);
		}

		counter++;
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
