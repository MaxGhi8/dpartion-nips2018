import gurobipy as gp
from gurobipy import GRB
import time

def compute_bipartite_ot(N, mu, nu):
    """
    Computes Exact Optimal Transport using the Bipartite Graph Formulation
    """
    model = gp.Model("bipartite_ot")
    model.Params.OutputFlag = 0
    
    num_nodes = N * N
    
    x = {}
    for a in range(N):
        for j in range(N):
            u = a * N + j
            for i in range(N):
                for b in range(N):
                    v = i * N + b
                    cost = (a - i)**2 + (j - b)**2
                    x[u, v] = model.addVar(obj=cost, vtype=GRB.CONTINUOUS, name=f"x_{u}_{v}")
                    
    for u in range(num_nodes):
        model.addConstr(gp.quicksum(x[u, v] for v in range(num_nodes)) == mu[u])
        
    for v in range(num_nodes):
        model.addConstr(gp.quicksum(x[u, v] for u in range(num_nodes)) == nu[v])
        
    start_time = time.perf_counter()
    model.optimize()
    end_time = time.perf_counter()
    
    resolve_time = end_time - start_time
    
    if model.status == GRB.OPTIMAL:
        return model.ObjVal, resolve_time
    else:
        return -1.0, resolve_time
