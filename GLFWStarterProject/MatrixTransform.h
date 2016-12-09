#pragma once
#include "Group.h"

class MatrixTransform :
	public Group
{
public:
	MatrixTransform();
	MatrixTransform(glm::mat4 set_M);
	~MatrixTransform();

	glm::mat4 M;

	void draw(glm::mat4 C);
	void setMatrix(glm::mat4 new_M);
};

