#include <vector>
#include <omp.h>

#include "omp_sum_sub.hpp"

// parallelSum function sums all the elements of a vector in parallel
// vec: vector of doubles
template <typename T>
double parallelSum(const std::vector<T>& vec) {
    double sum = 0.0;

    #pragma omp parallel for reduction(+:sum)
    for (size_t i = 0; i < vec.size(); ++i) {
        sum += vec[i];
    }

    return sum;
}

// prefixSum function calculates the prefix sum of a vector in parallel
template <typename T>
void prefixSum(std::vector<T>& arr) {
    int n = arr.size();
    std::vector<T> prefix_sum(n);  // Change std::vector<int> to std::vector<T>

    #pragma omp parallel
    {
        T sum = 0;  // Change int sum to T sum

        #pragma omp for
        for (int i = 0; i < n; ++i) {
            sum += arr[i];
            prefix_sum[i] = sum;
        }
    }

    arr = prefix_sum;
}

/* Subtractions */

// parallelSub function subtracts all the elements of a vector in parallel
template <typename T>
double parallelSub(const std::vector<T>& vec) {
    double sub = 0.0;

    #pragma omp parallel for reduction(-:sub)
    for (size_t i = 0; i < vec.size(); ++i) {
        sub -= vec[i];
    }

    return sub;
}

// prefixSub function calculates the prefix subtraction of a vector in parallel
template <typename T>
void prefixSub(std::vector<T>& arr) {
    int n = arr.size();
    std::vector<T> prefix_sub(n);  // Change std::vector<int> to std::vector<T>

    #pragma omp parallel
    {
        T sub = 0;  // Change int sub to T sub

        #pragma omp for
        for (int i = 0; i < n; ++i) {
            sub -= arr[i];
            prefix_sub[i] = sub;
        }
    }

    arr = prefix_sub;
}

// Explicit template instantiation
template void prefixSum<int>(std::vector<int>&);
template void prefixSum<float>(std::vector<float>&);
template void prefixSum<double>(std::vector<double>&);
template void prefixSum<long>(std::vector<long>&);

template double parallelSum<int>(const std::vector<int>&);
template double parallelSum<float>(const std::vector<float>&);
template double parallelSum<double>(const std::vector<double>&);
template double parallelSum<long>(const std::vector<long>&);

template void prefixSub<int>(std::vector<int>&);
template void prefixSub<float>(std::vector<float>&);
template void prefixSub<double>(std::vector<double>&);
template void prefixSub<long>(std::vector<long>&);

template double parallelSub<int>(const std::vector<int>&);
template double parallelSub<float>(const std::vector<float>&);
template double parallelSub<double>(const std::vector<double>&);
template double parallelSub<long>(const std::vector<long>&);
