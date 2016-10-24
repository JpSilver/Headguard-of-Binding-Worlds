#include "Entity.h"

Entity::Entity(){
	shader = 0;
	position = vec3(0);
	rotation = vec3(0);
	scale = vec3(1);
	frame = 0;
	this->modelHandler = nullptr;
	int screen_width = 1024;
	int screen_height = 768;
	float zNear = 0.1F;
	float zFar = 1000.0F;
	float fov = 75.0f;
	projectionMatrix = glm::perspectiveFov(fov, (float)screen_width, (float)screen_height, zNear, zFar);
};

Entity::Entity(vec3 position, Shader *shader, ModelHandler *modelHandler, Armature armature)
{
	this->shader = shader;
	this->position = position;
	this->armature = armature;
	this->rotation = vec3(0);
	this->scale = vec3(1);
	this->modelHandler = modelHandler;
	int screen_width = 1024;
	int screen_height = 768;
	float zNear = 0.1F;
	float zFar = 1000.0F;
	float fov = 75.0f;
	projectionMatrix = glm::perspectiveFov(fov, (float)screen_width, (float)screen_height, zNear, zFar);
};

Entity::Entity(vec3 position, Shader *shader, ModelHandler *modelHandler, string name)
{
	this->shader = shader;
	this->position = position;
	this->armature = armature;
	this->rotation = vec3(0);
	this->scale = vec3(1);
	this->modelHandler = modelHandler;
	this->name = name;
	int screen_width = 1024;
	int screen_height = 768;
	float zNear = 0.1F;
	float zFar = 1000.0F;
	float fov = 75.0f;
	projectionMatrix = glm::perspectiveFov(fov, (float)screen_width, (float)screen_height, zNear, zFar);
};

Entity::Entity(vec3 position, vec3 rotation, vec3 scale, Shader *shader, ModelHandler *modelHandler)
{
	this->shader = shader;
	this->position = position;
	this->rotation = rotation;
	this->scale = scale;
	this->modelHandler = modelHandler;
	int screen_width = 1024;
	int screen_height = 768;
	float zNear = 0.1F;
	float zFar = 1000.0F;
	float fov = 75.0f;
	projectionMatrix = glm::perspectiveFov(fov, (float)screen_width, (float)screen_height, zNear, zFar);
};

void Entity::addModel(int modIndex)
{
	this->models.push_back(modelHandler->getModel(modIndex));
}
void Entity::addModel(Model model)
{
	this->models.push_back(modelHandler->addModel(model));
};

void Entity::draw(Camera camera, Shader shader){
	armature.addFrame();
	vector<glm::mat3x4> animations = vector<glm::mat3x4>();
	mat4 modelMatrix = mat4(1);
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::rotate(modelMatrix, rotation.x, vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, rotation.y, vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, rotation.z, vec3(0.0f, 0.0f, 1.0f));
	modelMatrix = glm::scale(modelMatrix, vec3(1, 1, 1));

	mat4 viewMatrix = camera.getViewMatrix();
	shader.loadUniformJoits(&armature, animations);
	// Use our shader
	for (int i = 0; i < this->models.size(); i ++) {
		int programID = shader.getProgramID();
		glUseProgram(programID);

		shader.loadUniformMat4(projectionMatrix, "mProject", programID);
		shader.loadUniformMat4(viewMatrix, "mView", programID);
		shader.loadUniformMat4(modelMatrix, "mModel", programID);
		shader.loadLight(Light(vec3(4, 4, 4), vec3(0.0F, 0.0F, 0.0F), vec3(1, 0, 0), LIGHTSUN, 0, "SUN"));
		Model *model = models[i];
		shader.bindTexture(model->getTexture(), 0);
		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, model->getVertexBuffer());
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			4,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, model->getUvBuffer());
		glVertexAttribPointer(
			1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			2,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, model->getNormalBuffer());
		glVertexAttribPointer(
			2,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, model->getVertices().size() * 4); // 3 indices starting at 0 -> 1 triangle
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
	}
	
}
void Entity::setPosition(vec3 position)
{
	this->position = position;
}
void Entity::setRotation(vec3 rotation)
{
	this->rotation = rotation;
}
void Entity::setAnimation(string animName)
{
	this->armature.setAnimation(animName);
};

GLuint Entity::getProgram(){
	return this->shader->getProgramID();
}
string Entity::getName()
{
	return name;
};