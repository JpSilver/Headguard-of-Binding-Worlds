#include <vector>
#include <glm/glm.hpp>
#include <fstream>
#include <string>
#include <iostream>

#ifndef MODELLOADER_H_INCLUDED
#define MODELLOADER_H_INCLUDED


using std::vector;
using std::ifstream;
using std::fstream;
using glm::vec2;
using glm::vec3;
using std::string;
using std::cout;
using std::endl;
using std::getline;

class ModelLoader {
public:
	bool endian = false;
	struct dsModel {
		vector< unsigned int > vertexIndices, uvIndices, normalIndices;
		vector<string> names;
		vector< vector<vec3> > verts;
		vector< vector<vec2> > textCoords;
	};

	dsModel load3dsModel(const char *filename);
	dsModel get3DSModel();

private:
	dsModel model3DS;
	short charToShort(char *c) {
		unsigned char *p = reinterpret_cast<unsigned char*>(c);
		short ret = 0;
		if (endian) {
			ret = ((p[1] << 8) | (p[0]));
		}
		else {
			ret = ((p[0] << 8) | (p[1]));
		}
		return ret;
	};


	int charToInt(char* c) {
		unsigned char *p = reinterpret_cast<unsigned char*>(c);
		int ret = 0;
		if (endian) {
			ret = ((p[3] << 24) | (p[2] << 16) | (p[1] << 8) | (p[0]));
		}
		else {
			ret = ((p[0] << 24) | (p[1] << 16) | (p[2] << 8) | (p[3]));
		}
		return ret;
	};

	float charToFloat(char *c) {
		unsigned char *p = reinterpret_cast<unsigned char*>(c);
		float f;
		if (endian) {
			((char*)&f)[0] = c[0];
			((char*)&f)[1] = c[1];
			((char*)&f)[2] = c[2];
			((char*)&f)[3] = c[3];
		}
		else {
			((char*)&f)[3] = c[0];
			((char*)&f)[2] = c[1];
			((char*)&f)[1] = c[2];
			((char*)&f)[0] = c[3];
		}
		return f;
	};

	double charToDouble(char *c) {
		unsigned char *p = reinterpret_cast<unsigned char*>(c);
		double d;
		if (endian) {
			((char*)&d)[0] = c[0];
			((char*)&d)[1] = c[1];
			((char*)&d)[2] = c[2];
			((char*)&d)[3] = c[3];
			((char*)&d)[4] = c[4];
			((char*)&d)[5] = c[5];
			((char*)&d)[6] = c[6];
			((char*)&d)[7] = c[7];
		}
		else {
			((char*)&d)[7] = c[0];
			((char*)&d)[6] = c[1];
			((char*)&d)[5] = c[2];
			((char*)&d)[4] = c[3];
			((char*)&d)[3] = c[4];
			((char*)&d)[2] = c[5];
			((char*)&d)[1] = c[6];
			((char*)&d)[0] = c[7];
		}
		return d;
	};

	unsigned short charToUShort(char *c) {
		unsigned char *p = reinterpret_cast<unsigned char*>(c);
		unsigned short ret = 0;
		if (endian) {
			ret = ((p[1] << 8) | (p[0]));
		}
		else {
			ret = ((p[0] << 8) | (p[1]));
		}
		return ret;
	};


	unsigned int charToUInt(char* c) {
		unsigned char *p = reinterpret_cast<unsigned char*>(c);
		unsigned int ret = 0;
		if (endian) {
			ret = ((p[3] << 24) | (p[2] << 16) | (p[1] << 8) | (p[0]));
		}
		else {
			ret = ((p[0] << 24) | (p[1] << 16) | (p[2] << 8) | (p[3]));
		}
		return ret;
	};

};


#endif // MODELLOADER_H_INCLUDED
