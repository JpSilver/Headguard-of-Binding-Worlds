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

GLuint Shader::loadUniformJoits(Armature &armature, vector<glm::mat3x4> &animations, double deltaTime){
	GLuint id = 0;

	armature.addFrame(deltaTime);
	float frame = armature.getFrame();
	Animation prevAnimation = armature.getPrevAnimation();
	Keyframe startKey = armature.getBaseAnimation().getKeyframeStart();
	Keyframe blend = prevAnimation.getKeyframeStart();
	Keyframe prevKey = armature.getAnimation().getKeyframeStart();
	Keyframe nextKey = armature.getAnimation().getKeyframeEnd();
	int numPrevJoints = prevKey.getJoints()->size();
	int numNextJoints = nextKey.getJoints()->size();
	vector<Joint> baseJoints = armature.getBaseJoints();
	vector<Joint> startJoints;
	vector<Joint> endJoints;
	int numJoints = baseJoints.size();
	int bStartFrame = blend.getFrame();
	int bEndFrame = prevAnimation.getKeyframeEnd().getFrame();
	int pFrame = prevKey.getFrame();
	int nFrame = nextKey.getFrame();
	int prevAnim = armature.getPrevAnimationIndex();
	int animIndex = armature.getAnimationIndex();
	int tStart = 0;
	int tPrev = 0;
	int tNext = 0;
	float endFrame = (float)bStartFrame+0.1F / (float)bEndFrame;
	float frameP = endFrame + (float)pFrame + 0.1F / (float)nFrame;
	float timeA = prevKey.getFrame();
	float timeB = nextKey.getFrame();
	if (prevAnimation.getFrame()+0.1 > bEndFrame && prevAnim != animIndex) {
		//armature.resetPrevAnimation();
	}
	cout << prevAnim << ' ' << animIndex << endl;
	bool addFrame = false;
	for (int i = 0; i < baseJoints.size(); i++) {
		Joint start = *startKey.getJoint(i);
		if ((tStart < numPrevJoints) && (blend.getJoint(tStart)->getJointID() == i)) {
			start = *prevKey.getJoint(tStart);
			tStart++;
		}
		startJoints.push_back(start);
		if ((tNext < numNextJoints)&&(nextKey.getJoint(tNext)->getJointID() == i)){
			endJoints.push_back(*nextKey.getJoint(tNext));
			tNext++;
		}
		else {
			endJoints.push_back(*startKey.getJoint(i));
		}
	}
	
	if (frame < timeA) {
		frame = timeA + (frame - (int)frame);
	}
	for (int i = 0; i < numJoints; i++) {
				
		Joint *jointStart = &startJoints[i];
		Joint *jointEnd = &endJoints[i];

		string jointBase = string("mLocal[") + to_string((long long)i) + string("]");
		string jointMatrix = string("mGlobal[") + to_string((long long)i) + string("]");

		glm::mat3x4 matAnimate;
		
		vec3 startPos = jointStart->getPos();
		vec3 endPos = jointEnd->getPos();

		vec3 pos = ShaderUtils::transitionToFrame(startPos, endPos, frame, timeA, timeB);

		vec4 startRot = jointStart->getRotation();
		vec4 endRot = jointEnd->getRotation();
		
		float t = (float)(frame - timeA) / (float)(timeB - timeA);
		vec4 quat = glm::mix(startRot, endRot, t);
		quat = glm::normalize(quat);
		Math::QuaternionMatrix(quat, matAnimate);
		matAnimate[0][3] = pos.x;
		matAnimate[1][3] = pos.y;
		matAnimate[2][3] = pos.z;

		Math::R_ConcatTransforms(baseJoints[i].getBoneMatrix(), matAnimate, baseJoints[i].localMatrix);

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

