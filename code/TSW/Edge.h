#include<iostream>
#include<sstream>
#include<string>
#include "CandidateEdge.h"
#define next_s 0
#define next_d 1
#define last_s 2
#define last_d 3
using namespace std;

class Edge  // class for the sampled edge in each substream, used both in SWTC and BPS
{
public:
    unsigned int s, d;
    double priority;
    // long long timestamp;
    int timestamp;
    int pointers[4]; // cross lists for the graph structure;
    int time_list_prev; // same as the pointers in candidate unit;
    int time_list_next;
    candidate_unit vice;
    Edge(unsigned int snum = 0, unsigned int dnum = 0, double p = -1, long long time = -1, int prev = -1, int next = -1)
    {
        s = snum;
        d = dnum;
        for(int i=0;i<4;i++)
            pointers[i] = -1;
        priority = p;
        timestamp = time;
        time_list_prev = prev;
        time_list_next = next;
        vice.reset();
    }
    void set_next_s(int s){pointers[next_s] = s;}
    void set_next_d(int d){pointers[next_d] = d;}
    void set_last_s(int s){pointers[last_s] = s;}
    void set_last_d(int d){pointers[last_d] = d;}
    void reset(unsigned int snum = 0, unsigned int dnum = 0, double p = -1, long long time = -1, int prev = -1, int next = -1){
            s = snum;
            d = dnum;
            for(int i=0;i<4;i++)
                pointers[i] = -1;
            priority = p;
            timestamp = time;
            time_list_prev = prev;
            time_list_next = next;
    }
};