#include "Shader.h"

Shader::Shader(){
	this->programID = 0;
};

Shader::Shader(string vertexShader, string fragmentShader){
	ShaderUtils shaderUtils = ShaderUtils();
	this->programID = shaderUtils.LoadShader(vertexShader, fragmentShader);
};

void Shader::initUniforms(){

};

GLuint Shader::loadUniformMat4(mat4 value, string name, int programID){
	GLuint MatrixID = glGetUniformLocation(programID, name.c_str());
  
	// Send our transformation to the currently bound shader, in the "MVP" uniform
	// This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &value[0][0]);
	return MatrixID;
};

void Shader::bindTexture(Texture *texture, GLuint textureUnit) {
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, texture->getTextureID());
};

GLuint Shader::loadUniformVec3Vector(vector<vec3> values, string name){
	GLuint id = 0;
	for(int i = 0; i < values.size(); i ++){
		string fin = name + string("[") + to_string((long long)i) + string("]");
		id = glGetUniformLocation(programID, fin.c_str());
		glUniform3f(id, values[i].x, values[i].y, values[i].z);
	}
	return id;
};

GLuint Shader::loadUniformJoits(Armature *armature, vector<glm::mat3x4> &animations){
	GLuint id = 0;
	vector<Joint> *baseJointsPtr = armature->getBaseJoints();
	vector<Joint> baseJoints = *baseJointsPtr;
	vector<Joint> startJoints;
	vector<Joint> endJoints;
	int numJoints = baseJointsPtr->size();
	Keyframe *prevKey = armature->getAnimation()->getKeyframeStart();
	Keyframe *nextKey = armature->getAnimation()->getKeyframeEnd();

	bool *useBone = new bool[numJoints];
	memset(useBone, false, sizeof(bool) * numJoints);
	int itStart = 0;
	int itEnd = 0;
	for (int i = 0; i < numJoints; i++) {
		Joint start = prevKey->getJoints()->at(itStart);
		Joint end = prevKey->getJoints()->at(itEnd);
		if (i == start.getJointID()) {
			startJoints.push_back(start);
			itStart++;
		}
		else {
			startJoints.push_back(baseJoints[i]);
		}
		if (i == end.getJointID()) {
			endJoints.push_back(end);
			itEnd++;
		}
		else {
			endJoints.push_back(baseJoints[i]);
		}
	}
	float frame = armature->getFrame();
	for (int i = 0; i < numJoints; i++) {
				
		Joint *jointStart = &startJoints[i];
		Joint *jointEnd = &endJoints[i];

		string jointBase = string("mLocal[") + to_string((long long)i) + string("]");
		string jointMatrix = string("mGlobal[") + to_string((long long)i) + string("]");
		glm::mat3x4 matAnimate;

		float timeA = prevKey->getFrame();
		float timeB = nextKey->getFrame();

		vec3 startPos = *jointStart->getPos();
		vec3 endPos = *jointEnd->getPos();

		vec3 pos = ShaderUtils::transitionToFrame(startPos, endPos, frame, 0, nextKey->getFrame());

		vec4 startRot = *jointStart->getRotation();
		vec4 endRot = *jointEnd->getRotation();
		float t = (float)(frame - timeA) / (float)(timeB - timeA);
		vec4 quat = glm::mix(startRot, endRot, t);
		quat = glm::normalize(quat);
		Math::QuaternionMatrix(quat, matAnimate);
		matAnimate[0][3] = pos.x;
		matAnimate[1][3] = pos.y;
		matAnimate[2][3] = pos.z;

		Math::R_ConcatTransforms(baseJoints[i].localMatrix, matAnimate, baseJoints[i].localMatrix);

		int parent = baseJoints[i].getParentID();
		if (parent == -1)
		{
			baseJoints[i].globalMatrix = baseJoints[i].localMatrix;
		}
		else
		{
			Joint *pJoint = &baseJoints[parent];
			Math::R_ConcatTransforms(pJoint->globalMatrix, baseJoints[i].localMatrix, baseJoints[i].globalMatrix);
		}

		id = glGetUniformLocation(programID, jointBase.c_str());
		glUniformMatrix3x4fv(id, 1, GL_FALSE, &baseJoints[i].pGlobal[0][0]);

		id = glGetUniformLocation(programID, jointMatrix.c_str());
		glUniformMatrix3x4fv(id, 1, GL_FALSE, &baseJoints[i].globalMatrix[0][0]);

	}
	delete[] useBone;
	return 0;
}
void Shader::loadLight(Light light)
{
	GLuint lightPosID = glGetUniformLocation(programID, "lPos[0]");
	GLuint lightRotID = glGetUniformLocation(programID, "lRot[0]");

	glUniform3f(lightPosID, light.getPosition().x, light.getPosition().y, light.getPosition().z);
	glUniform3f(lightRotID, light.getRotation().x, light.getRotation().y, light.getRotation().z);
}
;

