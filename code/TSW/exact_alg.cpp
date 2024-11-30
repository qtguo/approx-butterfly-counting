#include "exact_alg.h"

vector<ll> hashmap_C;
vector<vertex_t> aux;

template<typename T>
inline T cn2(T n) {
    return n * (n - 1) / 2;
}

ld bfc_exact(const BiGraph &graph) {
    hashmap_C.resize(graph.n_vertex);
    aux.resize(graph.n_vertex);

    const auto &adj = graph.adj;

    ll n_wedge_in[2] = {0, 0};
    vertex_t l = 0, r = 0;
    graph.vertex_range(L_SIDE, l, r);
    for (vertex_t u = l; u < r; ++u)
        n_wedge_in[0] += cn2(static_cast<ll>(adj[u].size()));

    graph.vertex_range(R_SIDE, l, r);
    for (vertex_t u = l; u < r; ++u)
        n_wedge_in[1] += cn2(static_cast<ll>(adj[u].size()));

    Side side = n_wedge_in[0] < n_wedge_in[1] ? R_SIDE : L_SIDE;
    graph.vertex_range(side, l, r);

    ld res = 0;
    // int count = 0;
    for (vertex_t u = l; u < r; ++u) {
        vertex_t idx = 0;
        for (vertex_t u1hop: adj[u]) {
            for (vertex_t u2hop: adj[u1hop]) {
                // count++;
                if (u <= u2hop) break;
                if (!hashmap_C[u2hop]) aux[idx++] = u2hop;
                res += hashmap_C[u2hop];
                ++hashmap_C[u2hop];
            }
        }
        for (int j = 0; j < idx; ++j) {
            hashmap_C[aux[j]] = 0;
        }
    }
    // std::cout<<"count:"<<count<<std::endl;
    return res;
}

inline unsigned long long degree_sq(unordered_map<int, unordered_set<int>> &partition) {
	unsigned long long res = 0;
	for (auto p : partition) {
		res += p.second.size() * p.second.size();
	}
	return res;
}

long long batch_bfly_count(std::vector<std::pair<int, int>> &edge_vec) {
	// Convert the edges to adjacency list.
	unordered_map<int, unordered_set<int> > left_par;
	unordered_map<int, unordered_set<int> > right_par;
	for (auto it = edge_vec.begin(); it != edge_vec.end(); it++) {
		int A = it->first;
		int B = it->second;
		left_par[A].insert(B);
		right_par[B].insert(A);
	}
	unsigned long long d2_l = degree_sq(left_par);
	unsigned long long d2_r = degree_sq(right_par);
	if (d2_l < d2_r) {
		std::swap(left_par, right_par);
	}

	long long total_count = 0;
	for (auto p : left_par) {
		int l = p.first;
		std::unordered_map<int, long long> counter;
		for (auto r : left_par[l]) {
			for (auto l2 : right_par[r]){
				if (l2 > l) {
					if (counter.find(l2) != counter.end()) {
						total_count += counter[l2];
						counter[l2]++;
					}
					else {
						counter[l2] = 1;
					}
				}
			}
		}
	}
	return total_count;
}
