cmake_minimum_required(VERSION 3.20)
project(Sieve_of_Eratosthenes_CUDA)

set(CMAKE_CXX_STANDARD 14)

add_executable(Sieve_of_Eratosthenes_CUDA
        main.cpp)

target_link_libraries(Sieve_of_Eratosthenes_CUDA  pthread)
target_link_libraries(Sieve_of_Eratosthenes_CUDA  pthread)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()