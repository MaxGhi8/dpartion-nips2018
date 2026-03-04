import sys
import os
import csv
import time
from tripartite import compute_tripartite_ot
from bipartite import compute_bipartite_ot

def read_csv(filepath, N):
    data = []
    if not os.path.exists(filepath):
        print(f"File not found: {filepath}", file=sys.stderr)
        return [0] * (N * N)
        
    with open(filepath, 'r') as f:
        reader = csv.reader(f)
        for row in reader:
            for cell in row:
                if cell.strip():
                    data.append(int(cell.strip()))
    return data

def main():
    base = "../DOTmark/Data/"
    nh = 64
    imsize = f"/data{nh}_"
    
    images = ["CauchyDensity"]
    fs = [f"{1001 + i}.csv" for i in range(10)]
    
    for fi in images:
        for f1_idx in range(len(fs) - 1):
            path_a = os.path.join(base, fi + imsize + fs[f1_idx])
            a = read_csv(path_a, nh)
            
            for f2_idx in range(f1_idx + 1, len(fs)):
                path_b = os.path.join(base, fi + imsize + fs[f2_idx])
                b = read_csv(path_b, nh)
                
                sum_a = sum(a)
                sum_b = sum(b)
                
                scaled_a = [val * sum_b for val in a]
                scaled_b = [val * sum_a for val in b]
                
                # compute
                cost, resolve_time = compute_tripartite_ot(nh, scaled_a, scaled_b)
                cost = cost / sum_b
                
                print(f"{fi} {fs[f1_idx]} {fs[f2_idx]} exact_gurobi runtime {resolve_time:.4f} UB {cost:.4f}", flush=True)

if __name__ == "__main__":
    main()
