#include "RayTracer.h"
#include "Scene.h"
#include "Ray.h"
#include "../external/simpleppm.h"
#include <iostream>

using namespace std;

RayTracer::RayTracer(nlohmann::json j)
{
	RayTracer::jsonFile = j;
}

void RayTracer::run()
{
	cout << "TESTING RUN FUNCTION" << endl;
	Scene *scene = new Scene(RayTracer::jsonFile);
	for (auto output = scene->vOutput.begin(); output != scene->vOutput.end(); output++) {

		
		double height = output->size[1];
		double width = output->size[0];
		vector<double> buffer(3 * width * height);

		double delta = (2 * tan(output->fov / 2)) / height;
		Eigen::Vector3f r = output->lookat.cross(output->up);

		Eigen::Vector3f A = output->center + output->lookat;
		Eigen::Vector3f B = A + tan(output->fov / 2) * output->up;
		Eigen::Vector3f C = B - (width / 2) * delta * r;

		for (int j = 0; j < height; j++) {
			for (int i = 0; i < width; i++) {
				Eigen::Vector3f pixelCenter = C + (i * delta + delta / 2) * r - (j * delta + delta / 2) * output->up;
				Ray ray (output->center, pixelCenter);
				vector<Eigen::Vector3f> intersections;
				vector<Geometry> intersectionsShapes;
				int minIndex = 0;

				for (auto shape = scene->vGeometry.begin(); shape != scene->vGeometry.end(); shape++) {
					
					Eigen::Vector3f point = ray.intersect(*shape);

					if (!std::isnan(point[0])) {

						intersections.push_back(point);
						intersectionsShapes.push_back(*shape);
						
					}
					
				}

				if (intersections.size() == 0) {
					buffer[3 * j * width + 3 * i + 0] = (output->bkc[0]);
					buffer[3 * j * width + 3 * i + 1] = (output->bkc[1]);
					buffer[3 * j * width + 3 * i + 2] = (output->bkc[2]);
				}
				else {


					for (int si = 0; si < intersections.size(); si++) {
						double currentMin = (output->center - intersections.at(minIndex)).norm();
						double newMin = (output->center - intersections.at(si)).norm();
						if (newMin < currentMin) {
							minIndex = si;
						}
					}
					buffer[3 * j * width + 3 * i + 0] = (intersectionsShapes.at(minIndex).ac[0]);
					buffer[3 * j * width + 3 * i + 1] = (intersectionsShapes.at(minIndex).ac[1]);
					buffer[3 * j * width + 3 * i + 2] = (intersectionsShapes.at(minIndex).ac[2]);
				}
				
			}
		}
		save_ppm(output->filename, buffer, width, height);
	}
}
