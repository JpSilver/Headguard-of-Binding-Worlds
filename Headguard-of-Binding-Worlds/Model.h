#ifndef MODEL_H
#define MODEL_H
#include <vector>
#include "glm/glm.hpp"
#include "Ms3dModel.h"
#include "TextureHandler.h"

using glm::vec3;
using glm::vec4;
using glm::vec2;
using std::vector;

class Model{
public:
	Model();
	//Model(const Model&);
	Model(Ms3dModel, TextureHandler*);
	vector<vec4> getVertices();
	vector<vec2> getUvCoords();
	vector<vec3> getNormals();
	GLuint getVertexBuffer();
	GLuint getUvBuffer();
	GLuint getNormalBuffer();
	Texture *getTexture();
	string getName();
	void removeClothes(Model &mod);

private:
	string name;
	GLuint vertexBuffer;
	GLuint uvBuffer;
	GLuint normalBuffer;
	vector<vec4> verteces;
	vector<vec4> hidden;
	vector<vec2> uvCoords;
	vector<vec3> normals;
	Texture *texture;
};


#endif //MODEL_H