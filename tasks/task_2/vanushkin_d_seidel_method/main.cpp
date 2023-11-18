// Copyright 2023 Vanushkin Dmitry
#include <gtest/gtest.h>
#include <vector>
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include "task_2/vanushkin_d_seidel_method/seidel_method.h"

void Prepare(DoubleMatrix& a, DoubleVector& b) {
    for (size_t i = 0; i < a.size(); i++) {
        double divider = a[i][i];

        for (size_t j = 0; j < a[0].size(); j++) {
            if (j != i) {
                a[i][j] /= -divider;
            } else {
                a[i][j] = 0;
            }
        }

        b[i] /= divider;
    }
}

TEST(seidel_method, sequential_test) {

    DoubleMatrix a = {
            {5, 0, 0},
            {0, 4, 0},
            {0, 7, 14},
    };

    DoubleVector b = {10, 2, 28};

    Prepare(a, b);

    auto result = SequentialSeidelMethod(a, b, 0.001);
    DoubleVector correctSolution = {2, 0.5, 1.75};

    ASSERT_TRUE(norm(result, correctSolution) <= 0.001);

}

TEST(seidel_method, seidel_view_preraring)

{
    DoubleMatrix a = {
            {5, 2, 5},
            {3, 10, 1},
            {0, 7, 10},
    };

    DoubleVector b = {1, 2, 3};

    Prepare(a, b);

    DoubleMatrix alpha = {
            {0, -0.4, -1},
            {-0.3, 0, -0.1},
            {0, -0.7, 0}
    };

    DoubleVector beta = {0.2, 0.2, 0.3};

    ASSERT_EQ(alpha, a);
    ASSERT_EQ(beta, b);
}

int main(int argc, char** argv) {
    boost::mpi::environment env(argc, argv);
    boost::mpi::communicator world;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();
    if (world.rank() != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }
    return RUN_ALL_TESTS();
}
