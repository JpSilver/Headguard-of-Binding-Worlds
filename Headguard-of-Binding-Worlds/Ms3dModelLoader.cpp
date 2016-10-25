#include "ms3dModelLoader.h"

ms3dModelLoader::ms3dModelLoader(){

};

string readString(ifstream &instream, int len) {
	char *strData = new char[len];
	instream.read(strData, len);
	string ret = string(strData);
	delete[] strData;
	return ret;
};

word readWord(ifstream &instream){
	char wordData[2];
	instream.read(wordData, 2);
	return bytesToWord(wordData);
};

int readInt(ifstream &instream){
	char intData[4];
	instream.read(intData, 4);
	return bytesToInt(intData);
};

float readFloat(ifstream &instream){
	char floatData[4];
	instream.read(floatData, 4);
	return bytesToFloat(floatData);
};

vec3 readVec3(ifstream &instream){
	float data[3];
	for(int i = 0; i < 3; i ++){
		data[i] = readFloat(instream);
	}
	return floatArrToVec3(data, "xyz");
};

vec4 readVec4(ifstream &instream){
	float data[4];
	for(int i = 0; i < 4; i ++){
		data[i] = readFloat(instream);
	}
	return floatArrToVec4(data, "xyzw");
};

void ms3dModelLoader::loadVertices(ifstream& ms3dFile, ms3d_vertex* vertices, word numVertices){
	char charData;
	for(int i = 0; i < numVertices; i ++){
		ms3d_vertex *curVertex = &vertices[i];
		ms3dFile.read(&charData, 1);
		curVertex->flag = charData;
		curVertex->vertex = readVec3(ms3dFile);
		ms3dFile.read(&charData, 1);
		curVertex->boneID = charData;
		ms3dFile.read(&charData, 1);
		curVertex->referenceCount = (byte)charData;

	};
};

void ms3dModelLoader::loadTriangles(ifstream& ms3dFile, ms3d_triangle* triangles, word numTriangles){
	char charData;
	for(int i = 0; i < numTriangles; i ++){
		ms3d_triangle *curTriangle = &triangles[i];
		//Read the flags
		curTriangle->flag = readWord(ms3dFile);
		//Read vertex indexes
		for(int k = 0; k < 3; k ++){
			curTriangle->vertexIndex[k] = readWord(ms3dFile);
		}
		//Read vertex normals
		for(int k = 0; k < 3; k ++){
			curTriangle->vertexNormals[k] = readVec3(ms3dFile);
		}
		//Read s texture coordinate
		curTriangle->s = readVec3(ms3dFile);
		//Read t texture coordinate
		curTriangle->t = readVec3(ms3dFile);
		//read smoothGroup
		ms3dFile.read(&charData, 1);
		curTriangle->smoothGroup = (byte)charData;
		//Read group index
		ms3dFile.read(&charData, 1);
		curTriangle->groupIndex = (byte)charData;
	};		
};

void ms3dModelLoader::loadGroups(ifstream& ms3dFile, ms3d_group* groups, word numGroups){
	char charData;
	for(int i = 0; i < numGroups; i ++){
		char flag;
		char name[32];
		ms3dFile.read(&flag, 1);
		ms3dFile.read(name, 32);
		groups[i] = ms3d_group();
		ms3d_group *curGroup = &groups[i];
		curGroup->flag = (byte)flag;
		memcpy(&curGroup->name[0], &name[0], 32);
		curGroup->numTriangles = readWord(ms3dFile);
		curGroup->triangleIndices = new word[curGroup->numTriangles];
		for(int k = 0; k < curGroup->numTriangles; k ++){
			curGroup->triangleIndices[k] = readWord(ms3dFile);
		}
		ms3dFile.read(&curGroup->materialIndex, 1);
	};
};

