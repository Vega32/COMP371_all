#define Ray_H
#include <Eigen/Core>
#include <Eigen/Dense>
#include "Scene.h"

using std::string;

class Ray {
private:
	Eigen::Vector3f intersectSphere(Geometry sphere);
	Eigen::Vector3f intersectTriangle(Geometry triangle);
	Eigen::Vector3f origine{ 0, 0, 0 };
	Eigen::Vector3f direction{ 0, 0, 0 };
public:
	Ray(Eigen::Vector3f origine, Eigen::Vector3f pixelCenter);
	Eigen::Vector3f intersect(Geometry geometry);
};