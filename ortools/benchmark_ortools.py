"""
DOTmark Benchmark using OR-Tools SimpleMinCostFlow (Network Simplex).
Equivalent to the C++ main.cpp benchmark.
https://developers.google.com/optimization/flow/mincostflow?hl=en
"""

import csv
import os
import sys
from math import gcd

from bipartite import compute_bipartite_ot
from tripartite import compute_tripartite_ot


def read_csv(filepath, N):
    """Read an N x N CSV file into a flat list of integers."""
    data = [0] * (N * N)
    if not os.path.exists(filepath):
        print(f"Warning: Could not open file {filepath}", file=sys.stderr)
        return data

    with open(filepath, "r") as f:
        reader = csv.reader(f)
        idx = 0
        for row in reader:
            for cell in row:
                cell = cell.strip()
                if cell:
                    data[idx] = int(cell)
                    idx += 1
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

                # GCD-based scaling to ensure exact supply == demand
                sum_a = sum(a)
                sum_b = sum(b)
                g = gcd(sum_a, sum_b)
                scale_a = sum_b // g
                scale_b = sum_a // g

                scaled_a = [val * scale_a for val in a]
                scaled_b = [val * scale_b for val in b]

                # Compute tripartite OT
                cost, resolve_time = compute_tripartite_ot(nh, scaled_a, scaled_b)

                # Rescale cost back to original mass of 'a'
                cost = cost / scale_a

                print(
                    f"{fi} {fs[f1_idx]} {fs[f2_idx]} exact_ortools_mincostflow "
                    f"runtime {resolve_time:.4f} UB {cost:.4f}",
                    flush=True,
                )


if __name__ == "__main__":
    main()
