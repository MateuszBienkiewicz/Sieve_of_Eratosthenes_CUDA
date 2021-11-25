#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <iostream>
#include <cmath>
#include <algorithm>
#include <chrono>
using namespace std::chrono;
using namespace std;

__global__
void fun(int n, double sqrt_n, bool* tab, int* result) {
    int index = threadIdx.x;
    int stride = blockDim.x;
    int k;
        for (int i = index; i <= sqrt_n; i += stride)
            //if (tab[i])
            if (i>1)
                for (int j = 2; (k = j * i) <= n; j++)
                    tab[k] = false;
        //for (int i = index; i <= n; i += stride)
            //if (tab[i]) {
                //result[0]++;
                //cout << i << ", ";
            //}
    //return wynik;
}

__global__
void count(int n, bool* tab, int* result) {
    int index = threadIdx.x;
    int stride = blockDim.x;
    for (int i = index; i <= n; i += stride)
        if (tab[i]) {
            result[index]++;
            //cout << i << ", ";
        }
}

__global__ void sum(int* input)
{
    const int tid = threadIdx.x;

    auto step_size = 1;
    int number_of_threads = blockDim.x;

    while (number_of_threads > 0)
    {
        if (tid < number_of_threads)
        {
            const auto fst = tid * step_size * 2;
            const auto snd = fst + step_size;
            input[fst] += input[snd];
        }

        step_size <<= 1;
        number_of_threads >>= 1;
        __syncthreads();
    }
}

int main() {
    const int gpu_indices_for_count = 128;
    int n;
    int* result;
    bool* tab;
    cout << "Podaj n: ";
    cin >> n;
    double sqrt_n = sqrt(n);
    cudaMallocManaged(&result, gpu_indices_for_count * sizeof(int));
    cudaMallocManaged(&tab, (n+1) * sizeof(bool));
    fill_n(result, gpu_indices_for_count, 0);
    fill_n(tab, n+1, true);
    auto start = high_resolution_clock::now();
    fun<<<1, gpu_indices_for_count >>>(n, sqrt_n, tab, result);
    cudaDeviceSynchronize();
    count<<<1, gpu_indices_for_count >>>(n, tab, result);
    cudaDeviceSynchronize();
    sum << <1, gpu_indices_for_count /2 >> > ( result);
    cudaDeviceSynchronize();
    result[0] -= 2;
    cout << endl <<"Liczby pierwsze:" << endl;
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    for (int i = 2; i <= n; i++)
        if (tab[i]) {
            cout << i << ", ";
        }
    cout << endl << "Czas: " << duration.count() << endl;
    cout << "Liczb pierwszych w zakresie [2, n] jest: " << result[0];
    cudaFree(result);
    cudaFree(tab);
    return 0;
}
