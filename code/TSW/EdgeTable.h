#include<iostream>
#include<string>
#include<vector>
#include<map>
#include "Edge.h"

class EdgeTable
{
public:
    int size;
    int tsl_head, tsl_tail, expiration; // mark the head and the tail of the list. expiration is only used in BPS-sample, namely the baseline method. As in this method there are two kinds
    // of edges, expired but not double expired edges, and not expired edges. We need a pointer to mark the begin of the not expired edges.
    Edge* table;

    EdgeTable(int s)
    {
        size = s;
        table = new Edge[s];
        tsl_head = -1;
        tsl_tail = -1;
        expiration = -1;
        for (int i = 0; i<s; i++)
        {
            table[i].reset();
            table[i].vice.reset();
        }
    }
    ~EdgeTable()
    {
        delete[]table;
    }

    void set_time_list(int pos, int type, int aim); // type == 0 means prev pointer, and type == 1 means next pointer

    void replace_sample(unsigned int s_num, unsigned int d_num, double p, long long time, int pos); // replace the sample edge at pos with a new edge;

    void replace_vice(unsigned int s_num, unsigned int d_num, double p, long long time, int pos); // replace the vice edge at pos with a new edge;

    void delete_sample(int pos); // delete the sample edge in the pos

    void delete_vice(int pos);

    void update_sample(int pos, long long time);

    void update_vice(int pos, long long time);
};
