#include "ModelLoader.h"


bool isEndian();

bool isEndian() {
	volatile uint16_t i = 1;
	return  ((uint8_t*)&i)[0] == 0x01;//0x01=little, 0x00=big
}

void readData(ifstream &in, char *data, int len);

void readData(ifstream &in, char *data, int len) {
	in.read(data, len);
}

ModelLoader::dsModel ModelLoader::get3DSModel() {
	return this->model3DS;
};

vector<string> seperate(string str, char sepChar) {
	vector<string> ret;
	string tmpStr;
	while (str.length() > 0) {
		int i = str.find_first_of(sepChar, 0);
		if (i == -1) {
			i = str.length();
		}
		ret.push_back(str.substr(0, i));
		str.erase(0, i + 1);
	};
	return ret;
};

ModelLoader::dsModel ModelLoader::load3dsModel(const char *filename) {
	endian = isEndian();
	ifstream file;
	file.open(filename, std::ios::binary | std::ios::in);
	vector<vec3> tmp_verts = vector<vec3>();
	vector<vec3> tmp_modelVerts = vector<vec3>();
	vector<vec2> tmp_coords = vector<vec2>();
	vector<vec2> tmp_modelCoords = vector<vec2>();
	char *SHORT = new char[2];
	char *INT = new char[4];
	char *FLOAT = new char[4];
	char *DOUBLE = new char[8];
	bool loadedObject = false;
	int objectID = -1;
	string objName;

	while (!file.eof()) {
		char *data;
		file.read(SHORT, 2);
		file.read(INT, 4);

		short id = charToShort(SHORT);
		int chunkLen = charToInt(INT);

		switch (id) {
		case 0x4D4D: {
			cout << "Started loading scene" << endl;
		}
					 break;
		case 0x002: {
			file.read(INT, 4);
			int version = charToShort(INT);
			cout << version << endl;
		}
					break;
		case 0x3D3D: {
		}
					 break;
		case 0x4000: {
			if (loadedObject) {
				for (int i = 0; i < model3DS.vertexIndices.size(); i++) {
					short s = model3DS.vertexIndices[i];
					tmp_modelVerts.push_back(tmp_verts[s]);
					tmp_modelCoords.push_back(tmp_coords[s]);
				}
				model3DS.vertexIndices.clear();
				tmp_verts.clear();
				tmp_coords.clear();
				model3DS.names.push_back(objName);
				model3DS.verts.push_back(tmp_modelVerts);
				model3DS.textCoords.push_back(tmp_modelCoords);
				tmp_modelVerts.clear();
				tmp_modelCoords.clear();
				loadedObject = false;
				objName = "";
			}
			loadedObject = true;
			int i = 0;
			char *c = new char[1];
			char *name = new char[20];
			do {
				file.read(c, 1);
				name[i] = c[0];
				i++;
			} while (c[0] != '\0' && i < 20);
			objName = string(name);
			delete c;
			delete[] name;
			cout << "Loaded : " << objName << endl;
		}
					 break;
		case 0x4100: {
		}
					 break;
		case 0x4110: {
			file.read(SHORT, 2);
			short quantity = charToShort(SHORT);
			for (int i = 0; i < quantity; i++) {
				file.read(FLOAT, 4);
				float x = charToFloat(FLOAT);
				file.read(FLOAT, 4);
				float y = charToFloat(FLOAT);
				file.read(FLOAT, 4);
				float z = charToFloat(FLOAT);
				tmp_verts.push_back(vec3(x, y, z));
			}

		}
					 break;
		case 0x4120: {
			file.read(SHORT, 2);
			short quantity = charToShort(SHORT);
			for (int i = 0; i < quantity; i++) {
				file.read(SHORT, 2);
				int a = charToShort(SHORT);
				file.read(SHORT, 2);
				int b = charToShort(SHORT);
				file.read(SHORT, 2);
				int c = charToShort(SHORT);
				file.read(SHORT, 2);
				int d = charToShort(SHORT);
				model3DS.vertexIndices.push_back(a);
				model3DS.vertexIndices.push_back(b);
				model3DS.vertexIndices.push_back(c);
			}
		}
					 break;
		case 0x4140: {
			file.read(SHORT, 2);
			short quantity = charToShort(SHORT);
			for (int i = 0; i < quantity; i++) {
				file.read(FLOAT, 4);
				float u = charToFloat(FLOAT);
				file.read(FLOAT, 4);
				float v = charToFloat(FLOAT);
				tmp_coords.push_back(vec2(u, v));
			}
		}
					 break;
		default: {
			if (id == 0x4600) {
				cout << "LIGHT!!==========================" << endl;
			}
			char *p = new char[chunkLen - 6];
			file.read(p, chunkLen - 6);
			delete[] p;
		}
		}

	}

	for (int i = 0; i < model3DS.vertexIndices.size(); i++) {
		short s = model3DS.vertexIndices[i];
		tmp_modelVerts.push_back(tmp_verts[s]);
		tmp_modelCoords.push_back(tmp_coords[s]);
	}
	model3DS.vertexIndices.clear();
	tmp_verts.clear();
	tmp_coords.clear();
	model3DS.names.push_back(objName);
	model3DS.verts.push_back(tmp_modelVerts);
	model3DS.textCoords.push_back(tmp_modelCoords);
	tmp_modelVerts.clear();
	tmp_modelCoords.clear();
	cout << "DONE!" << endl;
	delete[] SHORT;
	delete[] INT;
	delete[] FLOAT;
	delete[] DOUBLE;
	return model3DS;
}
