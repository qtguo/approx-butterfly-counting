#include "BiGraph.h"
#include <sstream>
#include <ctime>
#include <algorithm>
#include <queue>
#include <iomanip>
#include <unordered_map>

template<typename T>
inline T cn2(T n) {
    return n * (n - 1) / 2;
}

BiGraph::BiGraph(vector<pair<int,int>> input){
    clear();
    read_graph(input);

    resize();
    calc_graph();
    // std::cerr << " End calc the graph!" << std::endl;
    // std::cerr << " n_wedge_in[0]: " << n_wedge_in[0] << ", n_wedges_in[1]: " << n_wedge_in[1] << "\n";
}

void BiGraph::read_graph(vector<pair<int,int>> input) {
    for(auto e:input) {
        vertex_t u=e.first, v=e.second;
        add_edge(u, v);
    }

    n_vertex = l_vertex_num + r_vertex_num;
    n_edges = static_cast<ll>(edges_set.size());
    largest_index_in[0] = l_vertex_num;
    largest_index_in[1] = n_vertex;

    adj.resize(n_vertex, vector<vertex_t>());

    for (const edge_t &edge: edges_set) {
        vertex_t u = vertex2id[0][edge.u];
        vertex_t v = vertex2id[1][edge.v] + l_vertex_num; // 左右编号不重复，r一定大于l
        adj[u].push_back(v);
        adj[v].push_back(u);
        edges.emplace_back(u, v); // unordered_set to vector
    }
}

void BiGraph::clear() {
    largest_index_in[0] = largest_index_in[1] = 0;
    n_vertex = 0;
    n_edges = 0;
    adj.clear();
    sampled_adj.clear();
    sum_wedges.clear();
    index_map.clear();
    index_map2.clear();
    index_map3.clear();
}

void BiGraph::resize() {
    sum_deg_neighbors.resize(n_vertex);
    sum_wedges.resize(n_vertex);
    index_map.resize(n_vertex);
    index_map2.resize(n_vertex);
    index_map3.resize(n_vertex);
}

void BiGraph::add_edge(const vertex_t u, const vertex_t v) {
  if (vertex2id[0].find(u) == vertex2id[0].end()) {
    vertex2id[0][u] = l_vertex_num++;
  }
  if (vertex2id[1].find(v) == vertex2id[1].end()) {
    vertex2id[1][v] = r_vertex_num++;
  }
  edges_set.insert({u, v});
}

void BiGraph::calc_graph() {
  n_wedge_in[0] = 0;
  for (vertex_t v = 0; v < largest_index_in[0]; v++) {
    n_wedge_in[0] += cn2(static_cast<ll> (adj[v].size()));
  }
  n_wedge_in[1] = 0;
  for (vertex_t v = largest_index_in[0]; v < largest_index_in[1]; v++) {
    n_wedge_in[1] += cn2(static_cast<ll>(adj[v].size()));
  }

  for (vertex_t v = 0; v < n_vertex; v++) {
    sort(adj[v].begin(), adj[v].end());

    sum_deg_neighbors[v] = 0;
    for (const vertex_t &neighbor: adj[v]) {
      sum_deg_neighbors[v] += static_cast<ll>(adj[neighbor].size());
    }
  }
}

void BiGraph::vertex_range(Side side, vertex_t &l, vertex_t &r) const {
    if (side == Side::L_SIDE) l = 0, r = largest_index_in[0];
    else if (side == Side::R_SIDE) l = largest_index_in[0], r = largest_index_in[1];
    else if (side == Side::BOTH) l = 0, r = largest_index_in[1];
}
