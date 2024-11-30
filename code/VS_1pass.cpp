#include <stdio.h>
#include <map>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <random>
#include <unordered_map>  
#include <iostream>
#include "Ams.h"
#include <set>
#include <algorithm>
#include <fstream>
using namespace std;
#define ld long double
#define CMP <

struct Edge{
    long u,v;
    bool valid = true;
    Edge() = default;
    Edge(long u, long v): u(u), v(v) {}
};

vector<Edge> edges;
unordered_map< long, vector<long> > u_map;
unordered_map< long, vector<long> > i_map;
unsigned long long left_co = 0;
unsigned long long right_co = 0;

ld error_percent(const ld exact_n_bf, const ld res, bool to_abs) {
  if (exact_n_bf == 0) return 0;
  ld error = (res - exact_n_bf) / exact_n_bf * 100.0;
  if (to_abs && error < 0) error *= -1.0;
  return error;
}

struct Result{
    Result() = default;

    Result(
            ld error, double elapsed_time, ld res
    ) : error(error), elapsed_time(elapsed_time), res(res) {}
    ld error{0};
    double elapsed_time{0};
    ld res{0};
    bool operator<(const Result &o) const {
        return error < o.error;
    }
};

Result _sparsification_merge_results(vector<Result> &vec, const ld exact_n_bf) {
    Result res;
    for (const auto &item: vec) {
        res.res += (item.res/static_cast<ld>(vec.size()));
        res.elapsed_time += item.elapsed_time;
    }
    // res.res /= static_cast<ld>(vec.size());
    res.error = error_percent(exact_n_bf, res.res, true);
    return res;
}

unsigned long long QueryF(vector<Ams> amses){
    unsigned long size = amses.size();
    unsigned long long res = 0;
    for(auto i=0;i<size;i++){
        // cout<<amses[i].query()<<" ";
        res += (amses[i].query())/size;
    }
    // cout<<endl;
    return res;
}

ld avg_error(vector<Result> vec){
    ld res = 0;
    ld len = (ld)vec.size();
    for(auto r:vec){
        res += r.error/len;
    }
    return res;
}   

#define get16bits(d) ((((uint32_t)(((const uint8_t *)(d))[1])) << 8)\
                       +(uint32_t)(((const uint8_t *)(d))[0]) )
                       
unsigned int Hsieh (const unsigned char * str, unsigned int len)
{
    unsigned int hash = len, tmp;
    int rem;

    //if (len <= 0 || str == NULL) return 0;

    rem = len & 3;
    len >>= 2;

    /* Main loop */
    for (;len > 0; len--) {
        hash  += get16bits (str);
        tmp    = (get16bits (str+2) << 11) ^ hash;
        hash   = (hash << 16) ^ tmp;
        str  += 2*sizeof (uint16_t);
        hash  += hash >> 11;
    }

    /* Handle end cases */
    switch (rem) {
        case 3: hash += get16bits (str);
            hash ^= hash << 16;
            hash ^= ((signed char)str[sizeof (uint16_t)]) << 18;
            hash += hash >> 11;
            break;
        case 2: hash += get16bits (str);
            hash ^= hash << 11;
            hash += hash >> 17;
            break;
        case 1: hash += (signed char)*str;
            hash ^= hash << 10;
            hash += hash >> 1;
    }

    /* Force "avalanching" of final 127 bits */
    hash ^= hash << 3;
    hash += hash >> 5;
    hash ^= hash << 4;
    hash += hash >> 17;
    hash ^= hash << 25;
    hash += hash >> 6;

    return hash;
}


#define mix(a,b,c) \
{ \
  a -= b; a -= c; a ^= (c>>13); \
  b -= c; b -= a; b ^= (a<<8); \
  c -= a; c -= b; c ^= (b>>13); \
  a -= b; a -= c; a ^= (c>>12);  \
  b -= c; b -= a; b ^= (a<<16); \
  c -= a; c -= b; c ^= (b>>5); \
  a -= b; a -= c; a ^= (c>>3);  \
  b -= c; b -= a; b ^= (a<<10); \
  c -= a; c -= b; c ^= (b>>15); \
}

