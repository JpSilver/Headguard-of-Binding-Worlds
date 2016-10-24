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
	vector<Joint> joints = armature->getJoints();
	float frameS = armature->getFrame();
	if(frameS < 0.0F){
		for(int i = 0; i < joints.size(); i ++){
			joints[i].matLocal = joints[i].getDefaultMatrix();
			joints[i].matGlobal = joints[i].globalSkeliton;
		}
	}
	Animation curAnimation = armature->getCurrentAnimation();
	Animation prevAnimation = armature->getPreviouseAnimation();
	int minFrame = curAnimation.startFrame;
	int maxFrame = curAnimation.endFrame;
	bool *useBone = new bool[joints.size()];
	memset(useBone, false, sizeof(bool) * joints.size());
	float usePrevFrame = -1;
	float useNextFrame = -1;
	for (int i = 0; i < curAnimation.numBones; i++) {
		int boneIndex = curAnimation.bones[i];
		useBone[boneIndex] = true;
		if (usePrevFrame == -1) {
			usePrevFrame = joints[boneIndex].getPrevFrame(frameS);
			useNextFrame = joints[boneIndex].getNextPositionFrame(frameS);
		}
	}
	int end = 2;
	for (int i = 0; i < joints.size(); i++) {
		float frame = frameS;
		int nextFrame = 1;
		int prevFrame = 2;
		float off = 0;

		nextFrame = joints[i].getNextPositionFrame(frame);
		prevFrame = joints[i].getPrevFrame(frame);
		end = nextFrame;
		if (!useBone[i]) {
			float f = usePrevFrame / (float)frame;//
			float a = joints[i].getPrevFrame(minFrame);
			float b = joints[i].getPrevFrame(a);
			float p = (float)(a - b)*f;

			end = a;
			frame = b + p;
			prevFrame = (int)b;
			nextFrame = 1;
		}

		string jointBase = string("mLocal[") + to_string((long long)i) + string("]");
		string jointMatrix = string("mGlobal[") + to_string((long long)i) + string("]");

		glm::mat3x4 matAnimate;
			
			float timeA = joints[i].getBoneMatrixData(prevFrame).frame;
			float timeB = joints[i].getBoneMatrixData(end).frame;

			vec3 pos = ShaderUtils::transitionToFrame(joints[i].getBoneMatrixData(prevFrame).pos, joints[i].getBoneMatrixData(nextFrame).pos, frame, prevFrame, end);
			AnimationData dataA = joints[i].getBoneMatrixData(prevFrame);
			AnimationData dataB = joints[i].getBoneMatrixData(nextFrame);
			float t = (float)(frame - timeA) / (float)(timeB - timeA);
			vec4 quat = glm::mix(dataA.rot, dataB.rot, t);
			quat = glm::normalize(quat);

			Math::QuaternionMatrix(quat, matAnimate);
			matAnimate[0][3] = pos.x;
			matAnimate[1][3] = pos.y;
			matAnimate[2][3] = pos.z;
		Math::R_ConcatTransforms(joints[i].getDefaultMatrix(), matAnimate, joints[i].matLocal);
		// build up the hierarchy if joints
		// matGlobal = matGlobal(parent) * matLocal
		int parent = joints[i].getParentIndex();
		if (parent == -1)
		{
			joints[i].matGlobal = joints[i].matLocal;
		}
		else
		{
			Joint *pJoint = &joints[parent];
			Math::R_ConcatTransforms(pJoint->matGlobal, joints[i].matLocal, joints[i].matGlobal);
		}

		id = glGetUniformLocation(programID, jointBase.c_str());
		glUniformMatrix3x4fv(id, 1, GL_FALSE, &joints[i].globalSkeliton[0][0]);

		id = glGetUniformLocation(programID, jointMatrix.c_str());
		glUniformMatrix3x4fv(id, 1, GL_FALSE, &joints[i].matGlobal[0][0]);

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

