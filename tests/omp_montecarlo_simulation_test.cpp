#include <gtest/gtest.h>
#include "omp_montecarlo_simulation.hpp"  // Ensure this header includes the monteCarloPi function template

TEST(MonteCarloPiTest, HandlesFloat) {
    float pi_estimate = monteCarloPi<float>(1000000);
    EXPECT_NEAR(pi_estimate, 3.1415, 0.01);  // Tolerance of 0.01
}

TEST(MonteCarloPiTest, HandlesDouble) {
    double pi_estimate = monteCarloPi<double>(1000000);
    EXPECT_NEAR(pi_estimate, 3.1415, 0.01);  // Tolerance of 0.01
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
