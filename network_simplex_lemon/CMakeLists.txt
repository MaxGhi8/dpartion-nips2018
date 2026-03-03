cmake_minimum_required(VERSION 3.10)
project(optimal_transport)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find the Lemon Graph Library
find_library(LEMON_LIBRARY NAMES lemon PATHS /usr/lib /usr/local/lib)
find_path(LEMON_INCLUDE_DIR NAMES lemon/smart_graph.h PATHS /usr/include /usr/local/include)

if(LEMON_LIBRARY AND LEMON_INCLUDE_DIR)
    message(STATUS "Found LEMON: ${LEMON_LIBRARY}")
    include_directories(${LEMON_INCLUDE_DIR})
else()
    message(FATAL_ERROR "LEMON library not found. Please install it.")
endif()

add_executable(ot_test ot_test.cpp)
target_link_libraries(ot_test ${LEMON_LIBRARY})

add_executable(benchmark_dotmark main.cpp)
target_link_libraries(benchmark_dotmark ${LEMON_LIBRARY})
