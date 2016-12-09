#include "Robot.h"

Robot::Robot()
{
	glm::mat4 left_arm_transform = glm::translate(glm::mat4(1.0f), glm::vec3(-3.0f, 0.0f, 0.0f)) * glm::mat4(1.0f);
	left_arm_transform = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 3.0f, 1.0f)) * left_arm_transform;
	left_arm->setMatrix(left_arm_transform);
	left_arm->addChild(cube);
	addChild(left_arm);
}


Robot::~Robot()
{
}
