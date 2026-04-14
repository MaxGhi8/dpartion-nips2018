# Design Document: Expanding Optimal Transport Tests

**Date:** 2026-04-14
**Topic:** Adding more test cases to `network_simplex_lemon/ot_test.cpp` for N=2, 3, and 4.

## 1. Problem Statement
The current `ot_test.cpp` only tests a single 3x3 case (top-left to bottom-right). We need to verify the correctness and consistency of both bipartite and tripartite algorithms across multiple grid sizes and distributions.

## 2. Goals
- Add tests for $N=2, 3, 4$.
- Cover various transport scenarios (point-to-point, mass splitting, bulk movement).
- Ensure both algorithms consistently return the same cost for each scenario.
- Measure and display resolve time for both algorithms.

## 3. Architecture & Components
The tests will be implemented in a single file `network_simplex_lemon/ot_test.cpp`.

### 3.1 Test Runner (`run_test`)
A helper function to:
- Take $N$, $\mu$, $\nu$, and an optional `expected_cost`.
- Execute `compute_bipartite_ot`.
- Execute `compute_tripartite_ot`.
- Compare results with each other (within $10^{-6}$ epsilon).
- If `expected_cost` is provided, compare with it.
- Print results and success/failure status.

### 3.2 Test Scenarios
- **Scenario 1: Corner-to-Corner (N=2)**
  - $\mu[0,0] = 1$, $\nu[1,1] = 1$.
  - Expected Cost: $1^2 + 1^2 = 2$.
- **Scenario 2: One-to-Many (N=3)**
  - $\mu[0,0] = 2$, $\nu[1,1] = 1, \nu[2,2] = 1$.
  - Expected Cost: $(1^2+1^2) + (2^2+2^2) = 2 + 8 = 10$.
- **Scenario 3: Uniform-to-Uniform (N=4)**
  - All $\mu[i,j] = 1$, all $\nu[i,j] = 1$.
  - Expected Cost: 0.
- **Scenario 4: Row Shift (N=4)**
  - $\mu[0, \text{all}] = 1$ (first row), $\nu[1, \text{all}] = 1$ (second row).
  - Each cell in row 0 moves 1 step down to row 1.
  - Expected Cost: $4 \times (1^2 + 0^2) = 4$.
- **Scenario 5: Random Mass (N=2, 3, 4)**
  - Agreement check between solvers on non-trivial distributions where we don't manually calculate the cost.

## 4. Testing Strategy
- Validation will be performed by running the compiled `ot_test` binary.
- Success is defined as all test scenarios passing (algorithms agreeing and matching manual expected values where available).

## 5. Security and Safety
No sensitive data is involved. All operations are local to the C++ test file.
