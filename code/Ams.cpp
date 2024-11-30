#include "Ams.h"

#include <stdio.h>
#include <stdlib.h>
#include <random>
#include <iostream>
#include <time.h>
#include <string>
#include <cstring>

using namespace std;

Ams::Ams(){

}

Ams::Ams(unsigned long long k, unsigned long long l){
    this->k = k;
    this->l = l;
    this->hash_seed = new int[this->l];
    this->stat = new int[this->k];
    this->f2 = 0;
    //for(int i = 0; i < this->k; i++)
    //    this->stat[i] = 0;

    random_device ams;
    mt19937 mt(ams());
    uniform_int_distribution<long> seed(0, this->p - 1);
    for(int i = 0; i < this->l; i++)
        this->hash_seed[i] = mt();
    
    this->seed = mt();

    this->bucket_seed_b = seed(mt);
    this->bucket_seed_a = seed(mt);
    while(this->bucket_seed_b == this->bucket_seed_a || this->bucket_seed_a == 0)
        this->bucket_seed_a = seed(mt);

    hash_map = new int*[this->k];
    for(int i = 0; i < this->k; i++){
        hash_map[i] = new int[this->l];
        for(int j = 0; j < this->l; j++)
            hash_map[i][j] = 0;
    }
}

int Ams::hashline(long u, long v){
    unsigned long long U = u;
    unsigned long long V = v;
    this->x = (U << 32) + V; 
    string str = to_string(this->x);
    int len = str.length();    

    int hash_line = MurmurHash64A(str.c_str(), len, this->seed) % this->k;//((magic_hash(this->x, this->seed) * this->bucket_seed_a) + this->bucket_seed_b) % this->p % this->k;//((magic_hash(U, 1) << 32) + magic_hash(V, 1)) % this->k;
    return hash_line;
}

/*
unsigned long long Ams::magic_hash(unsigned long long a, int seed){
    a -= (a<<seed);
    a ^= (a>>17);
    a -= (a<<9);
    a ^= (a<<4);
    a -= (a<<3);
    a ^= (a<<10); 
    a ^= (a>>15);
    return a;
}
*/

unsigned long long Ams::MurmurHash64A (const void * key, int len, int seed){

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

void Ams::update(long u, long v){
    int hash_id = hashline(u, v);
    string str = to_string(this->x);
    int len = str.length();

    long long s = 0;
    for(int i = 0; i < this->l; i++){
        
        this->hash_result = MurmurHash64A(str.c_str(), len, this->hash_seed[i]);

        if(this->hash_result & 1 == 1){
            s += 2*hash_map[hash_id][i] + 1;
            hash_map[hash_id][i] += 1;
        }
        else{
            s += -2*hash_map[hash_id][i] + 1;
            hash_map[hash_id][i] += -1;
        }
            
    }

    this->f2 += s / this->l;
    // if(abs(f2)>6854775807){
    //     cout<<s<<" "<<this->l<<" "<<s/this->l<<" "<<f2<<endl;
    // }
}

unsigned long long Ams::query(){
    // this->f2 = 0;
    // for(int i = 0; i < this->k; i++)
    //    for(int j = 0; j < this->l; j++)
    //        this->f2 += hash_map[i][j] * hash_map[i][j];
    // this->f2 = this->f2 / this->l;
    // for(int i=0;i<this->k;i++){
    //     for(int j=0;j<this->l;j++){
    //         fprintf(fw, "%d ", hash_map[i][j]);
    //     }
    //     fprintf(fw, "\n");
    // }
    // fprintf(fw, "#################\n");
    return this->f2;
}

/*
unsigned long long Ams::count(){
    unsigned long long Count = 0;
    for(int i = 0; i < this->k; i++)
        Count += this->stat[i] * this->stat[i];

    return Count;
}
*/
