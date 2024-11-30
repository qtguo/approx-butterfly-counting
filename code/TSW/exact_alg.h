#include <iostream>
#include <vector>
#include "BiGraph.h"
using ld = long double;
using ll = long long;
using vertex_t = int;
using namespace std;

ld bfc_exact(const BiGraph &graph);
long long batch_bfly_count(std::vector<std::pair<int, int>> &edge_vec);
