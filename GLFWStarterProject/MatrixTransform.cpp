#include "MatrixTransform.h"



MatrixTransform::MatrixTransform()
{
	M = glm::mat4(1.0f);
}

MatrixTransform::MatrixTransform(glm::mat4 set_M) {
	M = set_M;
}

MatrixTransform::~MatrixTransform()
{
}

void MatrixTransform::draw(glm::mat4 C) {
	glm::mat4 drawMatrix = C * M;

	for (Node * child : * children) {
		child->draw(drawMatrix);
	}
}

void MatrixTransform::setMatrix(glm::mat4 new_M) {
	M = new_M;
}