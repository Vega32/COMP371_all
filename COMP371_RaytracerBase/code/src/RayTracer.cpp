#include "RayTracer.h"
#include "Scene.h"

using namespace std;

RayTracer::RayTracer(nlohmann::json j)
{
	RayTracer::jsonFile = j;
}

void RayTracer::run()
{
	cout << "TESTING RUN FUNCTION" << endl;
	Scene *scene = new Scene(RayTracer::jsonFile);
	cout<<scene->vGeometry.at(0).center<<endl;
}
