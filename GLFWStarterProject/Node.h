#pragma once
#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
// Use of degrees is deprecated. Use radians instead.
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Node
{
public:
	Node();
	Node(char * node_name);
	virtual ~Node();
	virtual void draw(glm::mat4 C) = 0;
	virtual void update() = 0;

	char * name;
	Node * parent;

	void setName(char * new_name);
};

