# Network Simplex with Lemon Graph Library

This directory contains implementations of the exact Kantorovich-Wasserstein distances using bipartite and 3-partite graphs via the Network Simplex solver provided by the Lemon Graph library.

It also contains tests and a simple program (`hello_lemon.cc`) to verify that your Lemon installation works.

## Prerequisites

Make sure you have a C++ compiler (like `g++`) and `cmake` installed, as well as the Lemon Graph library.
On Ubuntu/Debian, the library can be installed via:

```bash
sudo apt-get update
sudo apt-get install liblemon-dev
```

## Running `hello_lemon.cc`

The file `hello_lemon.cc` is a minimal, standalone C++ file. Because it has no complex dependencies other than the Lemon library itself, you can compile and run it directly with `g++`. This file is used to check that you have correctly installed the Lemon library.

1. **Compile it**:
   Use `g++` and make sure to link the lemon library with the `-llemon` flag:

   ```bash
   g++ -o hello_lemon hello_lemon.cc -llemon
   ```

2. **Run it**:
   ```bash
   ./hello_lemon
   ```

## Running `ot_test.cpp` (Optimal Transport Tests)

The file `ot_test.cpp` relies on several header files (`bipartite.h` and `tripartite.h`).
This directory includes a `CMakeLists.txt` file configured to find the Lemon library and compile everything automatically.

1. **Create a build directory**:
   We do this to keep all the generated compiler files separate from our clean source code.

   ```bash
   mkdir -p build
   cd build
   ```

2. **Configure the build with CMake**:

   ```bash
   cmake ..
   ```

3. **Compile the program**:

   ```bash
   make
   ```

4. **Run the test**:

   ```bash
   ./ot_test
   ```

5. **Quick Re-compilation**:

If you modify `ot_test.cpp` or any of the `.h` files, you do not need to repeat all the steps above! Just go into your `build` directory and run `make` again. The compiler is smart enough to only rebuild exactly what has changed.
