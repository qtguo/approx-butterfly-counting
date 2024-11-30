#include<iostream>
#include<sstream>
#include<string>
using namespace std;

class candidate_unit // class for the candidate edge or test edge in each substream, used in both BPS and SWTC. In BPS, it is the expired but not double-expired test edge.
    //In SWTC, it is the not chosen edge in the 2 slices. To be precise, in one case, it is the edge with the largest priority in the last slice, but expires and servers as a test edge (case 2 in the paper).
    //In the other case, it the edge with the largest priority in current slice,
{
public:
    unsigned int s, d; // nodes of the edge
    double priority; // can be computed according to s and d. saved only for convinience
    long long timestamp;
    int time_list_prev; // suppose the size of the sample table is m. A pointer in value range 0 ~ m-1 means sample unit in the corresponding table pos.
    //A pointer in value range m ~ 2m-1 means candidate unit in the corresponding table pos. -1 means an empty pointer.
    int time_list_next;

    candidate_unit(unsigned int snum = 0, unsigned int dnum = 0, double p = -1, long long time = -1, int prev = -1, int next = 1)
    {
        s = snum;
        d = dnum;
        priority = p;
        timestamp = time;
        time_list_prev = prev;
        time_list_next = next;
    }
    void reset(unsigned int snum = 0, unsigned int dnum = 0, double p = -1, long long time = -1, int prev = -1, int next = -1)
    {
        s = snum;
        d = dnum;
        priority = p;
        timestamp = time;
        time_list_prev = prev;
        time_list_next = next;
    }
};