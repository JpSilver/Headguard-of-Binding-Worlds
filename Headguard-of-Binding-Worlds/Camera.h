#ifndef CAMERA_H
#define CAMERA_H

#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <iostream>

using glm::vec3;
using glm::mat4;
using glm::vec2;
using std::cout;
using std::endl;


class Camera{
public:
	Camera();
	Camera(vec3);
	Camera(vec3, vec3);
	mat4 getViewMatrix();
	void move(vec3 direction){
		position.x += direction.x;
		position.y += direction.y;
		position.z += direction.z;
	};
	vec3 getRotation() {
		vec3 rot = vec3(direction.x, direction.y, -direction.z);
		return rot;
	};
	vec3 getPosition() {
		return this->position;
	};
	void setMousePosition(vec2);
	void rotateToMouse(vec2, float);
	void addRotation(vec3 rot);

private:
	vec3 position;
	vec3 direction;
	vec3 target;
	vec3 up = vec3(0.0, -1.0, 0.0);
	vec2 mousePosition;

};

#endif //CAMERA_H