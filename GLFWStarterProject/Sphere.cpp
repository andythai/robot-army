#include "Sphere.h"

Sphere::Sphere()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);


	for (int i = 0; i < segments; i++) {
		float angle = 2.0f * glm::pi<float>() * i / segments;
		float next_angle = 2.0f * glm::pi<float>() * (i + 1) / segments;
		float x1 = cos(angle) * radius;
		float y1 = sin(angle) * radius;
		float x2 = cos(next_angle) * radius;
		float y2 = sin(next_angle) * radius;
		glm::vec4 vector1 = { x1, y1, 0, 1 };
		glm::vec4 vector2 = { x1, y2, 0, 1 };
		points.push_back(glm::vec3(vector1));
		points.push_back(glm::vec3(vector2));

		glm::vec4 mod1 = glm::vec4(vector1);
		glm::vec4 mod2 = glm::vec4(vector2);
		for (int j = 0; j < 99; j++) {
			glm::vec4 change1 = glm::rotate(glm::mat4(1.0f), 3.6f * (j + 1) / 180.0f * glm::pi<float>(), glm::vec3(1.0f, 1.0f, 0.0f)) * mod1;
			glm::vec4 change2 = glm::rotate(glm::mat4(1.0f), 3.6f * (j + 1) / 180.0f * glm::pi<float>(), glm::vec3(1.0f, 1.0f, 0.0f)) * mod2;
			points.push_back(glm::vec3(change1));
			points.push_back(glm::vec3(change2));
		}
	}


	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// glBufferData populates the most recently bound buffer with data starting at the 3rd argument and ending after
	// the 2nd argument number of indices. How does OpenGL know how long an index spans? Go to glVertexAttribPointer.
	glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec3), points.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(1,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
		3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
		GL_FLOAT, // What type these components are
		GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
		3 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
		(GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


Sphere::~Sphere()
{
}

void Sphere::draw(GLuint shaderProgram) {

	// Calculate the combination of the model and view (camera inverse) matrices
	glm::mat4 modelview = Window::V * toWorld;
	// We need to calcullate this because modern OpenGL does not keep track of any matrix other than the viewport (D)
	// Consequently, we need to forward the projection, view, and model matrices to the shader programs
	// Get the location of the uniform variables "projection" and "modelview"
	uProjection = glGetUniformLocation(shaderProgram, "projection");
	uModelview = glGetUniformLocation(shaderProgram, "modelview");
	uModel = glGetUniformLocation(shaderProgram, "model");
	uView = glGetUniformLocation(shaderProgram, "view");

	// Materials
	glUniform3f(glGetUniformLocation(shaderProgram, "material.ambient"), 0.7f, 0.1f, 0.1f);
	glUniform3f(glGetUniformLocation(shaderProgram, "material.diffuse"), 0.5f, 0.4f, 0.5f);
	glUniform3f(glGetUniformLocation(shaderProgram, "material.specular"), 0.5f, 0.5f, 0.4f);
	glUniform1f(glGetUniformLocation(shaderProgram, "material.shininess"), 30);

	// Now send these values to the shader program
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);
	glUniformMatrix4fv(uModel, 1, GL_FALSE, &toWorld[0][0]);
	glUniformMatrix4fv(uView, 1, GL_FALSE, &Window::V[0][0]);

	glBindVertexArray(VAO);
	//glDrawElements(GL_TRIANGLES, points.size() / 3, GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_LINES, 0, points.size());
	glBindVertexArray(0);
}

void Sphere::render() {
		// Calculate the combination of the model and view (camera inverse) matrices
		glm::mat4 modelview = Window::V * toWorld;
		// We need to calcullate this because modern OpenGL does not keep track of any matrix other than the viewport (D)
		// Consequently, we need to forward the projection, view, and model matrices to the shader programs
		// Get the location of the uniform variables "projection" and "modelview"
		uProjection = glGetUniformLocation(shaderProgram, "projection");
		uModelview = glGetUniformLocation(shaderProgram, "modelview");
		uModel = glGetUniformLocation(shaderProgram, "model");
		uView = glGetUniformLocation(shaderProgram, "view");

		// Materials
		glUniform3f(glGetUniformLocation(shaderProgram, "material.ambient"), 0.1f, 0.1f, 0.1f);
		glUniform3f(glGetUniformLocation(shaderProgram, "material.diffuse"), 0.5f, 0.4f, 0.5f);
		glUniform3f(glGetUniformLocation(shaderProgram, "material.specular"), 0.5f, 0.5f, 0.4f);
		glUniform1f(glGetUniformLocation(shaderProgram, "material.shininess"), 30);

		// Now send these values to the shader program
		glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
		glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);
		glUniformMatrix4fv(uModel, 1, GL_FALSE, &toWorld[0][0]);
		glUniformMatrix4fv(uView, 1, GL_FALSE, &Window::V[0][0]);

		glBindVertexArray(VAO);
		//glDrawElements(GL_TRIANGLES, points.size() / 3, GL_UNSIGNED_INT, 0);
		glDrawArrays(GL_LINES, 0, points.size());
		glBindVertexArray(0);
}

void Sphere::update() {

}