void ms3dModelLoader::loadMaterials(ifstream& ms3dFile, ms3d_material* materials, word numMaterials){
	char charData;
	for(int i = 0; i < numMaterials; i ++){
		ms3d_material *curMaterial = &materials[i];
		ms3dFile.read(curMaterial->name, 32);
		//Read ambiant data
		curMaterial->ambiant = readVec4(ms3dFile);
		//Read diffuse data
		curMaterial->diffuse = readVec4(ms3dFile);
		//Read specular data
		curMaterial->specular = readVec4(ms3dFile);
		//Read emissive data
		curMaterial->emissive = readVec4(ms3dFile);
		//Read shininess data
		curMaterial->shininess = readFloat(ms3dFile);
		//Read transparancy data
		curMaterial->transparency = readFloat(ms3dFile);
		//Read mode
		ms3dFile.read(&curMaterial->mode, 1);
		//Read texture name
		ms3dFile.read(curMaterial->texture, 128);
		//Read alphamap name
		ms3dFile.read(curMaterial->alphamap, 128);
	};
};

void ms3dModelLoader::loadJoints(ifstream& ms3dFile, ms3d_joint* joints, word numJoints, float animFps){
	char charData;
	for(int i = 0; i < numJoints; i ++){
		joints[i] = ms3d_joint();
		ms3d_joint *curJoint = &joints[i];
		ms3dFile.read(&charData, 1);
		curJoint->flag = (byte)charData;
		ms3dFile.read(curJoint->name, 32);
		ms3dFile.read(curJoint->parentName, 32);
		curJoint->rotation = readVec3(ms3dFile);
		curJoint->position = readVec3(ms3dFile);

		curJoint->numKeyframeRot = readWord(ms3dFile);
		curJoint->numKeyframeTran = readWord(ms3dFile);

		curJoint->keyframeRot = new ms3d_keyframeRot[curJoint->numKeyframeRot];
		curJoint->keyframeTran = new ms3d_keyframeTrans[curJoint->numKeyframeTran];

		for(int k = 0; k < curJoint->numKeyframeRot; k ++){
			curJoint->keyframeRot[k] = ms3d_keyframeRot();
			int time = animFps * readFloat(ms3dFile);
			curJoint->keyframeRot[k].frame = time;
			curJoint->keyframeRot[k].rotation = readVec3(ms3dFile);
		}
		for(int k = 0; k < curJoint->numKeyframeTran; k ++){
			curJoint->keyframeTran[k] = ms3d_keyframeTrans();
			int time = animFps * readFloat(ms3dFile);
			curJoint->keyframeTran[k].frame = time;
			curJoint->keyframeTran[k].position = readVec3(ms3dFile);
		}
	};
};

void ms3dModelLoader::createObject(int numTriangles, int numGroups, int numVertices, ms3d_group *groups, ms3d_triangle *triangles, ms3d_vertex *vertices, Ms3dModel &retModel){
	for(int i = 0; i < numGroups; i ++){
		retModel.name = groups[i].name;
			numTriangles =  groups[i].numTriangles;
			for (int k = 0; k < numTriangles; k++) {
				int triID = groups[i].triangleIndices[k];
				if (triID < numTriangles) {
					ms3d_triangle *curTriangle = &triangles[triID];
					float U[3] = { curTriangle->s.x, curTriangle->s.y, curTriangle->s.z };
					float V[3] = { curTriangle->t.x, curTriangle->t.y, curTriangle->t.z };
					for (int n = 0; n < 3; n++) {
						float u = U[n];
						float v = V[n];
						word vertexID = curTriangle->vertexIndex[n];
						if (vertexID < numVertices) {
							ms3d_vertex *vertex = &vertices[vertexID];
							if (vertex != nullptr) {
								char boneID = vertex->boneID;
								retModel.verteces.push_back(
									vec4(vertex->vertex.x,
										vertex->vertex.y,
										vertex->vertex.z, boneID));
								retModel.uvCoords.push_back(vec2(u, v));
							}
							vec3 normal = curTriangle->vertexNormals[n];
							retModel.normals.push_back(normal);
						}
					}
				}
			}
		}
};

