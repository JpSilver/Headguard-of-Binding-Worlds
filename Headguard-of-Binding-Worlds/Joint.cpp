#include "Joint.h"

unsigned int Joint::ID = 0;

Joint::Joint()
{
	boneMatrix = mat3x4(1);
	pos = vec3(0.0F);
	rot = vec4(0.0F, 0.0F, 0.0F, 1.0F);
	name = "Joint:" + (char)(ID + 48);
	m_ID = 0;
	ID++;
};

Joint::Joint(mat3x4 matrix, vec3 pos, vec4 rot,int jointIndex, int parentIndex, string name)
{
	this->boneMatrix = matrix;
	this->pos = pos;
	this->rot = rot;
	this->name = name;
	this->m_ID = jointIndex;
	this->parentIndex = parentIndex;
}
int Joint::getJointID()
{
	return this->m_ID;
}
int Joint::getParentID()
{
	return this->parentIndex;
};

string Joint::getName()
{
	return this->name;
}
void Joint::setPosition(vec3 position)
{
	this->pos = position;
}
void Joint::setRotation(vec4 rotation)
{
	this->rot = rotation;
};

mat3x4 &Joint::getBoneMatrix()
{
	return this->boneMatrix;
};

vec3 &Joint::getPos()
{
	return this->pos;
};

vec4 &Joint::getRotation()
{
	return this->rot;
};
