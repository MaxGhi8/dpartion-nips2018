#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include <lemon/smart_graph.h>
#include <lemon/network_simplex.h>
#include <chrono>

#include <cstdint>

// Compute squared Euclidean distance cost
inline double compute_cost(int i, int j, int a, int b) {
    return (a - i) * (a - i) + (j - b) * (j - b);
}

// Implement Bipartite formulation for 2D histograms of size N x N
// mu and nu are the input histograms as flat arrays of size N*N
inline double compute_bipartite_ot(int N, const std::vector<std::int64_t>& mu, const std::vector<std::int64_t>& nu, double& resolve_time) {
    lemon::SmartDigraph g;
    lemon::SmartDigraph::NodeMap<std::int64_t> supply(g);
    lemon::SmartDigraph::ArcMap<double> cost(g);
    lemon::SmartDigraph::ArcMap<double> capacity(g);
    
    int num_nodes = N * N;
    
    // Create vector of nodes for mu (V1) and nu (V2)
    std::vector<lemon::SmartDigraph::Node> V1(num_nodes);
    std::vector<lemon::SmartDigraph::Node> V2(num_nodes);
    
    std::int64_t total_supply = 0;
    std::int64_t total_demand = 0;
    
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
                    lemon::SmartDigraph::Arc arc = g.addArc(V1[u], V2[v]);
                    cost[arc] = compute_cost(a, j, i, b);
                    capacity[arc] = total_supply; // Uncapacitated
                }
            }
        }
    }
    
    lemon::NetworkSimplex<lemon::SmartDigraph, double, double> ns(g);
    ns.supplyMap(supply);
    ns.costMap(cost);
    // ns.upperMap(capacity); // Omit for uncapacitated problem
    
    auto tStart = std::chrono::high_resolution_clock::now();
    lemon::NetworkSimplex<lemon::SmartDigraph, double, double>::ProblemType status = ns.run();
    auto tEnd = std::chrono::high_resolution_clock::now();
    
    std::chrono::duration<double> tElapsed = tEnd - tStart;
    resolve_time = tElapsed.count();
    
    if (status == lemon::NetworkSimplex<lemon::SmartDigraph, double, double>::OPTIMAL) {
        return ns.totalCost();
    } else {
        std::cerr << "Error: Optimal solution not found!" << std::endl;
        return -1.0;
    }
}
