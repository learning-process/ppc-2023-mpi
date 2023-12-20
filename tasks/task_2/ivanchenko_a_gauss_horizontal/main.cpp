// Copyright 2023 Ivanchenko Aleksei
#include <gtest/gtest.h>
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>
#include "task_2/ivanchenko_a_gauss_horizontal/gauss_horizontal.h"

TEST(GAUSS_HORIZONTAL, test_sequential) {
    boost::mpi::communicator comm;
    if (comm.rank() == 0) {
        size_t size = 3;
        // Define the coefficient matrix A
        std::vector<double>A(size * size);
        A[0*size + 0] = 1.0; A[0*size + 1] = 2.0; A[0*size + 2] = 3.0;
        A[1*size + 0] = 4.0; A[1*size + 1] = 5.0; A[1*size + 2] = 6.0;
        A[2*size + 0] = 7.0; A[2*size + 1] = 8.0; A[2*size + 2] = 10.0;

        // Define the right-hand side vector b
        std::vector<double>b(size);
        b[0] = 1.0;
        b[1] = 2.0;
        b[2] = 3.0;

        std::vector<double>expected(size);
        expected[0] = -1.0 / 3.0;
        expected[1] = 2.0 / 3.0;
        expected[2] = 0;

        std::vector<double>actual = gaussSequentional(A, b);

        for (int i = 0; i < expected.size(); i++) {
            EXPECT_FLOAT_EQ(expected[i], actual[i]);
        }
    }
}
TEST(GAUSS_HORIZONTAL, test_sequential_2) {
    boost::mpi::communicator comm;
    if (comm.rank() == 0) {
        size_t size = 16;
        // Define the coefficient matrix A
        std::vector<double>A(size * size);
        A[0*size + 0] = 1.0; A[0*size + 1] = 0;
        for (int i = 1; i < 15; i++) {
            A[i*size + i] = -907.0;
            A[i*size + i-1] = 450.0;
            A[i*size + i+1] = 450.0;
        }
        A[15*size + 14] = 0.0; A[15*size + 15] = 1.0;
        // Define the right-hand side vector b
        std::vector<double>b(size);
        b[0] = 1.0;
        b[1] = 2.33777778;
        b[2] = 3.55111111;
        b[3] = 4.64000000;
        b[4] = 5.60444444;
        b[5] = 6.44444444;
        b[6] = 7.16000000;
        b[7] = 7.75111111;
        b[8] = 8.21777778;
        b[9] = 8.56000000;
        b[10] = 8.77777778;
        b[11] = 8.87111111;
        b[12] = 8.84000000;
        b[13] = 8.68444444;
        b[14] = 8.40444444;
        b[15] = 0.0;

        std::vector<double>expected(size);
        expected[0] = 1.0;
        expected[1] = 0.80888889;
        expected[2] = 0.63555556;
        expected[3] = 0.48000000;
        expected[4] = 0.34222222;
        expected[5] = 0.22222222;
        expected[6] = 0.12000000;
        expected[7] = 0.03555556;
        expected[8] = -0.03111111;
        expected[9] = -0.08;
        expected[10] = -0.111111111;
        expected[11] = -0.12444444;
        expected[12] = -0.12000000;
        expected[13] = -0.097777778;
        expected[14] = -0.057777778;
        expected[15] = 0.0;

        // Solve the system of equations
        std::vector<double>actual = gaussSequentional(A, b);

        for (int i = 0; i < expected.size(); i++) {
            EXPECT_FLOAT_EQ(expected[i], actual[i]);
        }
    }
}
TEST(GAUSS_HORIZONTAL, test_sequential_3) {
    boost::mpi::communicator comm;
    if (comm.rank() == 0) {
        size_t size = 4;
        // Define the coefficient matrix A
        std::vector<double>A(size * size);
        A[0*size + 0] = 1.0; A[0*size + 1] = 2.0; A[0*size + 2] = 0;   A[0*size + 3] = 0;
        A[1*size + 0] = 1.0; A[1*size + 1] = 3.0; A[1*size + 2] = 1.0; A[1*size + 3] = 0.0;
        A[2*size + 0] = 0;   A[2*size + 1] = 0;   A[2*size + 2] = 2.0; A[2*size + 3] = 3.0;
        A[3*size + 0] = 1.0; A[3*size + 1] = 1.0; A[3*size + 2] = 1.0; A[3*size + 3] = 1.0;

        // Define the right-hand side vector b
        std::vector<double>b(size);
        b[0] = 1.0;
        b[1] = 2.0;
        b[2] = 3.0;
        b[3] = 4.0;

        std::vector<double>expected(size);
        expected[0] = 7.0 / 2.0;
        expected[1] = -5.0 / 4.0;
        expected[2] = 9.0 / 4.0;
        expected[3] = -0.5;

        std::vector<double>actual = gaussSequentional(A, b);

        for (int i = 0; i < expected.size(); i++) {
            EXPECT_FLOAT_EQ(expected[i], actual[i]);
        }
    }
}
TEST(GAUSS_HORIZONTAL, test_parallel_random_fill_3) {
    boost::mpi::communicator comm;
    size_t size = 3;
    std::vector<double> A(size * size);
    std::vector<double> b(size), expected(size);
    if (comm.rank() == 0) {
        A = getRandomMatrix(size, size);
        b = getRandomVector(size);
    }

    std::vector<double>actual = gaussParallel(A, b);

    if (comm.rank() == 0) {
        expected = gaussSequentional(A, b);
        for (int i = 0; i < expected.size(); i++) {
            EXPECT_FLOAT_EQ(expected[i], actual[i]);
        }
    }
}
TEST(GAUSS_HORIZONTAL, test_parallel_random_fill_19) {
    boost::mpi::communicator comm;
    size_t size = 19;
    std::vector<double> A(size * size);
    std::vector<double> b(size), expected(size);
    if (comm.rank() == 0) {
        A = getRandomMatrix(size, size);
        b = getRandomVector(size);
    }

    std::vector<double>actual = gaussParallel(A, b);

    if (comm.rank() == 0) {
        expected = gaussSequentional(A, b);
        for (int i = 0; i < expected.size(); i++) {
            EXPECT_FLOAT_EQ(expected[i], actual[i]);
        }
    }
}
TEST(GAUSS_HORIZONTAL, test_parallel_random_fill_24) {
    boost::mpi::communicator comm;
    size_t size = 24;
    std::vector<double> A(size * size);
    std::vector<double> b(size), expected(size);
    if (comm.rank() == 0) {
        A = getRandomMatrix(size, size);
        b = getRandomVector(size);
    }

    std::vector<double>actual = gaussParallel(A, b);

    if (comm.rank() == 0) {
        expected = gaussSequentional(A, b);
        for (int i = 0; i < expected.size(); i++) {
            EXPECT_FLOAT_EQ(expected[i], actual[i]);
        }
    }
}
TEST(GAUSS_HORIZONTAL, test_parallel_random_fill_29) {
    boost::mpi::communicator comm;
    size_t size = 29;
    std::vector<double> A(size * size);
    std::vector<double> b(size), expected(size);
    if (comm.rank() == 0) {
        A = getRandomMatrix(size, size);
        b = getRandomVector(size);
    }

    std::vector<double>actual = gaussParallel(A, b);

    if (comm.rank() == 0) {
        expected = gaussSequentional(A, b);
        for (int i = 0; i < expected.size(); i++) {
            EXPECT_FLOAT_EQ(expected[i], actual[i]);
        }
    }
}
int main(int argc, char** argv) {
    boost::mpi::environment env(argc, argv);
    boost::mpi::communicator comm;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();
    if (comm.rank() != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }
    return RUN_ALL_TESTS();
}
