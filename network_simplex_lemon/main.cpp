#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
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
    // The executable will run from network_simplex_lemon/build/
    string base = "../../DOTmark/Data/";
    int nh = 64;
    string imsize = "/data"+to_string(nh)+"_";
    
    vector<string> images = {
        "CauchyDensity", 
        // "ClassicImages", "GRFmoderate", "GRFrough", 
        // "GRFsmooth", "LogGRF", "LogitGRF", "MicroscopyImages", "Shapes", "WhiteNoise"
    };
    
    vector<string> fs = {
        "1001.csv", "1002.csv", "1003.csv", "1004.csv", "1005.csv", 
        "1006.csv", "1007.csv", "1008.csv", "1009.csv", "1010.csv"
    };
    
    int num_nodes = nh * nh;
    
    cout << fixed << setprecision(4);

    for (size_t fi = 0; fi < images.size(); ++fi) {
        for (size_t f1 = 0; f1 < fs.size() - 1; ++f1) {
            // First image
            string path_a = base + images[fi] + imsize + fs[f1];
            vector<int64_t> a = read_csv(path_a, nh);
            
            for (size_t f2 = f1 + 1; f2 < fs.size(); ++f2) {
                // Second image
                string path_b = base + images[fi] + imsize + fs[f2];
                vector<int64_t> b = read_csv(path_b, nh);
                
                // We ensure sum(a) exactly equals sum(b). 
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

                double resolve_time = 0.0;
                
                // Compute optimal transport cost
                double D = compute_tripartite_ot(nh, scaled_a, scaled_b, resolve_time);
                
                // We scaled 'a' by 'sum_b' and 'b' by 'sum_a' to make their mass exactly equal to (sum_a * sum_b)
                // MATLAB computes the distance on the UNNORMALIZED arrays (mass = sum_a)
                // So to get the cost scaled to the original mass of 'a', we only divide out 'sum_b'.
                D = D / (double)sum_b;

                cout << images[fi] << " " << fs[f1] << " " << fs[f2] 
                     << " exact_lemon" 
                     << " runtime " << resolve_time
                     << " UB " << D << endl;
            }
        }
    }
    
    return 0;
}
