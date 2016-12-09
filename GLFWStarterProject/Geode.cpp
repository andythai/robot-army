#include "Geode.h"

Geode::Geode()
{
}

Geode::Geode(char * geode_name) {
	name = geode_name;
}

Geode::~Geode()
{
}

void Geode::draw(glm::mat4 C) {
	//glMatrixMode(GL_MODELVIEW);
	//glLoadMatrixf(&C[0][0]);
	toWorld = C;
	render();
}