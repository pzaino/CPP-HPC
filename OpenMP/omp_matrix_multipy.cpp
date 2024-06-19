
#include <vector>
#include <omp.h>

#include "omp_matrix_multiply.hpp"

template <typename T>
void matrixMultiply(const std::vector<std::vector<T>>& A, 
                    const std::vector<std::vector<T>>& B,
                    std::vector<std::vector<T>>& C) {

    int n = A.size();
    int m = B.size();
    int p = B[0].size();

    #pragma omp parallel for collapse(2)
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < p; ++j) {
            C[i][j] = 0;
            for (int k = 0; k < m; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

// Explicit template instantiation
template void matrixMultiply<int>(const std::vector<std::vector<int>>&, 
                                  const std::vector<std::vector<int>>&, 
                                  std::vector<std::vector<int>>&);
template void matrixMultiply<float>(const std::vector<std::vector<float>>&, 
                                    const std::vector<std::vector<float>>&, 
                                    std::vector<std::vector<float>>&);
template void matrixMultiply<double>(const std::vector<std::vector<double>>&, 
                                     const std::vector<std::vector<double>>&, 
                                     std::vector<std::vector<double>>&);
template void matrixMultiply<long>(const std::vector<std::vector<long>>&,
                                    const std::vector<std::vector<long>>&,
                                    std::vector<std::vector<long>>&);
