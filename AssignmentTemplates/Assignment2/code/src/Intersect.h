#ifndef INTERSECT_H
#define INTERSECT_H
#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

struct intersection {
	bool hit = false;
	glm::vec3 bery;
	float Z;
	int index;
	glm::vec3 point3D;
};

intersection checkIntersect(float xpos, float ypos, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3);

#endif