#pragma once
#include <glm\glm.hpp>
#include <string>

using std::string;

#define LIGHTPOINT 0
#define LIGHTLAMP 1
#define LIGHTSUN 2

using glm::vec3;

class Light {
public:
	Light(vec3, vec3, vec3, char, float, string);
	vec3 getPosition();
	vec3 getRotation();
	vec3 getColor();
	char getType();
	float getRange();
	string getName();
private:
	vec3 position;
	vec3 rotation;
	vec3 color;
	char type;
	float range;
	string name;
	
};