int ms3dModelLoader::getJointByName(char name[32], int numJoints, ms3d_joint *joints){
	for(int i = 0; i < numJoints; i ++){
		ms3d_joint *curJoint = &joints[i];
		if(string(name) == string(curJoint->name)){
			return i;
		}
	}
	return -1;
};

void ms3dModelLoader::buildArmature(int numJoints, int numFrames, float fps,  ms3d_joint* joints, Ms3dModel &model){
	for(int i = 0; i < numJoints; i ++){
		
		
		//newJoint = Joint(animData, baseData, parent); 
		//newJoint.globalSkeliton = globalMat;
		//model.armature->addJoint(newJoint);
	}
};


Ms3dModel ms3dModelLoader::loadMs3dModel(string modelPath, TextureHandler* textureHandler, bool displayDebug){
	this->textureHandler = textureHandler;
	ifstream modelFile(modelPath.c_str(), std::ios::binary);

	//Temp data holders
	char charData;
	Ms3dModel retModel = Ms3dModel();

	//Needed integers.
	int version;
	int positionID = 0;

	//Needed words
	word numVertices = 0;
	word numTriangles = 0;
	word numGroups = 0;
	word numMaterials = 0;
	word numJoints = 0;

	//Needed structures
	ms3d_header header;
	ms3d_vertex *verteces;
	ms3d_triangle *triangles;
	ms3d_group *groups;
	ms3d_material * materials;
	ms3d_joint *joints;


	char modelID[11]; 
	modelFile.read(modelID, 10);
	modelID[10] = '\0';
	if(string(modelID) == string("MS3D000000")){
		modelID[10] = '\0';
		//Read header.
		version = readInt(modelFile);
		memcpy(&header.ID[0], &modelID[0], 11);
		header.version = version;

		//Read verteces
		numVertices = readWord(modelFile);

		//Display num verteces
		verteces = new ms3d_vertex[numVertices];
		memset(&verteces[0], 0, 15 * numVertices);
		cout << "Loading vertices..." << endl;
		//Read vertex data
		loadVertices(modelFile, verteces, numVertices);

		//Read triangles
		numTriangles = readWord(modelFile);

		//Alocate space for triangles
		triangles = new ms3d_triangle[numTriangles];
		memset(&triangles[0], 0, 70 * numTriangles);
		cout << "Loading triangles..." << endl;
		//Loop trugh all the triangles
		loadTriangles(modelFile, triangles, numTriangles);

		//Read number of groups
		numGroups = readWord(modelFile);

		//Alocate memory for groups
		groups = new ms3d_group[numGroups];
		cout << "Loading groups..." << endl;
		loadGroups(modelFile, groups, numGroups);

		//Read materials
		numMaterials = readWord(modelFile);

		//Alocate memory for materials
		materials = new ms3d_material[numMaterials];
		memset(&materials[0], 0, 361*numMaterials);
		cout << "Loading materials..." << endl;
		loadMaterials(modelFile, materials, numMaterials);

		//Read joint data
		float animationFPS = readFloat(modelFile);
		float currentTime = readFloat(modelFile);
		int totalFrames = readInt(modelFile);
		numJoints = readWord(modelFile);

		//Alocate memory for joints
		joints = new ms3d_joint[numJoints];
		cout << "Loading Joints..." << endl;
		loadJoints(modelFile, joints, numJoints, animationFPS);

		if(displayDebug){
		//Display header...
		cout << "Model ID : " << header.ID << endl;
		cout << "Version  : " << header.version << endl;
		cout << endl;

		//Display all vertice data
		cout << "Number of verteces : " << numVertices << endl;
		cout << "///////////VERTEX DATA///////////" << endl;

		for(int i = 0; i < numVertices; i ++){
			ms3d_vertex *curVertex = &verteces[i];
			cout << "flags     : " << (int)curVertex->flag << endl;
			cout << "positions : [ : " << curVertex->vertex[0] << ", " << curVertex->vertex[1] << ", " << curVertex->vertex[2] << "]" << endl;
			cout << "Bone ID   : " << (int)curVertex->boneID << endl;
			cout << "Ref Count : " << (int)curVertex->referenceCount << endl;
			cout << "...................................." << endl;
		}
		cout << endl;

		//Display all triangle data
		cout << "Number of triangles : " << numTriangles << endl;
		cout << "///////////TRIANGLE DATA///////////" << endl;
		for(int i = 0; i < numTriangles; i ++){
			ms3d_triangle *curTriangle = &triangles[i];
			cout << "flags               : " << (int)curTriangle->flag << endl;
			cout << "Vertex indexes      : " << endl;
			cout << "                  A : " << curTriangle->vertexIndex[0] << endl;
			cout << "                  B : " << curTriangle->vertexIndex[1] << endl;
			cout << "                  C : " << curTriangle->vertexIndex[2] << endl;

			cout << "Vertex normals      : " << endl;
			cout << "            A [" << curTriangle->vertexNormals[0][0] << ", " << curTriangle->vertexNormals[0][1] << ", " << curTriangle->vertexNormals[0][2] << "]" << endl;

			cout << "Vertex normals      : " << endl;
			cout << "            B [" << curTriangle->vertexNormals[1][0] << ", " << curTriangle->vertexNormals[1][1] << ", " << curTriangle->vertexNormals[1][2] << "]" << endl;

			cout << "Vertex normals      : " << endl;
			cout << "            C [" << curTriangle->vertexNormals[2][0] << ", " << curTriangle->vertexNormals[2][1] << ", " << curTriangle->vertexNormals[2][2] << "]" << endl;

			cout << "Texture coordinates : " << endl;
			cout << "		     S [" << curTriangle->s[0] << ", " << curTriangle->s[1] << ", " << curTriangle->s[2] << "]" << endl;

			cout << "		     T [" << curTriangle->t[0] << ", " << curTriangle->t[1] << ", " << curTriangle->t[2] << "]" << endl;

			cout << "Smooth Group        : " << (int)curTriangle->smoothGroup << endl;
			cout << "Group Index         : " << (int)curTriangle->groupIndex << endl;
			cout << "...................................." << endl;
		}
		cout << endl;

		//Display group info
		cout << "Number of groups : " << numGroups << endl;
		cout << "///////////GROUP DATA///////////" << endl;
		
		for(int i = 0; i < numGroups; i ++){
			ms3d_group *curGroup = &groups[i];
			cout << "Flag : " << (int)curGroup->flag << endl;
			cout << "Name : " << curGroup->name << endl;
			cout << "Triangles[" << curGroup->numTriangles << "] : " << endl;
			for(int k = 0; k < curGroup->numTriangles; k ++){
				cout << "	Index : " << curGroup->triangleIndices[k] << endl;
			}
			cout << "Material index : " << (int)curGroup->materialIndex << endl;
			cout << "...................................." << endl;
		}
		cout << endl;

		//Display material data
		cout << "Number of materials : " << numMaterials << endl;
		cout << "///////////MATERIAL DATA///////////" << endl;
		for(int i = 0; i < numMaterials; i ++){
			ms3d_material *curMaterial = &materials[i];
			cout << "Name         : " << curMaterial->name << endl;
			cout << "Ambiant      [" << curMaterial->ambiant[0] << ", " << curMaterial->ambiant[1] << ", " << curMaterial->ambiant[2] << ", " << curMaterial->ambiant[3] << "]" << endl;
			cout << "Diffuse      [" << curMaterial->diffuse[0] << ", " << curMaterial->diffuse[1] << ", " << curMaterial->diffuse[2] << ", " << curMaterial->diffuse[3] << "]" << endl;
			cout << "Specular     [" << curMaterial->specular[0] << ", " << curMaterial->specular[1] << ", " << curMaterial->specular[2] << ", " << curMaterial->specular[3] << "]" << endl;
			cout << "Emissive     [" << curMaterial->emissive[0] << ", " << curMaterial->emissive[1] << ", " << curMaterial->emissive[2] << ", " << curMaterial->emissive[3] << "]" << endl;
			cout << "Shininess    : " << curMaterial->shininess << endl;
			cout << "Transparency : " << curMaterial->transparency << endl;
			cout << "Mode         : " << (int)curMaterial->mode << endl;
			cout << "Texture      : " << curMaterial->texture << endl;
			cout << "Alphamap     : " << curMaterial->alphamap << endl;
			cout << "...................................." << endl;
		}
		cout << endl;

		//Display keyframe data
		cout << "Number of joints : " << numJoints << endl;
		cout << "///////////JOINT DATA///////////" << endl;
		for(int i = 0; i < numJoints; i ++){
			ms3d_joint *curJoint = &joints[i];
			cout << "Flag        : " << (int)curJoint->flag << endl;
			cout << "Name        : " << curJoint->name << endl;
			cout << "Parent Name : " << curJoint->parentName << endl;
			cout << "Rotation[" << curJoint->rotation[0] << ", " << curJoint->rotation[1] << ", " << curJoint->rotation[2] << "]" << endl;
			cout << "Position[" << curJoint->position[0] << ", " << curJoint->position[1] << ", " << curJoint->position[2] << "]" << endl;

			for(int k = 0; k < curJoint->numKeyframeRot; k ++){
				cout << "Rotation[" << k << "]" << endl;
				cout << "	Frame : " << curJoint->keyframeRot[k].frame << endl;
				cout << "	Rotation[" << curJoint->keyframeRot[k].rotation[0] << ", " << curJoint->keyframeRot[k].rotation[1] << ", " << curJoint->keyframeRot[k].rotation[2] << "]" << endl;
			}

			for(int k = 0; k < curJoint->numKeyframeRot; k ++){
				cout << "Transformation[" << k << "]" << endl;
				cout << "	Frame : " << curJoint->keyframeRot[k].frame << endl;
				cout << "	Transformation[" << curJoint->keyframeTran[k].position[0] << ", " << curJoint->keyframeTran[k].position[1] << ", " << curJoint->keyframeTran[k].position[2] << "]" << endl;
			}
			cout << "...................................." << endl;
		}
		}
		cout << "Building joints" << endl;
		buildArmature(numJoints, totalFrames, animationFPS,joints, retModel);
		cout << "Building object..." << endl;
		createObject(numTriangles, numGroups, numVertices, groups, triangles, verteces, retModel);
		cout << "Loading textures..." << endl;
		string texture = string(materials[0].texture);
		int lastPos = texture.find_last_of("\\", texture.length())+1;
		texture = "Data/Textures/" + texture.substr(lastPos, texture.length() - lastPos);
		retModel.textureID = textureHandler->getSize();
		textureHandler->addTexture(Texture(texture));
	}else{
		cout << "Not a ms3d model file!" << endl;
	}
	if(numVertices > 0) delete verteces;
	if(numTriangles > 0) delete triangles;
	if(numGroups > 0) {
		for(int i = 0; i < numGroups; i ++){
			delete groups[i].triangleIndices;
		}
		delete groups;
	}
	if(numMaterials > 0) delete materials;
	if(numJoints > 0){
		for(int i = 0; i < numJoints; i ++){
			delete joints[i].keyframeRot;
			delete joints[i].keyframeTran;
		}
		delete joints;
	}
	modelFile.close();
	cout << "done!" << endl;
	return retModel;
}
Armature ms3dModelLoader::loadMS3dAnimation(string filename)
{
	ifstream file(filename.c_str(), std::ios::binary);
	//Reading variables :
	char nameBuff[32];
	short numJoints;

	//Armature variables :
	word numAnimations;
	float fps;
	int numFrames;
	string armName;
	vector<Joint> baseJoints;

	//Animation variables :
	vector<Animation> animations;
	string animName;
	short endFrame = 0;
	short numKeys = 0;

	//Key variables : 
	vector<Keyframe> keys;
	short frame = 0;

	//Base joint variables : 
	string baseJName;
	string basePJName;
	vec3 baseJPos;
	vec3 baseJRot;
	vec3 baseJKPos;
	vec3 baseJKRot;
	int baseParIndex;

	//Key joint variables;
	vector<Joint> keyJoints;
	string kJointName;
	string kJointParName;
	vec3 kJointPos;
	vec3 kJointRot;
	int kJointparIndex = -1;
	glm::mat3x4 kJointBaseMat;
	vec4 kJointBoneRot = vec4(0, 0, 0, 1);
	glm::mat3x4 kJointBoneMat;
	glm::mat3x4 kJointGlobalMat;
	armName = readString(file, 32);
	fps = readFloat(file);


	numJoints = readWord(file);
	
	for (int i = 0; i < numJoints; i++) {
		//Read joint data
		baseJName = readString(file, 32);
		basePJName = readString(file, 32);
		baseJPos = readVec3(file);
		baseJRot = readVec3(file);
		baseJKRot = readVec3(file);
		baseJKPos = readVec3(file);
		baseParIndex = getJointByName(basePJName, baseJoints);

		//Create new variables that is needed
		mat3x4 baseData = mat3x4(1);
		glm::mat3x4 boneMat;
		vec4 boneRot = vec4(0, 0, 0, 1);
		glm::mat3x4 globalMat = mat3x4();

		baseData = createBaseMatrix(baseJRot, baseJPos);
		
		if (baseParIndex == -1) {
			globalMat = baseData;
			boneMat = createBoneMatrix(baseJKRot, baseJKPos);
			Math::AngleQuaternion(baseJKRot, boneRot);
		}
		else {
			boneMat = createBoneMatrix(baseJKRot, baseJKPos);
			Math::AngleQuaternion(baseJKRot, boneRot);
		}

		baseJoints.push_back(Joint(boneMat, baseJKPos, boneRot, i, baseParIndex, baseJName));
	}

	//Read number of animations
	numAnimations = readWord(file);

	int prevEnd = 0;
	for (int i = 0; i < numAnimations; i++) {
		file.read(nameBuff, 32);
		endFrame = readWord(file) - prevEnd;
		animName = string(nameBuff);
		keys.clear();
		numKeys = readWord(file);
		for (int k = 0; k < numKeys; k++) {
			frame = readWord(file);
			numJoints = readWord(file);
			keyJoints.clear();
			for (int a = 0; a < numJoints; a++) {
				kJointName = readString(file, 32);
				kJointParName = readString(file, 32);
				kJointPos = readVec3(file);
				kJointRot = readVec3(file);
				Joint baseJoint = baseJoints[getJointByName(kJointName, baseJoints)];
				kJointparIndex = getJointByName(kJointParName, baseJoints);
				kJointBaseMat = *baseJoint.getBoneMatrix();
				kJointBoneMat = createBoneMatrix(kJointRot, kJointPos);
				Math::AngleQuaternion(kJointRot, kJointBoneRot);
				
				if (kJointparIndex != -1) {
					kJointGlobalMat = mat3x4();
				}
				else {
					kJointGlobalMat = kJointBaseMat;
				}
				keyJoints.push_back(Joint(kJointBaseMat, kJointPos, kJointBoneRot, baseJoint.getJointID(), kJointparIndex, kJointName));
			}
			Keyframe keyframe = Keyframe(keyJoints, frame);
			keys.push_back(keyframe);
		}
		animations.push_back(Animation(keys, animName, endFrame));
	}
	return Armature(fps, numAnimations, animations, baseJoints, armName);
};

int ms3dModelLoader::getJointByName(string name, vector<Joint>& joints) {
	for (int i = 0; i < joints.size(); i++) {
		if (name == joints[i].getName()) {
			return i;
		}
	}
	return -1;
}