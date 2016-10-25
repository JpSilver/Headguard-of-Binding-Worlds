#include "LightHandler.h"

void LightHandler::addLight(Light light)
{
	lights.push_back(light);
};

Light LightHandler::getLight(string name)
{
	for (int i = 0; i < lights.size(); i++) {
		if (lights[i].getName() == name) {
			return lights[i];
		}
	}
	return Light(vec3(0), vec3(0), vec3(1), 0, 0, "NULL");
}
vector<Light> LightHandler::getLights()
{
	return lights;
};
