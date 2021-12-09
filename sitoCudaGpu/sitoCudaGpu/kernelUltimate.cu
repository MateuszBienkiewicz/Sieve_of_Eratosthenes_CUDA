#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <iostream>
#include <cmath>
#include <chrono>
using namespace std::chrono;
using namespace std;

__global__
void fun2(int n, int i, bool* tab) {
    int index; index = blockIdx.x * blockDim.x + threadIdx.x;
    int stride; stride = blockDim.x * gridDim.x;
    int k, j;
    if (index >= 2) j = index; else j = index + stride;
    for (; (k = j * i) <= n; j += stride)
            tab[k] = false;
}

__global__
void fun(int n, double sqrt_n, bool* tab) {
    int index; index = blockIdx.x * blockDim.x + threadIdx.x;
    int stride; stride = blockDim.x * gridDim.x;
    int i;
    const int num_streams = 4;
    cudaStream_t st;
    cudaStreamCreateWithFlags(&st, cudaStreamNonBlocking);
    if (index >= 2) i = index; else i = index + stride;
    for (; i <= sqrt_n; i += stride) {
            fun2 << <40000 / (2 * i) + 1, 1024, 0, st >> > (n, i, tab);
    }
    cudaStreamDestroy(st);

}

__global__ void sum(int n, bool* tab, int* result)
{
    int index; index = threadIdx.x;
    int stride; stride = blockDim.x;
    int i;
    __shared__ int wyniki[2048];
    for (int k = index; k < 2048; k += stride) wyniki[k] = 0;
    __syncthreads();
    for (i = index; i <= n; i += stride)
        if (tab[i]) {
            wyniki[index]++;
        }

    __syncthreads();

    int tid; tid = threadIdx.x;

    int step_size; step_size = 1;
    int number_of_threads; number_of_threads = blockDim.x;

    //__shared__ int fst;
    //__shared__ int snd;

    while (number_of_threads > 0)
    {
        if (tid < number_of_threads)
        {
            const auto fst = tid * step_size * 2;
            const auto snd = fst + step_size;
            wyniki[fst] += wyniki[snd];
        }

        step_size <<= 1;
        number_of_threads >>= 1;
        __syncthreads();
    }
    if (tid == 0) {
        wyniki[0] -= 2;
        result[0] = wyniki[0];
    }
}

bool InitCUDA(void)
{
    int count = 0;
    int i = 0;

    cudaGetDeviceCount(&count);
    if (count == 0) {
        fprintf(stderr, "There is no device.\n");
        return false;
    }

    for (i = 0; i < count; i++) {
        cudaDeviceProp prop;
        if (cudaGetDeviceProperties(&prop, i) == cudaSuccess) {
            if (prop.major >= 1) {
                printf("Device %d supports CUDA %d.%d\n", i, prop.major, prop.minor);
                printf("It has warp size %d, %d regs per block, %d bytes of shared memory per block, %d threads per block.\n",
                    prop.warpSize, prop.regsPerBlock, prop.sharedMemPerBlock, prop.maxThreadsPerBlock);
                printf("max Threads %d x %d x %d\n", prop.maxThreadsDim[0], prop.maxThreadsDim[1], prop.maxThreadsDim[2]);
                printf("max Grid %d x %d x %d\n", prop.maxGridSize[0], prop.maxGridSize[1], prop.maxGridSize[2]);
                printf("total constant memory %d\n", prop.totalConstMem);
                if (prop.concurrentKernels == 0)
                {
                    printf("> GPU does not support concurrent kernel execution\n");
                    printf(" CUDA kernel runs will be serialized\n");
                }
                break;
            }
        }
    }
    if (i == count) {
        fprintf(stderr, "There is no device supporting CUDA 1.x.\n");
        return false;
    }
    cudaSetDevice(i);
    return true;
}

__global__
void fill(int n, bool* tab) {
    int index; index = blockIdx.x * blockDim.x + threadIdx.x;
    int stride; stride = blockDim.x * gridDim.x;
    for (int i = index; i <= n; i += stride)
        tab[i] = true;
}

int main() {
    if (!InitCUDA())
    {
        return 0;
    }
    const int gpu_indices_for_count = 1024;
    int n;
    int* result;
    bool* tab;
    cout << "Podaj n: ";
    //cin >> n;
    n = 100000000;
    if (n > 3000000000)
    {
        cout << "input range too large, limit is 3*10e9";
        return 0;
    }
    auto start = high_resolution_clock::now();
    cudaMallocManaged(&result, sizeof(int));
    cudaMallocManaged(&tab, (n + 1) * sizeof(bool));
    fill<<<1000, gpu_indices_for_count >>>(n, tab);
    double sqrt_n = sqrt(n);
    cudaDeviceSynchronize();
    fun << <1, 1024 >> > (n, sqrt_n, tab);
    cudaDeviceSynchronize();
    sum << <1, gpu_indices_for_count >> > (n, tab, result);
    cudaDeviceSynchronize();
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << endl << "Liczby pierwsze:" << endl;
    
    for (int i = 2; i <= n; i++)
        if (tab[i]) {
            //cout << i << ", ";
        }
    cout << endl << "Czas: " << duration.count() << endl;
    cout << "Liczb pierwszych w zakresie [2, n] jest: " << result[0];
    cudaFree(result);
    cudaFree(tab);
    //cin >> n;
    return 0;
}
