#include <gtest/gtest.h>
#include <vector>
#include <complex>

#include "fft.hpp"

// Example test for fft function
TEST(FFTTest, HandlesEmptyVector) {
    std::vector<std::complex<float>> vec;
    EXPECT_NO_THROW(fft(vec));
}

TEST(FFTTest, HandlesSingleElementVector) {
    std::vector<std::complex<float>> vec = {1};
    fft(vec);
    EXPECT_EQ(vec[0], std::complex<float>(1));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
