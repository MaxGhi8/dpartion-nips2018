# Final Polish and Random Mass Scenario Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Add Scenario 5 (Random Mass) to `network_simplex_lemon/ot_test.cpp`, include `<cstdint>`, and use `std::boolalpha` for clearer output.

**Architecture:** Minor updates to `run_test` and `main`, and addition of a new test function `test_random_n4`.

**Tech Stack:** C++17, LEMON Graph Library.

---

### Task 1: Final Polish and Random Mass Scenario

**Files:**
- Modify: `network_simplex_lemon/ot_test.cpp`

- [ ] **Step 1: Add `#include <cstdint>` and update `run_test`**

```cpp
#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <cstdint>
#include "bipartite.h"
#include "tripartite.h"

using namespace std;

bool run_test(const string& name, int N, const vector<int64_t>& mu, const vector<int64_t>& nu, double expected_cost = -1.0) {
    // ... (rest of the code)
    if (match && correct) {
        cout << "  SUCCESS" << endl;
        return true;
    } else {
        cout << boolalpha;
        cout << "  FAILED: match=" << match << ", correct=" << correct;
        if (expected_cost >= 0) cout << " (expected " << expected_cost << ")";
        cout << endl;
        return false;
    }
}
```

- [ ] **Step 2: Implement `test_random_n4()`**

```cpp
void test_random_n4() {
    int N = 4;
    vector<int64_t> mu(N * N);
    vector<int64_t> nu(N * N);
    for (int i = 0; i < N * N; ++i) {
        mu[i] = i + 1;
        nu[i] = N * N - i;
    }
    // mu: {1, 2, ..., 16}, Total = 136
    // nu: {16, 15, ..., 1}, Total = 136
    run_test("Random Mass N=4", N, mu, nu);
}
```

- [ ] **Step 3: Call `test_random_n4()` in `main`**

```cpp
int main() {
    test_corner_to_corner_n2();
    test_one_to_many_n3();
    test_uniform_n4();
    test_row_shift_n4();
    test_random_n4();
    return 0;
}
```

- [ ] **Step 4: Compile the updated test**

Run: `cd network_simplex_lemon/build && make ot_test`
Expected: Success

- [ ] **Step 5: Run the tests**

Run: `./network_simplex_lemon/build/ot_test`
Expected: All tests report SUCCESS, including "Random Mass N=4".

- [ ] **Step 6: Commit**

```bash
git add network_simplex_lemon/ot_test.cpp
git commit -m "test: add random mass scenario and polish output in ot_test.cpp"
```
