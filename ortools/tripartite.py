"""
Tripartite formulation for Optimal Transport using OR-Tools SimpleMinCostFlow.
Uses the specialized Network Simplex algorithm (same as Lemon).
"""

import time

from ortools.graph.python import min_cost_flow


def compute_tripartite_ot(N, mu, nu):
    """
    Computes Exact Optimal Transport using the Tripartite Graph Formulation.

    V1 (sources) -> V2 (intermediate) -> V3 (sinks)
    A1 arcs: cost = (a-i)^2, A2 arcs: cost = (j-b)^2

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
    safe_cap = INT64_MAX // (2 * N + 1)
    cap = min(total_supply, safe_cap)

    smcf = min_cost_flow.SimpleMinCostFlow()

    # Node indices: V1 = [0, num_nodes), V2 = [num_nodes, 2*num_nodes), V3 = [2*num_nodes, 3*num_nodes)
    # A1 arcs: V1(a,j) -> V2(i,j), cost = (a-i)^2
    for a in range(N):
        for j in range(N):
            u1 = a * N + j
            for i in range(N):
                v2 = num_nodes + i * N + j
                cost = (a - i) ** 2
                smcf.add_arc_with_capacity_and_unit_cost(u1, v2, cap, cost)

    # A2 arcs: V2(i,j) -> V3(i,b), cost = (j-b)^2
    for i in range(N):
        for j in range(N):
            u2 = num_nodes + i * N + j
            for b in range(N):
                v3 = 2 * num_nodes + i * N + b
                cost = (j - b) ** 2
                smcf.add_arc_with_capacity_and_unit_cost(u2, v3, cap, cost)

    # Set supplies
    for k in range(num_nodes):
        smcf.set_node_supply(k, mu[k])  # V1: source
        smcf.set_node_supply(num_nodes + k, 0)  # V2: intermediate
        smcf.set_node_supply(2 * num_nodes + k, -nu[k])  # V3: sink

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
