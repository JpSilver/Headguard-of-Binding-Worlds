#ifndef ENTITY_H
#define ENTITY_H
#include "Model.h"
#include "ModelHandler.h"
#include "Camera.h"
#include <glm/gtx/quaternion.hpp>
#include "Shader.h"

class Entity{
public:
	Entity(vec3, Shader *, ModelHandler *, Armature &armature);
	Entity(vec3, Shader *, ModelHandler *, string);
	Entity(vec3, vec3, vec3, Shader *, ModelHandler *);
	void addModel(int modIndex);
	void addModel(Model model);
	void draw(Camera, Shader, double);
	void setPosition(vec3 position);
	void setRotation(vec3 rotation);
	void setAnimation(string);
	GLuint getProgram();
	string getName();
protected:
	Shader *shader;
	vector<Model *> models;
	ModelHandler *modelHandler;
	vec3 position;
	vec3 rotation;
	vec3 scale;
	float frame;
	string name;
	Armature armature;
	mat4 projectionMatrix;
};

#endif //ENTITY_H