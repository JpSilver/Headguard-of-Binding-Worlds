#include "Joint.h"

Joint::Joint()
{
	boneMatrix = mat3x4(1);
	pos = vec3(0.0F);
	rot = vec4(0.0F, 0.0F, 0.0F, 1.0F);
	name = "Joint:" + (char)(ID + 48);
	ID++;
};

Joint::Joint(mat3x4 matrix, vec3 pos, vec4 rot, int parentIndex, string name)
{
	this->boneMatrix = matrix;
	this->pos = pos;
	this->rot = rot;
	this->name = name;
}
string Joint::getName()
{
	return this->name;
}
;

mat3x4 * Joint::getBoneMatrix()
{
	return &this->boneMatrix;
};

vec3 * Joint::getPos()
{
	return &this->pos;
};

vec4 * Joint::getRotation()
{
	return &this->rot;
};
