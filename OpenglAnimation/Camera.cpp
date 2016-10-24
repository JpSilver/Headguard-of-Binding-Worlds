#include "Camera.h"

Camera::Camera(){
	this->position = vec3(0.0);
	this->direction = vec3(0.0);
};

Camera::Camera(vec3 position){
	this->position = position;
	this->direction = vec3(0.0);
};

Camera::Camera(vec3 position, vec3 rotation){
	this->position = position;
	this->direction = rotation;
};
mat4 Camera::getViewMatrix(){
	return glm::lookAt(
		this->position, // Camera is at (4,3,3), in World Space
		target, // and looks at the origin
		this->up  // Head is up (set to 0,-1,0 to look upside-down)
		);
}

void Camera::setMousePosition(vec2 mp)
{
	this->mousePosition = mp;
};

void Camera::rotateToMouse(vec2 newMp, float distance)
{
	float rotX = (float)(newMp.x - mousePosition.x)/distance;
	float rotY = (float)(newMp.y - mousePosition.y)/distance;
	addRotation(vec3(0.0F, rotX, rotY));
	this->mousePosition = newMp;
	cout << rotX << endl;
	cout << rotY << endl;
};

void Camera::addRotation(vec3 rot) {
	direction.x += rot.x;
	direction.y += rot.y;
	direction.z += rot.z;
	target.x = cos(direction.z) * sin(direction.y);
	target.y = sin(direction.z);
	target.z = cos(direction.z) * cos(direction.y);
	target = position + target;
}
