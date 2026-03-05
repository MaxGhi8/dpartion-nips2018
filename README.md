# Kantorovich-Wasserstein Distances via _(d+1)_-partite Graphs

This repository contains additional material and benchmark implementations for the paper:

> Gennaro Auricchio, Federico Bassetti, Stefano Gualandi, Marco Veneroni.
> [Computing Kantorovich-Wasserstein Distances on d-dimensional histograms using (d+1)-partite graphs](https://arxiv.org/abs/1805.07416v1).
> In Proc NeurIPS 2018, Montreal Canada. [arXiv:1805.07416v1](https://arxiv.org/abs/1805.07416v1)

## Repository Structure

### `network_simplex_lemon/` — Exact Solver (C++ / Lemon)

Highly optimized C++ implementation using the [Lemon Graph Library](https://lemon.cs.elte.hu/trac/lemon)'s [Network Simplex](http://lemon.cs.elte.hu/pub/doc/latest/a00269.html) algorithm.

| File             | Description                                                                 |
| ---------------- | --------------------------------------------------------------------------- |
| `tripartite.h`   | Tripartite graph formulation — decomposes 2D transport into two 1D problems |
| `bipartite.h`    | Standard bipartite graph formulation — full source-to-sink matching         |
| `main.cpp`       | benchmark script                                                            |
| `ot_test.cpp`    | Small-scale correctness tests                                               |
| `CMakeLists.txt` | Build configuration with optimizations                                      |

**Build & Run:**

```bash
cd network_simplex_lemon/build
cmake .. && make
./benchmark_dotmark
```

---

### `ortools/` — Exact Solver (Python / OR-Tools)

Python implementation using [Google OR-Tools](https://developers.google.com/optimization/flow/mincostflow) `SimpleMinCostFlow` — a specialized Network Simplex algorithm accessed through Python.

| File                   | Description                                      |
| ---------------------- | ------------------------------------------------ |
| `tripartite.py`        | Tripartite formulation using `SimpleMinCostFlow` |
| `bipartite.py`         | Bipartite formulation using `SimpleMinCostFlow`  |
| `benchmark_ortools.py` | DOTmark benchmark                                |

**Run:**

```bash
cd ortools/
python3 benchmark_ortools.py
```

**Dependencies:** `pip install ortools`

---

### `gurobi_solver/` — Exact Solver (Python / Gurobi)

Python implementation using the native [Gurobi](https://www.gurobi.com/) LP solver via `gurobipy`.

| File                  | Description               |
| --------------------- | ------------------------- |
| `tripartite.py`       | Tripartite LP formulation |
| `bipartite.py`        | Bipartite LP formulation  |
| `benchmark_gurobi.py` | DOTmark benchmark script  |

**Run:**

```bash
cd gurobi_solver/
python3 benchmark_gurobi.py
```

**Dependencies:** `pip install gurobipy` (requires Gurobi license)

---

### `improved_sinkhorn/` — Approximate Solver (MATLAB)

Optimized Sinkhorn algorithm exploiting Kronecker product structure for matrix-vector multiplications. Based on [Cuturi 2013](http://marcocuturi.net/SI.html) and developed by the authors of the paper.

| File           | Description                                      |
| -------------- | ------------------------------------------------ |
| `myFS.m`       | Improved Sinkhorn with Kronecker-factored kernel |
| `myFSscript.m` | DOTmark benchmark script                         |

---

### `sinkhorn/` — Standard Sinkhorn (MATLAB)

Reference implementation of the standard Sinkhorn algorithm, taken from [Cuturi 2013](http://marcocuturi.net/SI.html).

| File                      | Description                                        |
| ------------------------- | -------------------------------------------------- |
| `sinkhornTransport.m`     | Full Sinkhorn implementation with multiple options |
| `benchmarkSinkhorn.m`     | DOTmark benchmark script                           |
| `testSinkhornTransport.m` | Correctness tests                                  |

---

### `DOTmark/` — Benchmark Dataset

The [DOTmark](http://www.stochastik.math.uni-goettingen.de/DOTmark/) benchmark dataset containing 10 classes of 2D histograms at resolutions 32×32, 64×64, and 128×128.

## Key Design Decisions

- **Tripartite Decomposition**: For the case of computing Wasserstein distances between two images, the (d+1)-partite formulation reduces the number of arcs from O(N⁴) (bipartite) to O(2·N³) (tripartite), enabling significantly faster computation.

## License

See [LICENSE](LICENSE).
