#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <cmath>

#include "bipartite.h"
#include "tripartite.h"

using namespace std;

// Function to read a 32x32 CSV file mapping into a 1D vector (N=32)
vector<int64_t> read_csv(const string& filepath, int N) {
    vector<int64_t> data(N * N, 0);
    ifstream file(filepath);
    
    if (!file.is_open()) {
        cerr << "Warning: Could not open file " << filepath << endl;
        return data; // Return empty/zero vector
    }
    
    string line;
    int row = 0;
    while (getline(file, line) && row < N) {
        stringstream ss(line);
        string cell;
        int col = 0;
        
        while (getline(ss, cell, ',') && col < N) {
            data[row * N + col] = stoll(cell);
            col++;
        }
        row++;
    }
    
    file.close();
    return data;
}

int main() {
    // Update base path to point two directories up to reach dpartion-nips2018/DOTmark
    // since the executable will run from network_simplex_lemon/build/
    string base = "../../DOTmark/Data/";
    string imsize = "/data32_";
    
    vector<string> images = {
        "CauchyDensity", 
        // "ClassicImages", "GRFmoderate", "GRFrough", 
        // "GRFsmooth", "LogGRF", "LogitGRF", "MicroscopyImages", "Shapes", "WhiteNoise"
    };
    
    vector<string> fs = {
        "1001.csv", "1002.csv", "1003.csv", "1004.csv", "1005.csv", 
        "1006.csv", "1007.csv", "1008.csv", "1009.csv", "1010.csv"
    };
    
    int nh = 32;
    int num_nodes = nh * nh;
    
    cout << fixed << setprecision(4);

    for (size_t fi = 0; fi < images.size(); ++fi) {
        for (size_t f1 = 0; f1 < fs.size() - 1; ++f1) {
            string path_a = base + images[fi] + imsize + fs[f1];
            vector<int64_t> a = read_csv(path_a, nh);
            
            for (size_t f2 = f1 + 1; f2 < fs.size(); ++f2) {
                
                string path_b = base + images[fi] + imsize + fs[f2];
                vector<int64_t> b = read_csv(path_b, nh);
                
                // For exact uncapacitated network simplex solvers, 
                // integer capacities are required to avoid precision errors. 
                // We ensure sum(a) exactly equals sum(b). Since they might be slightly off in the DOTmark CSVs, 
                // we multiply a by sum(b) and b by sum(a) to force perfect integer equality.
                
                int64_t sum_a = 0;
                int64_t sum_b = 0;
                for(int64_t v : a) sum_a += v;
                for(int64_t v : b) sum_b += v;
                
                vector<int64_t> scaled_a(num_nodes);
                vector<int64_t> scaled_b(num_nodes);
                
                for(int i = 0; i < num_nodes; ++i) {
                    scaled_a[i] = a[i] * sum_b;
                    scaled_b[i] = b[i] * sum_a;
                }

                auto tStart = chrono::high_resolution_clock::now();
                
                // Compute optimal transport cost
                double D = compute_bipartite_ot(nh, scaled_a, scaled_b);
                
                // We scaled 'a' by 'sum_b' and 'b' by 'sum_a' to make their mass exactly equal to (sum_a * sum_b)
                // MATLAB computes the distance on the UNNORMALIZED arrays (mass = sum_a)
                // So to get the cost scaled to the original mass of 'a', we only divide out 'sum_b'.
                D = D / (double)sum_b;

                auto tEnd = chrono::high_resolution_clock::now();
                chrono::duration<double> tElapsed = tEnd - tStart;

                // Emulate MATLAB Output: images(fi) fs(f1) fs(f2) lambda runtime UB
                // Since this is exact network simplex, lambda is conceptually 'inf' or exact. We mimic the string formatting.
                cout << images[fi] << " " << fs[f1] << " " << fs[f2] 
                     << " exact_lemon" 
                     << " runtime " << tElapsed.count() 
                     << " UB " << D << endl;
            }
        }
    }
    
    return 0;
}
