#include "Sample.h"

void Sample::proceed(unsigned int s, unsigned int d, long long time)
{
    //    if (s < d)
    //    {
    //        unsigned int tmp = s;
    //        s = d;
    //        d = tmp;
    //    }
    string s_string = my_to_string(s);
    string d_string = my_to_string(d);
    string e = s_string + d_string;
    double p = double((*hfunc[hashindex])((const unsigned char *)e.c_str(), e.length()) % 1000000 + 1) / 1000001;
    current_time = time;
    // cout<<"in_update"<<endl;
    st->update(time - window_size, land_mark, last_mark); // delete all the expired edges, set the corresponding vice to illusion edge for now
    // cout<<"out_update"<<endl;
    if (time - land_mark >= window_size)
    {
        assert(time - land_mark < 2 * window_size);
        last_mark = land_mark;
        land_mark = land_mark + window_size;
        st->ective();
        //	cout << time << ' '<<land_mark<<" effected" << endl;
    }
    // cout<<s<<" "<<d<<endl;
    // cout<<"in_insert"<<endl;
    st->insert(s, d, p, time, land_mark, last_mark, hashindex, window_size);
    // cout<<"out_insert"<<endl;
    // cout<<st->f2<<" "<<st->coaffiliation_count<<endl;
}

void Sample::proceed_without_edge(long long time){
    st->update(time - window_size, land_mark, last_mark); // delete all the expired edges, set the corresponding vice to illusion edge for now
    // cout<<"out_update"<<endl;
    if (time - land_mark >= window_size)
    {
        assert(time - land_mark < 2 * window_size);
        last_mark = land_mark;
        land_mark = land_mark + window_size;
        st->ective();
        //	cout << time << ' '<<land_mark<<" effected" << endl;
    }
}

void Sample::prepare(long long time)
{
#ifdef MT
#ifdef AMS
    st->f2 = st->ams->query();
#endif
    st->bfcount = (st->f2 - st->coaffiliation_count) / 2;
    cout << "f2:" << st->f2 << endl;
    cout << "co_count:" << st->coaffiliation_count << endl;
#else
    st->count(time - window_size, last_mark);
#endif
    int m = st->size;
    double alpha = 0.7213 / (1 + (1.079 / m));
    double total_num = (double(alpha * m * m) / (st->q_count));
    int sample_num = st->valid_num;
    if (total_num < 2.5 * m)
    {
        total_num = -log(1 - double(st->edge_count) / m) * m;
    } // ?i
    total_num = total_num * (double(st->valid_num) / st->edge_count);

    edge_estimate = total_num;
    cout << "total_num: " << total_num << endl;
    cout << "sample_num: " << sample_num << endl;

#if ALG == 1
    sample_prob = (double(sample_num) / total_num) * (double(sample_num - 1) / (total_num - 1)) * (double(sample_num - 2) / (total_num - 2)) * (double(sample_num - 3) / (total_num - 3));
    sample_prob = sample_prob / clr_num / clr_num / clr_num;
// sample_prob = (double(sample_num) / (double(total_num) / double(clr_num))) * (double(sample_num - 1) / (double(total_num) / double(clr_num) - 1)) * (double(sample_num - 2) / (double(total_num) / double(clr_num) - 2)) * (double(sample_num - 3) / (double(total_num) / double(clr_num) - 3));
#elif ALG == 2
    sample_prob = (double(sample_num) / total_num) * (double(sample_num - 1) / (total_num - 1)) * (double(sample_num - 2) / (total_num - 2)) * (double(sample_num - 3) / (total_num - 3));
    double vtx_prob = double(up) / double(mod);
    sample_prob = sample_prob * vtx_prob * vtx_prob;
#else
    sample_prob = (double(sample_num) / total_num) * (double(sample_num - 1) / (total_num - 1)) * (double(sample_num - 2) / (total_num - 2)) * (double(sample_num - 3) / (total_num - 3));
#endif
    cout << "sample_prob: " << sample_prob << endl;
}