// Copyright 2023 Kriseev Mikhail
#include <gtest/gtest.h>
#include <random>
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/timer.hpp>
#include "./csc_matrix_multiplication.h"

TEST(CSC_Matrix_Multiplication, matrix_1x1) {
    boost::mpi::communicator world;
    CscMatrix a = {1, 1, {3}, {0}, {0, 1}};
    CscMatrix b = {1, 1, {4}, {0}, {0, 1}};
    CscMatrix expected = {1, 1, {12}, {0}, {0, 1}};
    auto actualParallel = multiplyCscMatricesParallel(a, b);

    if (world.rank() == 0) {
        CscMatrix actualSequential = multiplyCscMatricesSequential(a, b);
        std::cout << "expected: \n";
        expected.print();
        std::cout << "actual (seq): \n";
        actualSequential.print();
        std::cout << "actual (par): \n";
        actualParallel.print();
        ASSERT_EQ(expected, actualSequential);
        ASSERT_EQ(expected, actualParallel);
    }
}
TEST(CSC_Matrix_Multiplication, matrix_4x3) {
    boost::mpi::communicator world;
    CscMatrix a = {
        4, 3, {1, 3, 5, 2, 4, 8, 7, 3}, {0, 2, 3, 1, 3, 0, 1, 2}, {0, 3, 5, 8}};
    CscMatrix b = {
        3, 4, {2, 3, 1, 2, 1, 4, 3}, {1, 0, 1, 2, 0, 2, 0}, {0, 1, 4, 6, 7}};

    CscMatrix expected = {4,
                          4,
                          {4, 8, 19, 16, 15, 19, 33, 28, 15, 5, 3, 9, 15},
                          {1, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 2, 3},
                          {0, 2, 6, 10, 13}};
    auto actualParallel = multiplyCscMatricesParallel(a, b);

    if (world.rank() == 0) {
        CscMatrix actualSequential = multiplyCscMatricesSequential(a, b);
        std::cout << "expected: \n";
        expected.print();
        std::cout << "actual (seq): \n";
        actualSequential.print();
        std::cout << "actual (par): \n";
        actualParallel.print();
        ASSERT_EQ(expected, actualSequential);
        ASSERT_EQ(expected, actualParallel);
    }
}

TEST(CSC_Matrix_Multiplication, identity_matrix) {
    boost::mpi::communicator world;
    CscMatrix a = {
        4, 3, {1, 3, 5, 2, 4, 8, 7, 3}, {0, 2, 3, 1, 3, 0, 1, 2}, {0, 3, 5, 8}};
    CscMatrix b = {3, 3, {1, 1, 1}, {0, 1, 2}, {0, 1, 2, 3}};

    CscMatrix expected(a);
    auto actualParallel = multiplyCscMatricesParallel(a, b);

    if (world.rank() == 0) {
        CscMatrix actualSequential = multiplyCscMatricesSequential(a, b);
        std::cout << "expected: \n";
        expected.print();
        std::cout << "actual (seq): \n";
        actualSequential.print();
        std::cout << "actual (par): \n";
        actualParallel.print();
        ASSERT_EQ(expected, actualSequential);
        ASSERT_EQ(expected, actualParallel);
    }
}

TEST(CSC_Matrix_Multiplication, incompatible_matrices) {
    boost::mpi::communicator world;
    CscMatrix a = {
        4, 3, {1, 3, 5, 2, 4, 8, 7, 3}, {0, 2, 3, 1, 3, 0, 1, 2}, {0, 3, 5, 8}};
    CscMatrix b = {
        4, 3, {1, 3, 5, 2, 4, 8, 7, 3}, {0, 2, 3, 1, 3, 0, 1, 2}, {0, 3, 5, 8}};

    CscMatrix expected = {4,
                          4,
                          {4, 8, 19, 16, 15, 19, 33, 28, 15, 5, 3, 9, 15},
                          {1, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 2, 3},
                          {0, 2, 6, 10, 13}};
    ASSERT_ANY_THROW(multiplyCscMatricesParallel(a, b));

    if (world.rank() == 0) {
        ASSERT_ANY_THROW(CscMatrix actualSequential =
                             multiplyCscMatricesSequential(a, b));
    }
}

TEST(CSC_Matrix_Multiplication, matrix_vector_product) {
    boost::mpi::communicator world;
    CscMatrix a = {
        4, 3, {1, 3, 5, 2, 4, 8, 7, 3}, {0, 2, 3, 1, 3, 0, 1, 2}, {0, 3, 5, 8}};
    CscMatrix b = {3, 1, {5, 7}, {0, 2}, {0, 2}};

    CscMatrix expected = {4, 1, {61, 49, 36, 25}, {0, 1, 2, 3}, {0, 4}};
    auto actualParallel = multiplyCscMatricesParallel(a, b);

    if (world.rank() == 0) {
        CscMatrix actualSequential = multiplyCscMatricesSequential(a, b);
        std::cout << "expected: \n";
        expected.print();
        std::cout << "actual (seq): \n";
        actualSequential.print();
        std::cout << "actual (par): \n";
        actualParallel.print();
        ASSERT_EQ(expected, actualSequential);
        ASSERT_EQ(expected, actualParallel);
    }
}

int main(int argc, char** argv) {
    boost::mpi::environment env(argc, argv);
    boost::mpi::communicator world;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();
    if (world.rank() != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }
    return RUN_ALL_TESTS();
}
