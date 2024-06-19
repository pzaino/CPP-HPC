
#include <functional>
#include <vector>
#include <omp.h>

#include "omp_quicksort.hpp"

#ifdef ZFP_RECURSIVE_FORM
// Recursive form of quicksort with OpenMP and custom
// evaluation function
template <typename T>
void quicksort(std::vector<T>& arr, int left, int right, 
               std::function<bool(T, T)> eval)
{
    if (left < right) 
    {
        int pivot = arr[right];
        int partitionIndex = left;

        #pragma omp parallel for
        for (int i = left; i < right; ++i) 
        {
            if (eval(arr[i], pivot)) 
            {
                std::swap(arr[i], arr[partitionIndex]);
                ++partitionIndex;
            }
        }
        std::swap(arr[partitionIndex], arr[right]);

        #pragma omp parallel sections
        {
            #pragma omp section
            {
                quicksort(arr, left, partitionIndex - 1, eval);
            }
            #pragma omp section
            {
                quicksort(arr, partitionIndex + 1, right, eval);
            }
        }
    }
}
#else
// Iterative form of quicksort with OpenMP and custom
// evaluation function
template <typename T>
void quicksort(std::vector<T>& arr, int left, int right, 
               std::function<bool(T, T)> eval) 
{
    std::vector<int> stack(right - left + 1);
    int top = -1;

    stack[++top] = left;
    stack[++top] = right;

    #pragma omp parallel sections
    {
        #pragma omp section
        {
            while (top >= 0) 
            {
                right = stack[top--];
                left = stack[top--];

                int pivot = arr[right];
                int partitionIndex = left;

                for (int i = left; i < right; ++i) 
                {
                    if (eval(arr[i], pivot)) {
                        std::swap(arr[i], arr[partitionIndex]);
                        ++partitionIndex;
                    }
                }
                std::swap(arr[partitionIndex], arr[right]);

                if (partitionIndex - 1 > left) 
                {
                    stack[++top] = left;
                    stack[++top] = partitionIndex - 1;
                }

                if (partitionIndex + 1 < right) 
                {
                    stack[++top] = partitionIndex + 1;
                    stack[++top] = right;
                }
            }
        }
    }
}
#endif

// Explicit template instantiation
template void quicksort<int>(std::vector<int>&, int, int, 
                             std::function<bool(int, int)>);
template void quicksort<float>(std::vector<float>&, int, int, 
                               std::function<bool(float, float)>);
template void quicksort<double>(std::vector<double>&, int, int, 
                                std::function<bool(double, double)>);
template void quicksort<long>(std::vector<long>&, int, int,
                                std::function<bool(long, long)>);   

