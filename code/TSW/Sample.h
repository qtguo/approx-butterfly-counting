#include<iostream>
#include<string>
#include<vector>
#include<unordered_map>
#include<map>
#include<assert.h>
#include "SampleTable.h"


using namespace std;

class Sample
{
public:
    SampleTable* st;
    int window_size;
    long long current_time;
    long long land_mark;
    long long last_mark;
    int edge_estimate;
    double sample_prob;
    int hashindex;

    Sample(int size, int w, int hi, double ratio)
    {
        st = new SampleTable(size, ratio*size);
        window_size = w;
        current_time = 0;
        land_mark = 0;
        last_mark = 0;
        hashindex = hi;
    }
    ~Sample()
    {
        delete st;
    }

    void proceed(unsigned int s, unsigned int d, long long time);
    void proceed_without_edge(long long time);
    void prepare(long long time);

    long double count()
    {
        return st->bfcount / sample_prob;
    }

    int local_count(unsigned int v)
    {
        Node* tmp = st->node_table->ID_to_pos(v);
        if (!tmp)
            return 0;
        else
            return (tmp->local_count) / sample_prob;
    }

    void all_local(unordered_map<unsigned int, int>& cr)
    {
        for(int i=0;i<st->node_table->length;i++)
        {
            cr[st->node_table->table[i].nodeID] = (st->node_table->table[i].local_count) / sample_prob;
            Node* tmp = st->node_table->table[i].next;
            while(tmp)
            {
                cr[tmp->nodeID] = (tmp->local_count) / sample_prob;
                tmp = tmp->next;
            }
        }
    }
};

