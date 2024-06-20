#include <gtest/gtest.h>
#include <vector>

#include "matrix_multipy.hpp"

// Example test for matrix multiplication function
TEST(MatrixMultiplyTest, HandlesEmptyMatrix) {
    std::vector<std::vector<int>> mat1;
    std::vector<std::vector<int>> mat2;
    std::vector<std::vector<int>> result;
    EXPECT_NO_THROW(matrixMultiply(mat1, mat2, result));
}

TEST(MatrixMultiplyTest, HandlesSingleElementMatrix) {
    std::vector<std::vector<int>> mat1 = {{1}};
    std::vector<std::vector<int>> mat2 = {{2}};
    std::vector<std::vector<int>> result;
    matrixMultiply(mat1, mat2, result);
    EXPECT_EQ(result[0][0], 2);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
