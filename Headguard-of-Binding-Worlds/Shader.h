#ifndef SHADER_h
#define SHADER_h

#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_interpolation.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec4.hpp>
#include <glm\glm.hpp>
#include <iostream>
#include <vector>
#include "shaderUtils.h"
#include "Texture.h"
#include "Armature.h"
#include "MathUtils.h"
#include "Light.h"

using glm::mat3;
using glm::toMat4;
using glm::mat4;
using glm::vec3;

using namespace std;

class Shader{
public:
	Shader(string, string);
	Shader();
	void initUniforms();
	int getProgramID(){
		return this->programID;
	};
	GLuint loadUniformMat4(mat4, string, int);
	GLuint loadUniformVec3Vector(vector<vec3>, string);
	GLuint loadUniformJoits(Armature*, vector<glm::mat3x4> &);
	void loadLight(Light light);
	void bindTexture(Texture*, GLuint);

private:
	float frameA = 0;
	float frameB = 0;
	GLuint programID;

};

#endif !SHADER_h