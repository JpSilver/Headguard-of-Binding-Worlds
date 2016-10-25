#include "StaticEntity.h"

void StaticEntity::draw(Camera camera, Shader shader)
{
	for (int i = 0; i < this->models.size(); i++) {
		Model *model = models[i];
		mat4 viewMatrix = camera.getViewMatrix();
		mat4 modelMatrix = mat4(1);
		modelMatrix = glm::translate(modelMatrix, position);
		modelMatrix = glm::rotate(modelMatrix, rotation.x, vec3(1.0f, 0.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, rotation.y, vec3(0.0f, 1.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, rotation.z, vec3(0.0f, 0.0f, 1.0f));
		modelMatrix = glm::scale(modelMatrix, vec3(1, 1, 1));

		// Use our shader
		int programID = shader.getProgramID();
		glUseProgram(programID);
		shader.loadUniformMat4(projectionMatrix, "mProject", programID);
		shader.loadUniformMat4(viewMatrix, "mView", programID);
		shader.loadUniformMat4(modelMatrix, "mModel", programID);
		shader.loadLight(Light(vec3(4, 4, 4), vec3(0.0F, 0.0F, 0.0F), vec3(1, 0, 0), LIGHTSUN, 0, "SUN"));
		//shader.loadUniformJoits(&armature);
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
};
