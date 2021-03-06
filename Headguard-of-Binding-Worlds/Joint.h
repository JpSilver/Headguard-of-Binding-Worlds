#ifndef JOINT_H
#define JOINT_H

#include <string>
#include <glm\vec4.hpp>
#include <glm/glm.hpp>
#include <glm\vec3.hpp>
#include "DataTypes.h"


using std::string;
using glm::mat3x4;
using glm::vec3;
using glm::vec4;

class Joint{
public:
	Joint();
	Joint(mat3x4, vec3, vec4, int, int, string);
	int getJointID();
	int getParentID();
	string getName();
	void setPosition(vec3);
	void setRotation(vec4);
	mat3x4 &getBoneMatrix();
	vec3 &getPos();
	vec4 &getRotation();
	mat3x4 localMatrix;
	mat3x4 globalMatrix;
	mat3x4 pGlobal;

private:
	mat3x4 boneMatrix;
	vec3 pos;
	vec4 rot;
	string name;
	int m_ID = 0;
	int parentIndex;
	static unsigned int ID;
};
#endif //JOINT_H