unsigned int BOB1(const unsigned char * str, unsigned int len)
{
    //register ub4 a,b,c,len;
    unsigned int a,b,c;
    unsigned int initval = 2;
    /* Set up the internal state */
    //len = length;
    a = b = 0x9e3779b9;  /* the golden ratio; an arbitrary value */
    c = initval;         /* the previous hash value */

    /*---------------------------------------- handle most of the key */
    while (len >= 12)
    {
        a += (str[0] +((unsigned int)str[1]<<8) +((unsigned int)str[2]<<16) +((unsigned int)str[3]<<24));
        b += (str[4] +((unsigned int)str[5]<<8) +((unsigned int)str[6]<<16) +((unsigned int)str[7]<<24));
        c += (str[8] +((unsigned int)str[9]<<8) +((unsigned int)str[10]<<16)+((unsigned int)str[11]<<24));
        mix(a,b,c);
        str += 12; len -= 12;
    }

    /*------------------------------------- handle the last 11 bytes */
    c += len;
    switch(len)              /* all the case statements fall through */
    {
        case 11: c+=((unsigned int)str[10]<<24);
        case 10: c+=((unsigned int)str[9]<<16);
        case 9 : c+=((unsigned int)str[8]<<8);
            /* the first byte of c is reserved for the length */
        case 8 : b+=((unsigned int)str[7]<<24);
        case 7 : b+=((unsigned int)str[6]<<16);
        case 6 : b+=((unsigned int)str[5]<<8);
        case 5 : b+=str[4];
        case 4 : a+=((unsigned int)str[3]<<24);
        case 3 : a+=((unsigned int)str[2]<<16);
        case 2 : a+=((unsigned int)str[1]<<8);
        case 1 : a+=str[0];
            /* case 0: nothing left to add */
    }
    mix(a,b,c);
    /*-------------------------------------------- report the result */
    return c;
}

unsigned long long MurmurHash64A (const void * key, int len, int seed){

    const unsigned long long m = 0xc6a4a7935bd1e995;
    const int r = 47;
    unsigned long long h = seed ^ (len * m);
    const unsigned long long * data = (const unsigned long long *)key;
    const unsigned long long * end = data + (len/8);
    while(data != end){
        unsigned long long k = *data++;
        k *= m;
        k ^= k >> r;
        k *= m; 
        h ^= k;
        h *= m;
    }
    const unsigned char * data2 = (const unsigned char*)data;
    switch(len & 7){
        case 7: h ^= uint64_t(data2[6]) << 48;
        case 6: h ^= uint64_t(data2[5]) << 40;
        case 5: h ^= uint64_t(data2[4]) << 32;
        case 4: h ^= uint64_t(data2[3]) << 24;
        case 3: h ^= uint64_t(data2[2]) << 16;
        case 2: h ^= uint64_t(data2[1]) << 8;
        case 1: h ^= uint64_t(data2[0]);
        h *= m;
    }
    h ^= h >> r;
    h *= m;
    h ^= h >> r;
    return h;
}


