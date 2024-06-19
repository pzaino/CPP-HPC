
#include <functional>
#include <vector>
#include <omp.h>

#include "omp_mergesort.hpp"


template <typename T>
void merge(std::vector<T>& arr, int left, int mid, int right, 
           std::function<bool(T, T)> eval) 
{
    int n1 = mid - left + 1;
    int n2 = right - mid;

    std::vector<T> L(n1);
    std::vector<T> R(n2);

    for (int i = 0; i < n1; ++i) 
    {
        L[i] = arr[left + i];
    }
    for (int j = 0; j < n2; ++j) 
    {
        R[j] = arr[mid + 1 + j];
    }

    int i = 0;
    int j = 0;
    int k = left;

    while (i < n1 && j < n2) 
    {
        if (eval(L[i], R[j])) 
        {
            arr[k] = L[i];
            ++i;
        } 
        else 
        {
            arr[k] = R[j];
            ++j;
        }
        ++k;
    }

    while (i < n1) 
    {
        arr[k] = L[i];
        ++i;
        ++k;
    }

    while (j < n2) 
    {
        arr[k] = R[j];
        ++j;
        ++k;
    }
}

// Explicit template instantiation
template void merge<int>(std::vector<int>&, int, int, int, std::function<bool(int, int)>);
template void merge<float>(std::vector<float>&, int, int, int, std::function<bool(float, float)>);
template void merge<double>(std::vector<double>&, int, int, int, std::function<bool(double, double)>);
template void merge<long>(std::vector<long>&, int, int, int, std::function<bool(long, long)>);

#ifdef ZFP_RECURSIVE_FORM
// Recursive form of quicksort with OpenMP and custom
// evaluation function
template <typename T>
void mergeSort(std::vector<T>& arr, int left, int right, 
               std::function<bool(T, T)> eval) 
{
    if (left < right) 
    {
        int mid = left + (right - left) / 2;

        #pragma omp parallel sections
        {
            #pragma omp section
            {
                mergeSort(arr, left, mid, eval);
            }
            #pragma omp section
            {
                mergeSort(arr, mid + 1, right, eval);
            }
        }

        merge(arr, left, mid, right, eval);
    }
}
#else
// Iterative form of quicksort with OpenMP and custom
// evaluation function
template <typename T>
void mergeSort(std::vector<T>& arr, int left, int right, 
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

                int mid = left + (right - left) / 2;

                if (left < right) 
                {
                    stack[++top] = left;
                    stack[++top] = mid;
                    stack[++top] = mid + 1;
                    stack[++top] = right;
                }
            }
        }
        #pragma omp section
        {
            while (top >= 0) 
            {
                right = stack[top--];
                left = stack[top--];
                int mid = left + (right - left) / 2;

                merge(arr, left, mid, right, eval);
            }
        }
    }
}
#endif

// Explicit template instantiation
template void mergeSort<int>(std::vector<int>&, int, int, std::function<bool(int, int)>);
template void mergeSort<float>(std::vector<float>&, int, int, std::function<bool(float, float)>);
template void mergeSort<double>(std::vector<double>&, int, int, std::function<bool(double, double)>);
template void mergeSort<long>(std::vector<long>&, int, int, std::function<bool(long, long)>);
