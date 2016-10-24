#ifndef UTILS_H
#define UTILS_H
#include <GL/glew.h>
#include <GL/GLU.h>
#include <GL/GL.h>
#include <glm\vec3.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm\gtx\euler_angles.hpp>

#include <string>
#include <fstream>
#include <vector>
#include "Armature.h"
#include "Texture.h"


using std::string;
using std::ifstream;
using std::vector;
using glm::vec3;
using glm::vec4;
using glm::mat4;
using glm::vec2;

namespace converter{
	inline word bytesToWord(char bytes[2]){
		word ret;
		memcpy(&ret, &bytes[0], sizeof(ret));
		return ret;
	};
	
	inline int bytesToInt(char bytes[4]){
		word ret = 0;
		ret = (ret << 8) + bytes[3];
		ret = (ret << 8) + bytes[2];
		ret = (ret << 8) + bytes[1];
		ret = (ret << 8) + bytes[0];
		return ret;
	};

	inline float bytesToFloat(char bytes[4]){
		float ret;
		memcpy(&ret, &bytes[0], sizeof(ret));
		return ret;
	};
};

namespace MS3D{
	struct ms3d_header{
		char ID[11];
		int version;
	};

	struct ms3d_vertex{
		byte flag;
		vec3 vertex;
		char boneID;
		byte referenceCount;
	};

	struct ms3d_triangle{
		word flag;
		word vertexIndex[3];
		vec3 vertexNormals[3];
		vec3 s;
		vec3 t;
		byte smoothGroup;
		byte groupIndex;
	};

	struct ms3d_group{
		byte flag;
		char name[32];
		word numTriangles;
		word *triangleIndices;
		char materialIndex;
	};

	struct ms3d_material{
		char name[32];
		vec4 ambiant;
		vec4 diffuse;
		vec4 specular;
		vec4 emissive;
		float shininess;
		float transparency;
		char mode;
		char texture[128];
		char alphamap[128];
	};

	struct ms3d_keyframeRot{
		float frame; //Time in seconds
		vec3 rotation; //x, y and z angles
	};

	struct ms3d_keyframeTrans{
		float frame;
		vec3 position; //Local position
	};

	struct ms3d_joint{
		byte flag;
		char name[32];
		char parentName[32];
		vec3 rotation;
		vec3 position;
		word numKeyframeRot;
		word numKeyframeTran;
		ms3d_keyframeRot *keyframeRot;
		ms3d_keyframeTrans *keyframeTran;
	};

	inline vec3 floatArrToVec3(float arr[3], string format){
		vec3 ret = vec3();
		for(int i = 0; i < 3; i ++){
			if(format[i] == 'x'){
				ret.x = arr[i];
			}
			if(format[i] == 'y'){
				ret.y = arr[i];
			}
			if(format[i] == 'z'){
				ret.z = arr[i];
			}
		}
		return ret;
	};

	inline vec4 floatArrToVec4(float arr[4], string format){
		vec4 ret = vec4();
		for(int i = 0; i < 3; i ++){
			if(format[i] == 'x'){
				ret.x = arr[i];
			}
			if(format[i] == 'y'){
				ret.y = arr[i];
			}
			if(format[i] == 'z'){
				ret.z = arr[i];
			}
			if(format[i] == 'w'){
				ret.w = arr[i];
			}
		}
		return ret;
	};
};
#endif //!UTILS_H