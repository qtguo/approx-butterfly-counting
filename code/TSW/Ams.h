#ifndef AMS_H
#define AMS_H
#include <stdlib.h>
#include <stdio.h>
class Ams{
private:
    int **a_hash;
    int **b_hash;
public:
    int **hash_map;
    int *hash_seed;
    int *stat;
    long long k, l, p = 4294967291;
    unsigned long long x, hash_result;
    long long f2;
    int seed;
    long bucket_seed_a, bucket_seed_b;

    Ams();
    Ams(unsigned long long k, unsigned long long l);
    int hashline(long u, long v);
    unsigned long long magic_hash(unsigned long long a, int seed);
    void update(long u, long v, int op);
    unsigned long long query();
    // unsigned long long count();
    unsigned long long MurmurHash64A(const void * key, int len, int seed);
};

#endif
