#pragma once
#include "Node.h"
class Geode :
	public Node
{
public:
	Geode();
	Geode(char * geode_name);
	~Geode();

	void draw(glm::mat4 C);
	virtual void render() = 0;
	GLuint shaderProgram;
	glm::mat4 toWorld = glm::mat4(1.0f);
};

