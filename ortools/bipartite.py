"""
Bipartite formulation for Optimal Transport using OR-Tools SimpleMinCostFlow.
Uses the specialized Network Simplex algorithm (same as Lemon).
"""

import time

from ortools.graph.python import min_cost_flow


def compute_bipartite_ot(N, mu, nu):
    """
    Computes Exact Optimal Transport using the Bipartite Graph Formulation.

    Args:
        N: Grid dimension (histograms are N x N).
        mu: Source histogram as flat list of int64, length N*N.
        nu: Target histogram as flat list of int64, length N*N.

    Returns:
        (cost, resolve_time): Total transport cost and solver wall time in seconds.
    """
    num_nodes = N * N
    total_supply = sum(mu)

    INT64_MAX = (1 << 63) - 1
    safe_cap = INT64_MAX // (num_nodes + 1)
    cap = min(total_supply, safe_cap)

    smcf = min_cost_flow.SimpleMinCostFlow()

    # Node indices: sources = [0, num_nodes), sinks = [num_nodes, 2*num_nodes)
    # Arcs: source(a,j) -> sink(i,b), cost = (a-i)^2 + (j-b)^2
    for a in range(N):
        for j in range(N):
            u = a * N + j
            for i in range(N):
                for b in range(N):
                    v = num_nodes + i * N + b
                    cost = (a - i) ** 2 + (j - b) ** 2
                    smcf.add_arc_with_capacity_and_unit_cost(u, v, cap, cost)

    # Set supplies
    for k in range(num_nodes):
        smcf.set_node_supply(k, mu[k])  # source
        smcf.set_node_supply(num_nodes + k, -nu[k])  # sink

    # Solve
    start_time = time.perf_counter()
    status = smcf.solve()
    end_time = time.perf_counter()

    resolve_time = end_time - start_time

    if status == smcf.OPTIMAL:
        return smcf.optimal_cost(), resolve_time
    else:
        print(f"Error: Solver status = {status}")
        return -1.0, resolve_time
