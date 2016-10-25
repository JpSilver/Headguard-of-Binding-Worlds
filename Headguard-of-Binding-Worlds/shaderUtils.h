#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm\gtx\euler_angles.hpp>
#include <string>
#include <fstream>
#include <vector>

using glm::vec3;
using glm::mat4;
using std::string;
using std::ifstream;
using std::vector;

class ShaderUtils {
public:
	static mat4 rotateMatrix(mat4 matrix, vec3 rotation) {
		matrix = glm::rotate(matrix, rotation.x, vec3(1, 0, 0));
		matrix = glm::rotate(matrix, rotation.y, vec3(0, 1, 0));
		matrix = glm::rotate(matrix, rotation.z, vec3(0, 0, 1));
		return matrix;
	};

	static float transitionToFrame(float o, float n, float frame, float prevFrame, float nextFrame) {
		if (o == 0 && n == 0) {
			return 0;
		}
		return o + (((frame - prevFrame) / (nextFrame - prevFrame))*(n - o));
	};

	static vec3 transitionToFrame(vec3 o, vec3 n, float frame, float prevFrame, float nextFrame) {
		vec3 ret;
		if (o == vec3(0) && n == vec3(0)) {
			return vec3(0);
		}
		ret.x = o.x + (((frame - prevFrame) / (nextFrame - prevFrame))*(n.x - o.x));
		ret.y = o.y + (((frame - prevFrame) / (nextFrame - prevFrame))*(n.y - o.y));
		ret.z = o.z + (((frame - prevFrame) / (nextFrame - prevFrame))*(n.z - o.z));
		return ret;
	};

	static mat4 transitionToFrame(mat4 o, mat4 n, float frame, float prevFrame, float nextFrame) {
		mat4 retMat = mat4(1);
		for (int x = 0; x < 4; x++) {
			for (int y = 0; y < 4; y++) {
				retMat[x][y] = transitionToFrame(o[x][y], n[x][y], frame, prevFrame, nextFrame);
			}
		}
		return retMat;
	};

	inline GLuint LoadShader(string vertexShader, string fragmentShader) {
		//Create the shaders
		GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

		//Required variables
		string vertexShaderCode = "";
		string fragmentShaderCode = "";

		ifstream vertexShaderStream(vertexShader.c_str());
		ifstream fragmentShaderStream(fragmentShader.c_str());

		GLint result = GL_FALSE;
		int infoLogLength;

		//Read vertex shader code
		if (vertexShaderStream.is_open()) {
			string line = "";
			while (getline(vertexShaderStream, line)) {
				vertexShaderCode += "\n" + line;
			}
			vertexShaderStream.close();
		}
		else {
			printf("Could not open %s...\n", vertexShader.c_str());
			getchar();
			return 0;
		}

		//Read fragment shader code
		if (fragmentShaderStream.is_open()) {
			string line = "";
			while (getline(fragmentShaderStream, line)) {
				fragmentShaderCode += "\n" + line;
			}
			fragmentShaderStream.close();
		}

		//Compile vertex shader
		printf("Compiling vertex shader :%s\n", vertexShader.c_str());
		char const *vertexSoursePointer = vertexShaderCode.c_str();
		glShaderSource(vertexShaderID, 1, &vertexSoursePointer, NULL);
		glCompileShader(vertexShaderID);

		//Check vertex shader
		glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &result);
		glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0) {
			vector<char> shaderErrorMsg(infoLogLength + 1);
			glGetShaderInfoLog(vertexShaderID, infoLogLength, NULL, &shaderErrorMsg[0]);
			printf("%s\n", &shaderErrorMsg[0]);
		}

		//Compile fragment shader
		printf("Compiling fragment shader :%s\n", fragmentShader.c_str());
		char const *fragmentSoursePointer = fragmentShaderCode.c_str();
		glShaderSource(fragmentShaderID, 1, &fragmentSoursePointer, NULL);
		glCompileShader(fragmentShaderID);

		//Check vertex shader
		glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &result);
		glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0) {
			vector<char> shaderErrorMsg(infoLogLength + 1);
			glGetShaderInfoLog(fragmentShaderID, infoLogLength, NULL, &shaderErrorMsg[0]);
			printf("%s\n", &shaderErrorMsg[0]);
		}

		//Link the program
		printf("Linking program\n");
		GLuint programID = glCreateProgram();
		glAttachShader(programID, vertexShaderID);
		glAttachShader(programID, fragmentShaderID);
		glLinkProgram(programID);

		//Check the program
		glGetShaderiv(programID, GL_LINK_STATUS, &result);
		glGetShaderiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0) {
			vector<char> shaderErrorMsg(infoLogLength + 1);
			glGetShaderInfoLog(programID, infoLogLength, NULL, &shaderErrorMsg[0]);
			printf("%s\n", &shaderErrorMsg[0]);
		}

		glDetachShader(programID, vertexShaderID);
		glDetachShader(programID, fragmentShaderID);
		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);

		return programID;
	};
};