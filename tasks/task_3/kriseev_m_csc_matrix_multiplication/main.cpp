// Copyright 2023 Kriseev Mikhail
#include <gtest/gtest.h>
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include "./csc_matrix_multiplication.h"

TEST(CSC_Matrix_Multiplication, matrix_1x1) {
    /*CscMatrix a = {3, 3, {1, 4, 2, 3, 2}, {0, 2, 0, 1, 2}, {0, 2, 3, 5}};
    CscMatrix b = {3, 2, {2, 4, 3}, {0, 2, 1}, {0, 2, 3}};
    CscMatrix res = multiplyCscMatricesParallel(a, b);
    /*for (double v : res.values) {
        std::cout << v << " ";
    }
    std::cout << "\n";
    for (size_t r : res.rowIndices) {
        std::cout << r << " ";
    }
    std::cout << "\n";
    for (size_t c : res.columnPointers) {
        std::cout << c << " ";
    }
    std::cout << "\n";
    std::cout << "parallel: \n";
    multiplyCscMatricesParallel(a, b);  //.print();
    */
    boost::mpi::communicator world;
    CscMatrix a = {1, 1, {3}, {0}, {0, 1}};
    CscMatrix b = {1, 1, {4}, {0}, {0, 1}};
    CscMatrix expected = {1, 1, {12}, {0}, {0, 1}};
    auto actualParallel = multiplyCscMatricesParallel(a, b);

    if (world.rank() == 0) {
        CscMatrix actualSequential = multiplyCscMatricesSequential(a, b);
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
