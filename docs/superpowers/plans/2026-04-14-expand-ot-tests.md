# Expand Optimal Transport Tests Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Expand `network_simplex_lemon/ot_test.cpp` to include a structured test suite for $N=2, 3, 4$ with various mass distributions, ensuring both Bipartite and Tripartite algorithms agree on the optimal transport cost.

**Architecture:** Refactor `ot_test.cpp` into a suite of test functions that use a shared `run_test` helper. Each test function will define $\mu$ and $\nu$ histograms and verify the costs.

**Tech Stack:** C++17, LEMON Graph Library.

---

### Task 1: Refactor and Add Test Runner

**Files:**
- Modify: `network_simplex_lemon/ot_test.cpp`

- [ ] **Step 1: Replace existing `main` with `run_test` helper and structured `main`**

```cpp
#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include "bipartite.h"
#include "tripartite.h"

using namespace std;

bool run_test(const string& name, int N, const vector<int64_t>& mu, const vector<int64_t>& nu, double expected_cost = -1.0) {
    cout << "--- Testing: " << name << " (N=" << N << ") ---" << endl;
    
    double t1 = 0.0, t2 = 0.0;
    double cost_bipartite = compute_bipartite_ot(N, mu, nu, t1);
    double cost_tripartite = compute_tripartite_ot(N, mu, nu, t2);
    
    cout << fixed << setprecision(6);
    cout << "  Bipartite Cost: " << cost_bipartite << " (" << t1 << "s)" << endl;
    cout << "  Tripartite Cost: " << cost_tripartite << " (" << t2 << "s)" << endl;
    
    bool match = abs(cost_bipartite - cost_tripartite) < 1e-6;
    bool correct = true;
    if (expected_cost >= 0) {
        correct = abs(cost_bipartite - expected_cost) < 1e-6;
    }
    
    if (match && correct) {
        cout << "  SUCCESS" << endl;
        return true;
    } else {
        cout << "  FAILED: match=" << match << ", correct=" << correct;
        if (expected_cost >= 0) cout << " (expected " << expected_cost << ")";
        cout << endl;
        return false;
    }
}

void test_corner_to_corner_n2() {
    int N = 2;
    vector<int64_t> mu(N * N, 0);
    vector<int64_t> nu(N * N, 0);
    mu[0] = 1; // (0,0)
    nu[3] = 1; // (1,1)
    double expected = (0-1)*(0-1) + (0-1)*(0-1); // 2.0
    run_test("Corner-to-Corner N=2", N, mu, nu, expected);
}

void test_one_to_many_n3() {
    int N = 3;
    vector<int64_t> mu(N * N, 0);
    vector<int64_t> nu(N * N, 0);
    mu[0] = 2; // (0,0)
    nu[4] = 1; // (1,1)
    nu[8] = 1; // (2,2)
    double expected = ( (0-1)*(0-1) + (0-1)*(0-1) ) + ( (0-2)*(0-2) + (0-2)*(0-2) ); // 2 + 8 = 10.0
    run_test("One-to-Many N=3", N, mu, nu, expected);
}

void test_uniform_n4() {
    int N = 4;
    vector<int64_t> mu(N * N, 1);
    vector<int64_t> nu(N * N, 1);
    run_test("Uniform-to-Uniform N=4", N, mu, nu, 0.0);
}

void test_row_shift_n4() {
    int N = 4;
    vector<int64_t> mu(N * N, 0);
    vector<int64_t> nu(N * N, 0);
    for (int j = 0; j < N; ++j) {
        mu[0 * N + j] = 1; // Row 0
        nu[1 * N + j] = 1; // Row 1
    }
    double expected = 4.0 * ((0-1)*(0-1) + (0-0)*(0-0)); // 4.0
    run_test("Row Shift N=4", N, mu, nu, expected);
}

int main() {
    test_corner_to_corner_n2();
    test_one_to_many_n3();
    test_uniform_n4();
    test_row_shift_n4();
    return 0;
}
```

- [ ] **Step 2: Compile the updated test**

Run: `cd network_simplex_lemon/build && make ot_test`
Expected: Success

- [ ] **Step 3: Run the tests**

Run: `./network_simplex_lemon/build/ot_test`
Expected: All tests report SUCCESS.

- [ ] **Step 4: Commit**

```bash
git add network_simplex_lemon/ot_test.cpp
git commit -m "test: expand OT tests for N=2, 3, 4"
```
