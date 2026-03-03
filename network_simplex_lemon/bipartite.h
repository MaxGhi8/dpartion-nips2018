#include <iostream>
#include <vector>
#include <cmath>
#include <lemon/smart_graph.h>
#include <lemon/network_simplex.h>

using namespace lemon;
using namespace std;

// Compute squared Euclidean distance cost
double compute_cost(int i, int j, int a, int b) {
    return (a - i) * (a - i) + (j - b) * (j - b);
}

// Implement Bipartite formulation for 2D histograms of size N x N
// mu and nu are the input histograms as flat arrays of size N*N
double compute_bipartite_ot(int N, const vector<double>& mu, const vector<double>& nu) {
    SmartDigraph g;
    SmartDigraph::NodeMap<double> supply(g);
    SmartDigraph::ArcMap<double> cost(g);
    SmartDigraph::ArcMap<double> capacity(g);
    
    int num_nodes = N * N;
    
    // Create vector of nodes for mu (V1) and nu (V2)
    vector<SmartDigraph::Node> V1(num_nodes);
    vector<SmartDigraph::Node> V2(num_nodes);
    
    double total_supply = 0;
    double total_demand = 0;
    
    for (int i = 0; i < num_nodes; ++i) {
        V1[i] = g.addNode();
        supply[V1[i]] = mu[i];
        total_supply += mu[i];
        
        V2[i] = g.addNode();
        supply[V2[i]] = -nu[i];
        total_demand += nu[i];
    }
    
    // Create arcs
    for (int a = 0; a < N; ++a) {
        for (int j = 0; j < N; ++j) {
            int u = a * N + j;
            for (int i = 0; i < N; ++i) {
                for (int b = 0; b < N; ++b) {
                    int v = i * N + b;
                    SmartDigraph::Arc arc = g.addArc(V1[u], V2[v]);
                    cost[arc] = compute_cost(a, j, i, b);
                    capacity[arc] = total_supply; // Uncapacitated
                }
            }
        }
    }
    
    NetworkSimplex<SmartDigraph, double, double> ns(g);
    ns.supplyMap(supply);
    ns.costMap(cost);
    // ns.upperMap(capacity); // Omit for uncapacitated problem
    
    NetworkSimplex<SmartDigraph, double, double>::ProblemType status = ns.run();
    
    if (status == NetworkSimplex<SmartDigraph, double, double>::OPTIMAL) {
        return ns.totalCost();
    } else {
        cerr << "Error: Optimal solution not found!" << endl;
        return -1.0;
    }
}
