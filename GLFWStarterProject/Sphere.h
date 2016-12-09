#pragma once
#include "Geode.h"
#include <vector>
#include "../Window.h"

class Sphere :
	public Geode
{
public:
	Sphere();
	~Sphere();

	void draw(GLuint shaderProgram);
	void render();
	void update();

	// These variables are needed for the shader program
	GLuint VBO, VAO;
	GLuint uProjection, uModelview, uView, uModel;

	GLuint segments = 50;
	GLuint radius = 14;

	std::vector<glm::vec3> points;

	glm::mat4 toWorld = glm::mat4(1.0f);
};

