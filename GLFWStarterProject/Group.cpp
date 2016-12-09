#include "Group.h"

Group::Group()
{
}

Group::Group(char * group_name) {
	name = group_name;
}

Group::~Group()
{
	for (Node * child : * children) {
		delete child;
	}
	delete children;
}

void Group::setParent(Node * new_parent) {
	parent = new_parent;
}

void Group::addChild(Node * new_child) {
	children->push_back(new_child);
}

void Group::removeChild(Node * to_delete) {
	children->remove(to_delete);
}

void Group::draw(glm::mat4 C) {
	for (Node * child : * children) {
		child->draw(C);
	}
}

void Group::update() {

}