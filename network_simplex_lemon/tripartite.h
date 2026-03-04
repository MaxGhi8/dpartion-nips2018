#include <iostream>
#include <vector>
#include <cmath>
#include <lemon/smart_graph.h>
#include <lemon/network_simplex.h>
#include <chrono>

#include <cstdint>

using namespace lemon;
using namespace std;

// Implement Tripartite formulation for 2D histograms of size N x N
// mu and nu are the input histograms as flat arrays of size N*N
double compute_tripartite_ot(int N, const vector<int64_t>& mu, const vector<int64_t>& nu, double& resolve_time) {
    SmartDigraph g;
    SmartDigraph::NodeMap<int64_t> supply(g);
    SmartDigraph::ArcMap<double> cost(g);
    
    int num_nodes = N * N;
    
    // Create nodes for V1 (source mu), V2 (intermediate), V3 (target nu)
    vector<SmartDigraph::Node> V1(num_nodes);
    vector<SmartDigraph::Node> V2(num_nodes);
    vector<SmartDigraph::Node> V3(num_nodes);
    
    int64_t total_supply = 0;
    
    for (int i = 0; i < num_nodes; ++i) {
        V1[i] = g.addNode();
        supply[V1[i]] = mu[i];
        total_supply += mu[i];
        
        V2[i] = g.addNode();
        supply[V2[i]] = 0; // Middle nodes have 0 supply
        
        V3[i] = g.addNode();
        supply[V3[i]] = -nu[i];
    }
    
    // Create arcs A1: (a,j) in V1 -> (i,j) in V2. Cost = (a-i)^2
    for (int a = 0; a < N; ++a) {
        for (int j = 0; j < N; ++j) {
            int u1 = a * N + j;
            for (int i = 0; i < N; ++i) {
                int v2 = i * N + j;
                SmartDigraph::Arc arc = g.addArc(V1[u1], V2[v2]);
                cost[arc] = (a - i) * (a - i);
            }
        }
    }
    
    // Create arcs A2: (i,j) in V2 -> (i,b) in V3. Cost = (j-b)^2
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            int u2 = i * N + j;
            for (int b = 0; b < N; ++b) {
                int v3 = i * N + b;
                SmartDigraph::Arc arc = g.addArc(V2[u2], V3[v3]);
                cost[arc] = (j - b) * (j - b);
            }
        }
    }
    
    NetworkSimplex<SmartDigraph, int64_t, double> ns(g);
    ns.supplyMap(supply);
    ns.costMap(cost);
    
    auto tStart = chrono::high_resolution_clock::now();
    NetworkSimplex<SmartDigraph, int64_t, double>::ProblemType status = ns.run();
    auto tEnd = chrono::high_resolution_clock::now();
    
    chrono::duration<double> tElapsed = tEnd - tStart;
    resolve_time = tElapsed.count();
    
    if (status == NetworkSimplex<SmartDigraph, int64_t, double>::OPTIMAL) {
        return ns.totalCost();
    } else {
        cerr << "Error: Optimal solution not found!" << endl;
        return -1.0;
    }
}
