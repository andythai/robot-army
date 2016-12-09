#pragma once
#include "Group.h"
#include "Cube.h"
#include "Sphere.h"
#include "MatrixTransform.h"

class Robot :
	public Group
{
public:
	Robot();
	~Robot();

	Sphere * sphere = new Sphere();
	Cube * cube;

	MatrixTransform head();
	MatrixTransform * left_arm = new MatrixTransform();
	MatrixTransform right_arm();
	MatrixTransform left_leg();
	MatrixTransform right_leg();

};

