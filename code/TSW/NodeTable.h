#include<iostream>
#include<string>
#include<vector>
#include<map>
#include "Node.h"
#include "hashfunction.h"
#include "util.h"
// list of Node link list
class NodeTable
{
public:
    Node* table;
    int k; // k hash functions?
    int length;
    int slice_length; // length/k

    NodeTable(int k_num, int l_num) // 4, 2*EdgeTableSize
    {
        k = k_num;
        length = l_num;
        slice_length = length / k;
        table = new Node[length];
        for (int i = 0; i<length; i++)
            table[i].init(0);
    }
    ~NodeTable()
    {
        for (int i = 0; i<length; i++)
        {
            if (table[i].next)
            {
                Node* tmp = table[i].next;
                Node* next = tmp;
                while (tmp)
                {
                    next = tmp->next;
                    delete tmp;
                    tmp = next;
                }
            }
        }
        delete[]table;
    }

    Node* insert(unsigned long node, int edge = -1);

    Node* set_edge(unsigned long node, int edge);

    int get_edge(unsigned long node);

    Node* ID_to_pos(unsigned long node);

    void delete_via_ID(unsigned long node);
    void active();

};

