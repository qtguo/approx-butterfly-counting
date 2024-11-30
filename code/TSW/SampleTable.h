#include <iostream>
#include <vector>
#include <assert.h>
#include <math.h>
#include <unordered_map>
#include "EdgeTable.h"
#include "NodeTable.h"
#include "Ams.h"
#include "def.h"
#include "exact_alg.h"
#include "BiGraph.h"
using namespace std;

extern const long long left_size;

class SampleTable
{
public:
    EdgeTable *edge_table;
    NodeTable *node_table;
    int size;
    int edge_count;
    int node_count;
    int valid_num;
    int illusion_valid;
    long double bfcount;
    long double illusion;
    double q_count;
    double illusion_q;
    int ams_size;
    int l;
#ifdef MT
#ifndef AMS
    vector<map<unsigned int, long double>> rec;
    vector<map<unsigned int, long double>> illusion_rec;
#endif
    Ams *ams;
    Ams *illusion_ams;
    long double f2;
    long double illusion_f2;
    long double coaffiliation_count;
    long double illusion_coaffiliation_count;
#endif

    explicit SampleTable(int s, int total_ams_size)
    {
        size = s;
        edge_count = 0;
        node_count = 0;
        valid_num = 0;
        bfcount = 0;
        illusion = 0;
        edge_table = new EdgeTable(s);
        node_table = new NodeTable(4, 2 * s);
        q_count = size;
        illusion_q = q_count;
        illusion_valid = 0;
#ifdef MT
        f2 = 0;
        coaffiliation_count = 0;
        illusion_f2 = 0;
        illusion_coaffiliation_count = 0;
#ifdef AMS
        ams_size = total_ams_size / 2;
        cout<<"ams_size: "<<ams_size<<endl;
        l = 2;
        ams = new Ams(ams_size / l, l);
        illusion_ams = new Ams(ams_size / l, l);
#else
        rec.resize(left_size + 1);
        illusion_rec.resize(left_size + 1);
#endif
#endif
    }
    ~SampleTable()
    {
        delete edge_table;
        delete node_table;
    }

    void modify_butterfly(Node *pos_s, Node *pos_d, long long last_mark, int op, long long w_time, int pos); // op = 1 means add, op = -1 means minus
    void modify_butterfly_illusion(Node *pos_s, Node *pos_d, long long landmark, int op);
    void modify_butterfly_both(Node *pos_s, Node *pos_d, long long last_mark, long long land_mark, int op, long long w_time, int pos);

    void link_list(Node *pos_s, Node *pos_d, int pos, unsigned int s_num, unsigned int d_num);

    void dismiss(Node *pos_s, Node *pos_d, int pos);

    void dismiss(unsigned int s, unsigned int d, int pos);

    void insert(unsigned int s_num, unsigned int d_num, double p, long long time, long long land_mark, long long last_mark, int hashindex, long long window_size);

    void update(long long time, long long land_mark, long long last_mark);

    void count(long long time, long long last_mark);

    void ective()
    {
        cout << "ective" << endl;
#ifdef MT
#ifdef AMS
        illusion_f2 = illusion_ams->query();
#endif
        f2 = illusion_f2;
        coaffiliation_count = illusion_coaffiliation_count;
        illusion = (illusion_f2 - illusion_coaffiliation_count) / 2;
#ifdef AMS
        delete ams;
        ams = illusion_ams;
#else
        rec = illusion_rec;
#endif
#endif

        bfcount = illusion;

        valid_num = illusion_valid;
        q_count = illusion_q;
        edge_count = illusion_valid;
        illusion_q = size;
        illusion_valid = 0;
#ifdef MT
        illusion_coaffiliation_count = 0;
        illusion_f2 = 0;
#endif
        illusion = 0;

// coaffiliation_count = 0;
// f2 = 0;
#ifdef MT
#ifdef AMS
        illusion_ams = new Ams(ams_size / l, l);
#else
        illusion_rec.clear();
        illusion_rec.resize(left_size + 1, map<unsigned int, long double>());
#endif
#endif
        node_table->active(); // need to?
    }
};
