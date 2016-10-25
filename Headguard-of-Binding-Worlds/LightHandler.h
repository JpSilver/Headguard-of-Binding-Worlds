#ifndef LIGHTHANDLER_H
#define LIGHTHANDLER_H

#include "Light.h"
#include <vector>

using std::vector;

class LightHandler {
public:
	void addLight(Light light);
	Light getLight(string);
	vector<Light> getLights();
private:
	vector<Light> lights;
};

#endif //LIGHTHANDLER_H