int main(int argc, char *argv[])
{

    char *fileread = argv[1];
    FILE *fr = fopen(fileread, "r");
    printf("fileread: %s\n", fileread);
    char *filewrite = argv[2];
    ofstream rout;
    // FILE *fw = fopen(filewrite, "w");
    printf("filewrite: %s\n", filewrite);
    double sample_rate = atof(argv[3]);
    printf("start_sample_rate: %lf\n", sample_rate);
    double fraction = atof(argv[4]);
    printf("fraction: %lf\n", fraction);
    ld gt = atof(argv[5]);
    printf("ground truth: %Lf\n", gt);
    long uID, iID;

    while(fscanf(fr, "%ld\t%ld\n", &uID, &iID) == 2){
        edges.push_back(Edge(uID, iID));
        u_map[uID].push_back(iID);
        i_map[iID].push_back(uID);
        left_co += u_map[uID].size() - 1;
        right_co += i_map[iID].size() - 1; 
    }
    fclose(fr);
   
    // std::cout << left_co << " " << right_co << endl;
    std::cout<<"left_vn:"<<u_map.size()<<"right_vn:"<<i_map.size()<<endl;

    random_shuffle(edges.begin(),edges.end());
    int edge_num = edges.size();
    // std::cout << edge_num << endl;

//    vector<double> sample_rates = {/*1e-6, 5e-6,*/ 0.00001, 0.00005, 0.0001, 0.0005, 0.001, 0.0025, 0.005, 0.0075, 0.01};
//    vector<double> sample_rates = {0.0004, 0.0005, 0.0006};
    vector<double> sample_rates = {125e-9,25e-8,5e-7,1e-6,2e-6,4e-6,8e-6,16e-6,32e-6,64e-6,128e-6/*,256e-6,512e-6,1024e-6*//*,2048e-6,4096e-6,8192e-6,16384e-6,32768e-6,65536e-6*/};
    int iter_num = 100;

    random_device ams;
    mt19937 mt(ams());
    vector<int> hash_seeds(iter_num);
    for(auto i=0;i<hash_seeds.size();i++){
        hash_seeds[i] = mt();
    }

    rout.open(filewrite, ios::app);
    for(auto s:sample_rates){
        // cout<<s<<endl;
        int ams_size = 1;
        sample_rate = s;
        int memory = int(2 * sample_rate * edge_num);
        if(memory==0){ continue;}
        fraction = double(ams_size)/1.0;
        int data_k = int(fraction * memory / (1.0f + fraction) / ams_size);
        if(data_k<2){ continue;}
        double sample = double(memory - data_k*ams_size) / double(edge_num);
        cout<<"memory:"<<memory<<endl;
        cout<<"data_k:"<<data_k<<endl;
        cout<<"sample_rate:"<<sample_rate<<" sample:"<<sample<<endl;

        random_device cas;
        mt19937 sam(cas());
        uniform_real_distribution<double> dis(0.0, 1.0);
        
        vector<Result> results(iter_num);
        for(int i=0;i<iter_num;i++){
            // int time = 0;
            int size = 0;
            int count = 0;
            ld butterfly_count = 0;
            ld co_affiliation_count = 0;
            ld f2 = 0;
            unordered_map< long, vector<long> > item_map;
            vector<Ams> amses(ams_size);
            for(int as=0;as<ams_size;as++){
                amses[as] = Ams(data_k/2, 2);
            }
            // Ams ams(data_k/2, 2);
            unordered_map<long, double> pmap;

            clock_t st = clock();
            // first pass
            random_shuffle(edges.begin(),edges.end());
            for( auto &edge : edges )
            {
                if(left_co CMP right_co){
                    uID = edge.u;
                    iID = edge.v;
                }
                else{
                    iID = edge.u;
                    uID = edge.v;    
                }
                if(item_map.find(iID) != item_map.end())
                {
                    vector<long>::iterator item_it = item_map[iID].begin();
                    for(; item_it != item_map[iID].end(); item_it++)
                    {
                        count++;
                        if(uID != *item_it)
                        {
                            long max_userID = (uID > *item_it) ? uID : *item_it;
                            long min_userID = (uID < *item_it) ? uID : *item_it;

                            co_affiliation_count ++;
                            for(int as=0;as<ams_size;as++){
                                amses[as].update(min_userID, max_userID);
                            }
                        }
                    }
                }

                // string s = to_string(uID);
                // double p = double(Hsieh((const unsigned char*)s.c_str(), s.length()) % 100000000) / (100000000-1);
                // if(p <= sample){
                //     size++;
                //     item_map[iID].push_back(uID);
                //     edge.valid = false;
                // }

                if(pmap.find(uID)==pmap.end()){
                    double p = dis(sam);
                    pmap[uID] = p;
                    if(p <= sample)
                    {
                        size++;
                        item_map[iID].push_back(uID);
                        edge.valid = false;
                    }
                }else{
                    if(pmap[uID] <= sample)
                    {
                        size++;
                        item_map[iID].push_back(uID);
                        edge.valid = false;
                    }
                }
            }

            // cout<<"time:"<<time<<endl;
            // cout<<"size:"<<size<<endl;
            // cout<<"count:"<<count<<endl;
            clock_t ed = clock();
            f2 = QueryF(amses);
            butterfly_count = (f2 - co_affiliation_count) / 2.0;
            // cout<<sample<<" "<<(1-sample);
            long double bfc_est = (long double)butterfly_count / 0.5 / sample / (sample+1.0);
            // long double bfc_est = (long double)butterfly_count / sample;
            double elapsed_time = double(ed - st) / CLOCKS_PER_SEC;
            long double error = error_percent(gt, bfc_est, true);
            std::cout<<item_map.size()<<" "<<f2<<" "<<co_affiliation_count<<" "<<bfc_est<<" "<<error<<endl;
            // rout<<s<<" "<<elapsed_time<<" "<<" "<<bfc_est<<" "<<error<<endl;
            results[i] = {error, elapsed_time, bfc_est};
        }
        Result avg = _sparsification_merge_results(results, gt);
        rout<<"avg: "<<s<<" "<<avg.elapsed_time<<" "<<avg.error<<endl;

        // sort(results.begin(),results.end());
        // Result mid_r = results[(iter_num-1)/2];
        // Result min_r = results[0];
        // Result max_r = results[results.size()-1];
        // ld avg_r = avg_error(results);
        // rout<<"mid: "<<s<<" "<<mid_r.elapsed_time<<" "<<mid_r.res<<" "<<mid_r.error<<endl;
        // rout<<"min: "<<s<<" "<<min_r.elapsed_time<<" "<<min_r.res<<" "<<min_r.error<<endl;
        // rout<<"max: "<<s<<" "<<max_r.elapsed_time<<" "<<max_r.res<<" "<<max_r.error<<endl;
        // rout<<"avg: "<<s<<" "<<avg_r<<endl;
    }
    // fclose(fw);
    rout.close();
    return 0;
}
