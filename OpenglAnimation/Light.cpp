#include "Light.h"

Light::Light(vec3 position, vec3 rotation, vec3 color, char type, float range, string name)
{
	this->position = position;
	this->rotation = rotation;
	this->color = color;
	this->type = type;
	this->range = range;
	this->name = name;
};

vec3 Light::getPosition()
{
	return this->position;
}
vec3 Light::getRotation()
{
	return vec3();
}
vec3 Light::getColor()
{
	return this->color;
}
char Light::getType()
{
	return this->type;
}
float Light::getRange()
{
	return this->range;
}
string Light::getName()
{
	return this->name;
};
