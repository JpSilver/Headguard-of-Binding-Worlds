#ifndef MS3DMODEL_H
#define MS3DMODEL_H

#include <glm\vec3.hpp>
#include <glm/glm.hpp>
#include <string>
#include "Texture.h"
#include "Armature.h"

using std::string;
using glm::vec4;
using glm::vec3;
using glm::vec2;

struct Ms3dModel {
	string name;
	vector<vec4> verteces;
	vector<vec2> uvCoords;
	vector<vec3> normals;
	int textureID;
	Armature *armature;
};
#endif //MS3DMODEL_H