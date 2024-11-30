#include "NodeTable.h"

Node* NodeTable::insert(unsigned long node, int edge)
{
    int min_length = 0x7FFFFFFF;
    int min_pos = -1;
    int empty_pos = -1;

    string node_s = my_to_string(node);

    for (int i = 0; i<k; i++)
    {
        unsigned int pos = (*hfunc[i])((unsigned char*)(node_s.c_str()), node_s.length()) % slice_length;
        int address = i*slice_length + pos;


        if (table[address].nodeID == 0)
        {
            if (empty_pos<0)
                empty_pos = address;
        }

        if (table[address].nodeID == node)
        {
            if (edge>0)
                table[address].set_first_edge(edge);
            return &(table[address]);
        }
        else
        {
            int l = 0;
            Node* tmp = table[address].next;
            while (tmp)
            {
                l++;
                if (tmp->nodeID == node)
                {
                    if (edge>0)
                        tmp->set_first_edge(edge);
                    return tmp;
                }
                tmp = tmp->next;
            }
            if (l<min_length)
            {
                min_length = l;
                min_pos = address;
            }
        }
    }

    if (empty_pos>=0)
    {
        table[empty_pos].nodeID = node;
        table[empty_pos].set_first_edge(edge);
        return &(table[empty_pos]);
    }
    Node* tmp = table[min_pos].next;
    if (!tmp)
    {
        tmp = new Node(node, edge);
        table[min_pos].next = tmp;
    }
    else
    {
        Node* last = tmp;
        while (tmp)
        {
            last = tmp;
            tmp = tmp->next;
        }
        tmp = new Node(node, edge);
        last->next = tmp;
    }
    return tmp;
}

Node* NodeTable::set_edge(unsigned long node, int edge)
{
    string node_s = my_to_string(node);
    for (int i = 0; i<k; i++)
    {
        unsigned int pos = (*hfunc[i])((unsigned char*)(node_s.c_str()), node_s.length()) % slice_length;
        int address = i*slice_length + pos;

        if (table[address].nodeID == node)
        {
            table[address].set_first_edge(edge);
            return &(table[address]);
        }

        else
        {
            Node* tmp = table[address].next;
            while (tmp)
            {
                if (tmp->nodeID == node)
                {
                    tmp->set_first_edge(edge);
                    return tmp;
                }
                tmp = tmp->next;
            }
        }
    }

    return NULL;
}

int NodeTable::get_edge(unsigned long node)
{
    string node_s = my_to_string(node);
    for (int i = 0; i<k; i++)
    {
        unsigned int pos = (*hfunc[i])((unsigned char*)(node_s.c_str()), node_s.length()) % slice_length;
        int address = i*slice_length + pos;

        if (table[address].nodeID == node)
            return table[address].first_edge;
        else
        {
            Node* tmp = table[address].next;
            while (tmp)
            {
                if (tmp->nodeID == node)
                    return tmp->first_edge;
                tmp = tmp->next;
            }
        }
    }

    return -1;
}

Node* NodeTable::ID_to_pos(unsigned long node)
{
    string node_s = my_to_string(node);
    for (int i = 0; i<k; i++)
    {
        unsigned int pos = (*hfunc[i])((unsigned char*)(node_s.c_str()), node_s.length()) % slice_length;
        int address = i*slice_length + pos;
        if (table[address].nodeID == node)
            return &(table[address]);
        else
        {
            Node* tmp = table[address].next;
            while (tmp)
            {
                if (tmp->nodeID == node)
                    return tmp;
                tmp = tmp->next;
            }
        }
    }
    return NULL;
}

void NodeTable::delete_via_ID(unsigned long node)
{
    string node_s = my_to_string(node);
    for (int i = 0; i<k; i++)
    {
        unsigned int pos = (*hfunc[i])((unsigned char*)(node_s.c_str()), node_s.length()) % slice_length;
        int address = i*slice_length + pos;

        if (table[address].nodeID == node)
        {
            if (table[address].next)
            {
                Node* cur = table[address].next;
                table[address].first_edge = cur->first_edge;
                table[address].next = cur->next;
                table[address].nodeID = cur->nodeID;
                delete cur;
            }
            else
                table[address].reset();
            return;
        }

        else
        {
            Node* tmp = table[address].next;
            Node* last = tmp;
            while (tmp)
            {
                if (tmp->nodeID == node)
                {
                    if (last == tmp)
                    {
                        table[address].next = tmp->next;
                        delete tmp;
                    }
                    else
                    {
                        last->next = tmp->next;
                        delete tmp;
                    }
                    return;
                }
                last = tmp;
                tmp = tmp->next;
            }
        }
    }
    return;
}

void NodeTable :: active()  // only used in SWTC with vision counting, and in local counting semantics
{
    for (int i = 0; i < length; i++)
    {
        table[i].local_count = table[i].vision_count;
        table[i].vision_count = 0;
        Node* tmp = table[i].next;
        while (tmp)
        {
            tmp->local_count = tmp->vision_count;
            tmp->vision_count = 0;
            tmp = tmp->next;
        }
    }
}
