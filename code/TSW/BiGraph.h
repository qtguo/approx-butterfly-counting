#ifndef BI_GRAPH
#define BI_GRAPH

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <random>
#include <bitset>
using ll = long long;
using ld = long double;

using namespace std;

enum Side {
    NO_CARE, L_SIDE, R_SIDE, BOTH
};

template <typename vertex_t>
struct Edge2 {
    Edge2() = default;
    Edge2(vertex_t u, vertex_t v): u(u), v(v) {}

    bool operator < (const Edge2 &o) const {
        if (u == o.u) return v < o.v;
        return u < o.u;
    }

    friend std::ostream &operator <<(std::ostream &out, const Edge2 &e) {
        out << "(" << e.u << ", " << e.v << ")";
        return out;
    }
    vertex_t u, v;
    bool valid = true;
};
using vertex_t = int;
using edge_t = Edge2<vertex_t>;

namespace std{
// hash(edge_t) -> size_t
    template<> struct hash<edge_t>{
        size_t operator()(const edge_t &p) const {
            auto res = (static_cast<size_t>(p.u) << (sizeof(int) * 8)) + static_cast<size_t>(p.v);
            return res;
        }
    };
// equal_to(edge_t,edge_t) -> bool
    template<> struct equal_to<edge_t>{
        bool operator()(const edge_t &p1, const edge_t &p2) const {
            return p1.u == p2.u && p1.v == p2.v;
        }
    };
}

class BiGraph {
public:
    BiGraph() = default;
    BiGraph (vector<pair<int,int>> input);
    ~BiGraph() = default;

    void read_graph(vector<pair<int,int>> input);
    void calc_graph();
    void clear();
    void resize();

    bool _swap_flag{false};

    string _alias;

    vertex_t n_vertex{};
    ll n_edges{};
    vertex_t l_vertex_num{};
    vertex_t r_vertex_num{};
    vertex_t largest_index_in[2]{};
    ll n_wedge_in[2]{}; // 0:中间点在l中的wedge的个数; 1:r



    vector <edge_t> edges;
    vector <ll> sum_deg_neighbors; // 每个节点邻居的度数和
    vector <ll> sum_wedges;
    // adjacent lists
    vector <vector <vertex_t> > adj;
    vector <vector <vertex_t> > sampled_adj;

    vector <vector <ld> > ansAll;

    void vertex_range(Side side, vertex_t &l, vertex_t &r) const;
    vertex_t side_vertex_num(Side side) const;
    ll sampled_edge_num() const;

private:
    void add_edge(vertex_t u, vertex_t v);
    void ComputeCore(int core_num, vector<vertex_t> &deg_vec, bool sampled);
    void BuildCsrVec(vector<vertex_t> &nid_oid_vec, vector<vertex_t> &oid_nid_vec, bool sampled);


    std::unordered_map <vertex_t, vertex_t> vertex2id[2]; // 用于左右分别重新编号
    std::unordered_set <edge_t> edges_set;

    vector <vertex_t> index_map;
    vector <vertex_t> index_map2;
    vector <std::pair<int,long>> index_map3;


    // for weighted sample method
    vector <vertex_t> weighted_sample_table;
    vector <vertex_t> J;
    vector <double> q;
    vector <vertex_t> vertex_table;
};
#endif // BI_GRAPH