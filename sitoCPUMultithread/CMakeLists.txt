cmake_minimum_required(VERSION 3.20)
project(Sieve_of_Eratosthenes_CUDA)

set(CMAKE_CXX_STANDARD 14)

add_executable(Sieve_of_Eratosthenes_CUDA
        main.cpp)

target_link_libraries(Sieve_of_Eratosthenes_CUDA  pthread)