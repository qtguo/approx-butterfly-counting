#include<iostream>
#include<sstream>
#include<string>
#define next_s 0
#define next_d 1
#define last_s 2
#define last_d 3
using namespace std;

class Node
{
public:
    unsigned int nodeID;
    unsigned int local_count;  // not needed in global counting;
    unsigned int vision_count; // not needed in asy_sample and BPS sample

    int first_edge;

    Node* next;

    Node()
    {
        nodeID = 0;
        local_count = 0;
        vision_count = 0;
        next = NULL;
        first_edge = -1;
    }
    Node(unsigned int id, int edge = -1)
    {
        nodeID = id;
        next = NULL;
        first_edge = edge;
        local_count = 0;
        vision_count = 0;
    }
    void init(unsigned int id, int edge = -1);
    void set_first_edge(int e);
    void reset();
};
