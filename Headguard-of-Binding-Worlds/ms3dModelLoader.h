#ifndef MS3DMODELLOADER_H
#define MS3DMODELLOADER_H

#include <iostream>
#include <fstream>
#include <string>
#include <glm/gtx/matrix_decompose.hpp>
#include "Ms3dModel.h"
#include "MathUtils.h"
#include "Utils.h"
#include "TextureHandler.h"
#include "Armature.h"

using std::cout;
using std::endl;
using std::ifstream;
using std::string;
using namespace MS3D;
using glm::vec3;
using glm::vec4;

using namespace converter;
using namespace Math;

class ms3dModelLoader{
public:
	ms3dModelLoader();
	Ms3dModel loadMs3dModel(string, TextureHandler*, bool);
	Armature loadMS3dAnimation(string filename);
private:
	TextureHandler *textureHandler;
	int getJointByName(char[32], int, ms3d_joint*);
	int getJointByName(string name, vector<Joint> &joints);
	void loadVertices(ifstream&, ms3d_vertex*, word);
	void loadTriangles(ifstream&, ms3d_triangle*, word);
	void loadGroups(ifstream&, ms3d_group*, word);
	void loadMaterials(ifstream&, ms3d_material*, word);
	void loadJoints(ifstream&, ms3d_joint*, word, float);
	void createObject(int, int, int, ms3d_group*, ms3d_triangle*, ms3d_vertex*, Ms3dModel&);
	void buildArmature(int, int, float, ms3d_joint*, Ms3dModel&);
};

#endif //MS3DMODELLOADER_H