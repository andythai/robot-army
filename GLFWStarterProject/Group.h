#pragma once
#include "Node.h"
#include <list>
class Group :
	public Node
{
public:
	Group();
	Group(char * group_name);
	~Group();

	std::list<Node*> * children = new std::list<Node*>();

	void setParent(Node* new_parent);
	void addChild(Node * new_child);
	void removeChild(Node * to_delete);

	void draw(glm::mat4 C);

	void update();
};

