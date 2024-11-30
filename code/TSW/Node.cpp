#include "Node.h"
void Node::init(unsigned int s, int edge)
{
    nodeID = s;
    next = NULL;
    first_edge = edge;
    local_count = 0;
    vision_count = 0;
}

void Node::set_first_edge(int e)
{
    first_edge = e;
}

void Node::reset()
{
    nodeID = 0;
    first_edge = -1;
    local_count = 0;
    vision_count = 0;
}

