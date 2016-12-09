#include "Node.h"

Node::Node()
{
	name = "";
}

Node::Node(char * node_name) {
	name = node_name;
}

Node::~Node()
{
}

void Node::setName(char * new_name) {
	name = new_name;
}