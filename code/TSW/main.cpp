#include <iostream>
#include <fstream>
#include <random>
#include <vector>
#include <ctime>
#include <algorithm>
#include <unordered_map>
#include "util.h"
#include "Sample.h"
#include "hashfunction.h"
// #include "def.h"

using namespace std;

// #define CLOCKS_PER_SEC ((clock_t)1000000)

struct record
{
    long long st;
    long long ed;
    long double bfc;
    record(long long s, long long e, long double b) : st(s), ed(e), bfc(b) {}
};

int main(int argc, const char * argv[])
{
    for(auto fname : vector<string>{"gottron"}){
    unsigned int s, d;
    long long tstmp;
    // double time_unit = 18.545; // 时间单位，(maximum timestamp - minimum timestamp)/number_of_edges
    // double time_unit = 1.0;
    string buf;
    stringstream ss;
#ifdef TSTMP
    ifstream fin("/home/zzuo/bgdata/"+fname+"/out2_tstmp."+fname+"");
    vector<tuple<int, int, int>> edges;
#else
    ifstream fin("/home/zzuo/bgdata/"+fname+"/out2."+fname+"");
    vector<pair<int, int>> edges;
#endif
    while (!fin.eof())
    {
        getline(fin, buf);
        ss.clear();
        ss << buf;
        if (!(ss >> s))
        {
            continue;
        }
        if (!(ss >> d))
        {
            continue;
        }
#ifdef TSTMP
        if(!(ss >> tstmp)){
            continue;
        }
        edges.push_back(make_tuple(s + 1, d + 1 + left_size, tstmp));
#else
        edges.push_back(make_pair(s + 1, d + 1 + left_size));
#endif
    }
#ifndef TSTMP
    random_shuffle(edges.begin(), edges.end());
#endif

    cout << "edge_size: " << edges.size() << endl;

    // vector<ld> gt;
    long wsize = 20000000;
    // for (int wsize = 10000000; wsize <= 20000000; wsize += 2000000)
    for (int hindex = 3; hindex < 4 /*5*/; hindex++)
    { // hindex: hash_index, 0~4
        // vector<ld> gt;
        // for (int hindex = 0; hindex < 5 /*5*/; hindex++)
        // for(int sample_size = 100000; sample_size <= 600000; sample_size += 100000)
        int sample_size = 50000;
        while(sample_size<=600000)
        { // 两重循环
            // int sample_size = (0.04*wsize)/2;
            vector<record> rec;
            long long count = 0;
            // set output path
#ifdef TSTMP
            string index = "/home/zzuo/SW-BFC/results/"+fname+"-tstmp/res_"+fname+"_";
#else
            string index = "/home/zzuo/SW-BFC/results/"+fname+"/res_"+fname+"_";
#endif
            index += char(hindex + '0');
            index += '_';
            index += my_to_string(wsize);
            index += '_';
            index += my_to_string(sample_size);
#if ALG == 1
            index += "_clr_";
            index += my_to_string(clr_num);
            index += ".txt";
#elif ALG == 2
            index += "_vtx";
            index += ".txt";
#else
            index += ".txt";
#endif
            cout << index << endl;
            ofstream fout(index.c_str());
            Sample *sc = new Sample(sample_size, wsize, hindex, 1000);

            long long t0 = -1;
            long long tmp_point = 0;
            int checkpoint = wsize / 4;

            // clock_t st = clock();
            cout<<edges.size()<<endl;
            for(int edge_index=0;edge_index<edges.size();edge_index++)
            {
                auto edge = edges[edge_index];
#ifdef TSTMP
                s = get<0>(edge);
                d = get<1>(edge);
                tstmp = get<2>(edge);
#else
                // t = clock();
                // if(t0<0) {t0 = t;}
                s = edge.first;
                d = edge.second;
                if (s != d)
                {
                    // count = (t - t0)/CLOCKS_PER_SEC/time_unit;
                    count = count + 1;
                }
                else
                {
                    continue;
                }
                tstmp = count;
#endif
#if ALG == 1
                string s_str = to_string(s);
                string d_str = to_string(d);
                int color1 = Hsieh((const unsigned char *)s_str.c_str(), s_str.length()) % clr_num;
                int color2 = Hsieh((const unsigned char *)d_str.c_str(), d_str.length()) % clr_num;
                // cout<<color1<<" "<<color2<<endl;
                if (color1 == color2)
                {
                    sc->proceed(s, d, tstmp);
                }else{
                    sc->proceed_without_edge(tstmp);
                }
#elif ALG == 2
                string s_str = to_string(s);
                int hash_value = Hsieh((const unsigned char *)s_str.c_str(), s_str.length()) % mod;
                if (hash_value <= up){
                    sc->proceed(s, d, tstmp);
                }else{
                    sc->proceed_without_edge(tstmp);
                }
#else
                sc->proceed(s, d, tstmp);
#endif
                assert(tstmp==count);
                if (tstmp >= wsize && int(tstmp / checkpoint) > tmp_point)
                {
                    srand((int)time(0));
                    tmp_point = tstmp / checkpoint;
                    sc->prepare(tstmp);
                    fout << "swbfc butterfly " << sc->st->valid_num << ' ' << sc->edge_estimate << ' ' << sc->st->bfcount << ' ' << sc->count() << endl;
                    cout << "check point " << tmp_point << " " << tstmp << " " << checkpoint << endl;
                    rec.push_back(record(count-wsize, count, sc->count()));
                }
            }
            // clock_t ed = clock();
            // clock_t seconds = (ed-st)/CLOCKS_PER_SEC;

            // cout<<count<<" "<<wsize<<endl;
            fin.close();
            fout.close();
            delete sc;
            if(sample_size==50000){
                sample_size=100000;
            }else{
                sample_size += 100000;
            }

            // eval
// #ifdef TSTMP
//             string index2 = "/home/zzuo/SW-BFC/results/"+fname+"-tstmp/eval_"+fname+"_";
// #else 
//             string index2 = "/home/zzuo/SW-BFC/results/"+fname+"/eval_"+fname+"_";
// #endif
//             index2 += char(hindex + '0');
//             index2 += '_';
//             index2 += my_to_string(wsize);
//             index2 += '_';
//             index2 += my_to_string(sample_size);
// #if ALG == 1
//             index2 += "_clr_";
//             index2 += my_to_string(clr_num);
//             index2 += ".txt";
// #elif ALG == 2
//             index2 += "_vtx";
//             index2 += ".txt";
// #else
//             index2 += ".txt";
// #endif
//             cout << index2 << endl;
//             ofstream fout2(index2.c_str());

//             if(gt.empty()){
//                 for (int i=0;i<rec.size();i++){
//                     cout<<"i"<<i<<endl;
//                     record r = rec[i];
//                     vector<pair<int, int>> win_edges;
//                     cout<<r.st<<" "<<r.ed-1<<endl;
//                     for (int k = r.st; k <= r.ed-1; k++)
//                     {
//                         // cout<<"k"<<k<<endl;
//                         win_edges.push_back(make_pair(get<0>(edges[k]), get<1>(edges[k])));
//                     }
//                     ld exact_b = batch_bfly_count(win_edges);
//                     gt.push_back(exact_b);
//                 }
//             }
//             for (int i=0;i<rec.size();i++)
//             {
//                 record r = rec[i];
//                 ld exact_b = gt[i];
//                 ld error = abs(exact_b - r.bfc) / exact_b * 100;
//                 fout2 << exact_b << " " << error << endl;
//             }
        }
    }
    }
    return 0;
}

/*while(!fin.eof()){
    getline(fin, buf);
    // cout<<buf<<endl;
    ss.clear();
    ss << buf;
    if(!(ss>>s)){
        continue;
    }
    if(!(ss>>d)){
        continue;
    }
    //cout<<s<<" "<<d<<endl;
    t = clock();
    if(t0<0) {t0 = t;}
    s = s + 1;
    d = d + 1 + left_size;
    if (s != d){
        count = (t - t0)/CLOCKS_PER_SEC/time_unit;
    }else{
        continue;
    }
    num++;
    // cout<<s<<" "<<d<<" "<<count<<endl;
    sc->proceed(s, d, count);
    if (count>=2*wsize && int(count/checkpoint) > tmp_point)	// whenever the window
    {
        srand((int)time(0));
        tmp_point = count/checkpoint;
        sc->prepare();
        fout<<"swbfc butterfly "<<sc->st->valid_num<<' '<<sc->edge_estimate<<' '<<sc->st->bfcount<<' '<<sc->count()<<endl;
        fout<<endl;
        cout<<sample_size<<" check point " << tmp_point << endl;
    }
}*/