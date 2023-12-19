// Copyright 2023 Ivanchenko Aleksei
#include <gtest/gtest.h>
#include <vector>
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include <algorithm>
#include <random>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>
#include "task_2/ivanchenko_a_gauss_horizontal/gauss_horizontal.h"

TEST(GAUSS_HORIZONTAL, test_sequential) {
    boost::mpi::communicator world;

    // Define the coefficient matrix A
    boost::numeric::ublas::matrix<double>A(3, 3);
    A(0, 0) = 1.0; A(0, 1) = 2.0; A(0, 2) = 3.0;
    A(1, 0) = 4.0; A(1, 1) = 5.0; A(1, 2) = 6.0;
    A(2, 0) = 7.0; A(2, 1) = 8.0; A(2, 2) = 10.0;

    // Define the right-hand side vector b
    boost::numeric::ublas::vector<double>b(3);
    b(0) = 1.0;
    b(1) = 2.0;
    b(2) = 3.0;

    boost::numeric::ublas::vector<double>expected(3);
    expected(0) = -1.0 / 3.0;
    expected(1) = 2.0 / 3.0;
    expected(2) = 0;

    boost::numeric::ublas::vector<double>actual = gaussSequentional(A, b);

    for (int i = 0; i < expected.size(); i++) {
        EXPECT_FLOAT_EQ(expected(i), actual(i));
    }
}
TEST(GAUSS_HORIZONTAL, test_sequential_2) {
    boost::mpi::communicator world;

    // Define the coefficient matrix A
    boost::numeric::ublas::matrix<double>A(16, 16);
    A(0, 0) = 1.0; A(0, 1) = 0;
    for (int i = 1; i < 15; i++) {
        A(i, i) = -907.0;
        A(i, i-1) = 450.0;
        A(i, i+1) = 450.0;
    }
    A(15, 14) = 0.0; A(15, 15) = 1.0;
    // Define the right-hand side vector b
    boost::numeric::ublas::vector<double>b(16);
    b(0) = 1.0;
    b(1) = 2.33777778;
    b(2) = 3.55111111;
    b(3) = 4.64000000;
    b(4) = 5.60444444;
    b(5) = 6.44444444;
    b(6) = 7.16000000;
    b(7) = 7.75111111;
    b(8) = 8.21777778;
    b(9) = 8.56000000;
    b(10) = 8.77777778;
    b(11) = 8.87111111;
    b(12) = 8.84000000;
    b(13) = 8.68444444;
    b(14) = 8.40444444;
    b(15) = 0.0;

    boost::numeric::ublas::vector<double>expected(16);
    expected(0) = 1.0;
    expected(1) = 0.80888889;
    expected(2) = 0.63555556;
    expected(3) = 0.48000000;
    expected(4) = 0.34222222;
    expected(5) = 0.22222222;
    expected(6) = 0.12000000;
    expected(7) = 0.03555556;
    expected(8) = -0.03111111;
    expected(9) = -0.08;
    expected(10) = -0.111111111;
    expected(11) = -0.12444444;
    expected(12) = -0.12000000;
    expected(13) = -0.097777778;
    expected(14) = -0.057777778;
    expected(15) = 0.0;

    // Solve the system of equations
    boost::numeric::ublas::vector<double>actual = gaussSequentional(A, b);

    for (int i = 0; i < expected.size(); i++) {
        EXPECT_FLOAT_EQ(expected(i), actual(i));
    }
}
TEST(GAUSS_HORIZONTAL, test_sequential_3) {
    boost::mpi::communicator world;

    // Define the coefficient matrix A
    boost::numeric::ublas::matrix<double>A(4, 4);
    A(0, 0) = 1.0; A(0, 1) = 2.0; A(0, 2) = 0; A(0, 3) = 0;
    A(1, 0) = 1.0; A(1, 1) = 3.0; A(1, 2) = 1.0; A(1, 3) = 0.0;
    A(2, 0) = 0; A(2, 1) = 0; A(2, 2) = 2.0; A(2, 3) = 3.0;
    A(3, 0) = 1.0; A(3, 1) = 1.0; A(3, 2) = 1.0; A(3, 3) = 1.0;

    // Define the right-hand side vector b
    boost::numeric::ublas::vector<double>b(4);
    b(0) = 1.0;
    b(1) = 2.0;
    b(2) = 3.0;
    b(3) = 4.0;

    boost::numeric::ublas::vector<double>expected(4);
    expected(0) = 7.0 / 2.0;
    expected(1) = -5.0 / 4.0;
    expected(2) = 9.0 / 4.0;
    expected(3) = -0.5;

    boost::numeric::ublas::vector<double>actual = gaussSequentional(A, b);

    for (int i = 0; i < expected.size(); i++) {
        EXPECT_FLOAT_EQ(expected(i), actual(i));
    }
}
TEST(GAUSS_HORIZONTAL, test_parallel_small) {
     // Define the coefficient matrix A
    boost::numeric::ublas::matrix<double>A(3, 3);
    A(0, 0) = 1.0; A(0, 1) = 2.0; A(0, 2) = 3.0;
    A(1, 0) = 4.0; A(1, 1) = 5.0; A(1, 2) = 6.0;
    A(2, 0) = 7.0; A(2, 1) = 8.0; A(2, 2) = 10.0;

    // Define the right-hand side vector b
    boost::numeric::ublas::vector<double>b(3);
    b(0) = 1.0;
    b(1) = 2.0;
    b(2) = 3.0;

    boost::numeric::ublas::vector<double>expected = gaussSequentional(A, b);
    boost::numeric::ublas::vector<double>actual = gaussParallel(A, b);

    for (int i = 0; i < expected.size(); i++) {
        EXPECT_FLOAT_EQ(expected(i), actual(i));
    }
}
TEST(GAUSS_HORIZONTAL, test_parallel_small_1) {
    // Define the coefficient matrix A
    boost::numeric::ublas::matrix<double>A(4, 4);
    A(0, 0) = 1.0; A(0, 1) = 2.0; A(0, 2) = 0; A(0, 3) = 0;
    A(1, 0) = 1.0; A(1, 1) = 3.0; A(1, 2) = 1.0; A(1, 3) = 0.0;
    A(2, 0) = 0; A(2, 1) = 0; A(2, 2) = 2.0; A(2, 3) = 3.0;
    A(3, 0) = 1.0; A(3, 1) = 1.0; A(3, 2) = 1.0; A(3, 3) = 1.0;

    // Define the right-hand side vector b
    boost::numeric::ublas::vector<double>b(4);
    b(0) = 1.0;
    b(1) = 2.0;
    b(2) = 3.0;
    b(3) = 4.0;

    boost::numeric::ublas::vector<double>expected = gaussSequentional(A, b);
    boost::numeric::ublas::vector<double>actual = gaussParallel(A, b);

    for (int i = 0; i < expected.size(); i++) {
        EXPECT_FLOAT_EQ(expected(i), actual(i));
    }
}
TEST(GAUSS_HORIZONTAL, test_parallel_small_11) {
    // Define the coefficient matrix A
    boost::numeric::ublas::matrix<double>A(4, 4);
    A(0, 0) = 9.0; A(0, 1) = 1.0; A(0, 2) = 10.0; A(0, 3) = 6.0;
    A(1, 0) = 7.0; A(1, 1) = 1.0; A(1, 2) = 7.0; A(1, 3) = 7.0;
    A(2, 0) = 8.0; A(2, 1) = 8.0; A(2, 2) = 7.0; A(2, 3) = 9.0;
    A(3, 0) = 9.0; A(3, 1) = 7.0; A(3, 2) = 5.0; A(3, 3) = 6.0;

    // Define the right-hand side vector b
    boost::numeric::ublas::vector<double>b(4);
    b(0) = 7.0;
    b(1) = 2.0;
    b(2) = 5.0;
    b(3) = 7.0;

    boost::numeric::ublas::vector<double>expected = gaussSequentional(A, b);
    boost::numeric::ublas::vector<double>actual = gaussParallel(A, b);

    for (int i = 0; i < expected.size(); i++) {
        EXPECT_FLOAT_EQ(expected(i), actual(i));
    }
}
TEST(GAUSS_HORIZONTAL, test_parallel_small_2) {
     // Define the coefficient matrix A
    boost::numeric::ublas::matrix<double>A(2, 2);
    A(0, 0) = 1.0; A(0, 1) = 2.0;
    A(1, 0) = 3.0; A(1, 1) = 1.0;
    // Define the right-hand side vector b
    boost::numeric::ublas::vector<double>b(2);
    b(0) = 3.0;
    b(1) = 4.0;

    boost::numeric::ublas::vector<double>expected = gaussSequentional(A, b);
    boost::numeric::ublas::vector<double>actual = gaussParallel(A, b);

    for (int i = 0; i < expected.size(); i++) {
        EXPECT_FLOAT_EQ(expected(i), actual(i));
    }
}
TEST(GAUSS_HORIZONTAL, test_parallel_big) {
    boost::mpi::communicator world;

    // Define the coefficient matrix A
    boost::numeric::ublas::matrix<double>A(16, 16);
    A(0, 0) = 1.0; A(0, 1) = 0;
    for (int i = 1; i < 15; i++) {
        A(i, i) = -907.0;
        A(i, i-1) = 450.0;
        A(i, i+1) = 450.0;
    }
    A(15, 14) = 0.0; A(15, 15) = 1.0;
    // Define the right-hand side vector b
    boost::numeric::ublas::vector<double>b(16);
    b(0) = 1.0;
    b(1) = 2.33777778;
    b(2) = 3.55111111;
    b(3) = 4.64000000;
    b(4) = 5.60444444;
    b(5) = 6.44444444;
    b(6) = 7.16000000;
    b(7) = 7.75111111;
    b(8) = 8.21777778;
    b(9) = 8.56000000;
    b(10) = 8.77777778;
    b(11) = 8.87111111;
    b(12) = 8.84000000;
    b(13) = 8.68444444;
    b(14) = 8.40444444;
    b(15) = 0.0;

    boost::numeric::ublas::vector<double>expected = gaussSequentional(A, b);
    boost::numeric::ublas::vector<double>actual = gaussParallel(A, b);

    for (int i = 0; i < expected.size(); i++) {
        EXPECT_FLOAT_EQ(expected(i), actual(i));
    }
}
TEST(GAUSS_HORIZONTAL, test_parallel_random_fill_0) {
    boost::mpi::communicator comm;
    size_t size = 4;
    boost::numeric::ublas::matrix<double>A(size, size);
    boost::numeric::ublas::vector<double>expected(size), b(size);
    if (comm.rank() == 0) {
        A = getRandomMatrix(size, size);
        b = getRandomVector(size);
    }
    boost::mpi::broadcast(comm, A, 0);
    boost::mpi::broadcast(comm, b, 0);
    boost::numeric::ublas::vector<double>actual = gaussParallel(A, b);
    if (comm.rank() == 0) {
        expected = gaussSequentional(A, b);
        for (int i = 0; i < expected.size(); i++) {
            EXPECT_FLOAT_EQ(expected(i), actual(i));
        }
    }
}
TEST(GAUSS_HORIZONTAL, test_parallel_random_fill) {
    boost::numeric::ublas::matrix<double>A = getRandomMatrix(17, 17);
    boost::numeric::ublas::vector<double>b = getRandomVector(17);

    boost::numeric::ublas::vector<double>expected = gaussSequentional(A, b);
    boost::numeric::ublas::vector<double>actual = gaussParallel(A, b);

    for (int i = 0; i < expected.size(); i++) {
        EXPECT_FLOAT_EQ(expected(i), actual(i));
    }
}
TEST(GAUSS_HORIZONTAL, test_parallel_random_fill_1) {
    boost::numeric::ublas::matrix<double>A = getRandomMatrix(19, 19);
    boost::numeric::ublas::vector<double>b = getRandomVector(19);
    boost::numeric::ublas::vector<double>expected = gaussSequentional(A, b);
    boost::numeric::ublas::vector<double>actual = gaussParallel(A, b);
    for (int i = 0; i < expected.size(); i++) {
        EXPECT_FLOAT_EQ(expected(i), actual(i));
    }
}
TEST(GAUSS_HORIZONTAL, test_parallel_random_fill_2) {
    boost::numeric::ublas::matrix<double>A = getRandomMatrix(24, 24);
    boost::numeric::ublas::vector<double>b = getRandomVector(24);

    boost::numeric::ublas::vector<double>expected = gaussSequentional(A, b);
    boost::numeric::ublas::vector<double>actual = gaussParallel(A, b);

    for (int i = 0; i < expected.size(); i++) {
        EXPECT_FLOAT_EQ(expected(i), actual(i));
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
