#include "Ray.h"
#include <iostream>
#include <limits>

Eigen::Vector3f Ray::intersectSphere(Geometry sphere)
{
	
	auto a = direction.dot(direction);
	auto b = ((-2) * direction).dot(sphere.center - origine);
	auto c = (sphere.center - origine).dot(sphere.center - origine) - sphere.radius * sphere.radius;

	auto discriminant = b * b - 4 * a * c;
	
	Eigen::Vector3f point;
	if (discriminant > 0) {
		auto t1 = (-b + std::sqrt(discriminant)) / (2 * a);
		auto t2 = (-b - std::sqrt(discriminant)) / (2 * a);
		
		if (t1 >= 0 && t2 >= 0) { //both points in front of camera
			if (t1 > t2) {
				point = origine + t2 * direction;
			}
			else {
				point = origine + t1 * direction;
			}
			return point;
		}
		else if (t1 >= 0 && t2 < 0) { //t1 in front of camera
			point = origine + t1 * direction;
			return point;
		}
		else if (t1 < 0 && t2 >= 0) { //t2 in front of camera
			point = origine + t2 * direction;
			return point;
		}
		else { //both behind camera
			float nan = std::numeric_limits<float>::quiet_NaN();
			point = Eigen::Vector3f(nan, nan, nan);
			return point;
		}
	}
	else if(discriminant == 0){
		point = origine + direction * (-b + std::sqrt(discriminant)) / (2 * a);
		return point;
	}
	else {
		
		float nan = std::numeric_limits<float>::quiet_NaN();
		point = Eigen::Vector3f(nan, nan, nan);
		return point;
	}
}

Eigen::Vector3f Ray::intersectTriangle(Geometry triangle)
{
	Eigen::Vector3f point;
	float nan = std::numeric_limits<float>::quiet_NaN();

	const double NEAR_ZERO = 0.0000001;

	Eigen::Vector3f edge1 = triangle.p2 - triangle.p1;
	Eigen::Vector3f edge2 = triangle.p3 - triangle.p1;

	Eigen::Vector3f directionCrossEdge2 = direction.cross(edge2);

	double determinant = edge1.dot(directionCrossEdge2);

	//check if ray is parallel
	if (determinant > -NEAR_ZERO && determinant < NEAR_ZERO) {
		//std::cout << "parallel" << std::endl;
		return Eigen::Vector3f(nan, nan, nan);
	}

	double invertedDeterminant = 1.0 / determinant;

	Eigen::Vector3f p1ToOrigine = origine - triangle.p1;
	
	double u = invertedDeterminant * p1ToOrigine.dot(directionCrossEdge2);

	Eigen::Vector3f p1ToOrigineCrossEdge1 = p1ToOrigine.cross(edge1);

	double v = invertedDeterminant * direction.dot(p1ToOrigineCrossEdge1);

	//check if ray intersects triangle
	if (u < 0 || v < 0 || u + v > 1) {
		//std::cout << "outside" << std::endl;
		return Eigen::Vector3f(nan, nan, nan);
	}

	double t = invertedDeterminant * edge2.dot(p1ToOrigineCrossEdge1);

	if (t > NEAR_ZERO) {// in front of camera
		//std::cout << "HIT" << std::endl;
		point = origine + t * direction;
		return point;
	}
	else {
		//std::cout << "behind" << std::endl;
		return Eigen::Vector3f(nan, nan, nan);
	}
}

Ray::Ray(Eigen::Vector3f origine, Eigen::Vector3f pixelCenter)
{
	Ray::origine = origine;
	Ray::direction = (pixelCenter - origine).normalized();
}

Eigen::Vector3f Ray::intersect(Geometry geometry)
{
	if (geometry.type == "sphere") {
		return intersectSphere(geometry);
	}
	else {
		return intersectTriangle(geometry);
	}
}
