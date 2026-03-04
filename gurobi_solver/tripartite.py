import gurobipy as gp
from gurobipy import GRB
import time

def compute_tripartite_ot(N, mu, nu):
    """
    Computes Exact Optimal Transport using the Tripartite Graph Formulation
    """
    model = gp.Model("tripartite_ot")
    model.Params.OutputFlag = 0 # Silent mode
    
    num_nodes = N * N
    
    # Create variables
    # A1: V1 -> V2
    x = {}
    for a in range(N):
        for j in range(N):
            u1 = a * N + j
            for i in range(N):
                v2 = i * N + j
                cost = (a - i) ** 2
                x[u1, v2] = model.addVar(obj=cost, vtype=GRB.CONTINUOUS, name=f"x_{u1}_{v2}")
                
    # A2: V2 -> V3
    y = {}
    for i in range(N):
        for j in range(N):
            u2 = i * N + j
            for b in range(N):
                v3 = i * N + b
                cost = (j - b) ** 2
                y[u2, v3] = model.addVar(obj=cost, vtype=GRB.CONTINUOUS, name=f"y_{u2}_{v3}")
                
    # Constraints
    # V1: supply = mu
    for a in range(N):
        for j in range(N):
            u1 = a * N + j
            model.addConstr(gp.quicksum(x[u1, i * N + j] for i in range(N)) == mu[u1])
            
    # V2: intermediate flow
    for i in range(N):
        for j in range(N):
            u2 = i * N + j
            flow_out = gp.quicksum(y[u2, i * N + b] for b in range(N))
            flow_in = gp.quicksum(x[a * N + j, u2] for a in range(N))
            model.addConstr(flow_out - flow_in == 0)
            
    # V3: supply = -nu (so flow in = nu)
    for i in range(N):
        for b in range(N):
            v3 = i * N + b
            model.addConstr(gp.quicksum(y[i * N + j, v3] for j in range(N)) == nu[v3])
            
    start_time = time.perf_counter()
    model.optimize()
    end_time = time.perf_counter()
    
    resolve_time = end_time - start_time
    
    if model.status == GRB.OPTIMAL:
        return model.ObjVal, resolve_time
    else:
        return -1.0, resolve_time
