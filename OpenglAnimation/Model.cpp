#include "Model.h"

Model::Model(){
	this->verteces = vector<vec4>();
	this->uvCoords = vector<vec2>();
	this->normals = vector<vec3>();
	this->texture = nullptr;
	this->name = "";
	this->vertexBuffer = 0;
	this->uvBuffer = 0;
	this->normalBuffer = 0;
};

Model::Model(Ms3dModel model, TextureHandler *textureHandler){
	this->verteces = model.verteces;
	this->uvCoords = model.uvCoords;
	this->normals = model.normals;
	this->texture = textureHandler->getTexture(model.textureID);
	this->name = model.name;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec4) * verteces.size() , &verteces[0], GL_STATIC_DRAW);

	glGenBuffers(1, &uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * uvCoords.size() , &this->uvCoords[0], GL_STATIC_DRAW);

	glGenBuffers(1, &normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * normals.size(), &this->normals[0], GL_STATIC_DRAW);
}

string Model::getName() {
	return name;
}
void Model::removeClothes(Model &mod)
{
	vector<vec4> clothes = mod.getVertices();
	bool hasIndece[64];
	for (int i = 0; i < clothes.size(); i++) {
		int indece = (int)clothes[i].w;
		hasIndece[indece] = false;
	}
	for (int i = 0; i < verteces.size(); i++) {
		int indece = (int)verteces[i].w;
		//aaverteces[i].w = -1.0;
	}
};

Texture *Model::getTexture() {
	return this->texture;
};
	
vector<vec4> Model::getVertices(){
	return verteces;
};

vector<vec2> Model::getUvCoords(){
	return uvCoords;
};

vector<vec3> Model::getNormals()
{
	return normals;
};

GLuint Model::getVertexBuffer(){
	return this->vertexBuffer;
};

GLuint Model::getUvBuffer(){
	return this->uvBuffer;
};

GLuint Model::getNormalBuffer()
{
	return this->normalBuffer;
};