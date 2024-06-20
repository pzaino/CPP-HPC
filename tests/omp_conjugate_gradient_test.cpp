#include <gtest/gtest.h>
#include <vector>
#include "omp_conjugate_gradient.hpp"

// Helper function to generate a positive definite matrix
std::vector<std::vector<double>> generatePositiveDefiniteMatrix(int size) {
    std::vector<std::vector<double>> A(size, std::vector<double>(size, 0));
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            A[i][j] = (i == j) ? size + 1 : 1;
        }
    }
    return A;
}

TEST(ConjugateGradientTest, HandlesSimpleCase) {
    std::vector<std::vector<double>> A = {
        {4, 1},
        {1, 3}
    };
    std::vector<double> b = {1, 2};
    std::vector<double> x = {0, 0}; // Initial guess
    conjugateGradient(A, b, x, 1000, 1e-10);

    EXPECT_NEAR(x[0], 0.0909, 1e-4);
    EXPECT_NEAR(x[1], 0.6364, 1e-4);
}

TEST(ConjugateGradientTest, HandlesLargerCase) {
    std::vector<std::vector<double>> A = generatePositiveDefiniteMatrix(3);
    std::vector<double> b = {1, 2, 3};
    std::vector<double> x = {0, 0, 0}; // Initial guess
    conjugateGradient(A, b, x, 1000, 1e-10);

    std::vector<double> expected_x = {0.0769, 0.6154, 0.8462};

    for (int i = 0; i < 3; ++i) {
        EXPECT_NEAR(x[i], expected_x[i], 1e-4);
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
