#include <iostream>
#include <vector>
#include "bipartite.h"
#include "tripartite.h"

using namespace std;

int main() {
    int N = 3; // 3x3 histogram
    int num_nodes = N * N;
    
    // Initialize two 1D arrays representing 3x3 grids (flattened)
    vector<int64_t> mu(num_nodes, 0);
    vector<int64_t> nu(num_nodes, 0);
    
    // (a=0, j=0) is top-left
    mu[0] = 1;
    
    // (i=2, b=2) is bottom-right
    nu[8] = 1;
    
    cout << "Running tests with N=" << N << endl;
    
    double cost_bipartite = compute_bipartite_ot(N, mu, nu);
    cout << "Bipartite Cost: " << cost_bipartite << endl;
    
    double cost_tripartite = compute_tripartite_ot(N, mu, nu);
    cout << "Tripartite Cost: " << cost_tripartite << endl;
    
    double expected_dist_sq = (0 - 2)*(0 - 2) + (0 - 2)*(0 - 2);
    
    if (abs(cost_bipartite - cost_tripartite) < 1e-6 && abs(cost_bipartite - expected_dist_sq) < 1e-6) {
        cout << "SUCCESS: Both algorithms compute the correct optimal transport cost!" << endl;
    } else {
        cout << "FAILED: Bipartite and Tripartite returned different costs, or cost is incorrect!" << endl;
    }
    
    return 0;
}
