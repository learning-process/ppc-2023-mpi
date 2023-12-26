// Copyright 2023 Kriseev Mikhail
#include <gtest/gtest.h>
#include <random>
#include <boost/mpi/environment.hpp>
#include <boost/mpi/collectives.hpp>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/timer.hpp>
#include "./csc_matrix_multiplication.h"

void initRandomCscMatrix(CscMatrix& a) {  // NOLINT
    double lambda = 0.05;
    std::random_device rd;
    std::mt19937 gen(rd());
    gen.seed(39857781);
    std::uniform_int_distribution<int> d(0, a.rows / 2);
    std::uniform_int_distribution<int> d_i(0, a.rows - 1);
    std::uniform_real_distribution<double> d_v(2, 10);
    std::vector<size_t> sizes;
    for (size_t i = 0; i < a.cols; ++i) {
        size_t size = static_cast<size_t>(d(gen));
        if (size > a.rows) {
            size = a.rows;
        }
        for (int j = 0; j < size; ++j) {
            a.setElement(i, d_i(gen), d_v(gen));
        }
    }
}

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

TEST(CSC_Matrix_Multiplication, random_matrices) {
    boost::mpi::communicator world;
    size_t cols_a = 20;
    size_t rows_a = 30;
    size_t cols_b = 500;
    size_t rows_b = cols_a;
    CscMatrix a(rows_a, cols_a, {}, {}, std::vector<size_t>(cols_a + 1, 0));
    CscMatrix b(rows_b, cols_b, {}, {}, std::vector<size_t>(cols_b + 1, 0));
    if (world.rank() == 0) {
        initRandomCscMatrix(a);
        initRandomCscMatrix(b);
    }
    boost::mpi::broadcast(world, a, 0);
    boost::mpi::broadcast(world, b, 0);
    std::cout << "matrices initialized\n";
    boost::mpi::timer timer;
    auto parallelResult = multiplyCscMatricesParallel(a, b);
    double parallelTime = timer.elapsed();

    if (world.rank() == 0) {
        timer.restart();
        auto sequentialResult = multiplyCscMatricesSequential(a, b);
        double sequentialTime = timer.elapsed();
        std::cout << "parallel time: " << parallelTime << "\n";
        std::cout << "sequential time: " << sequentialTime << "\n";
        ASSERT_EQ(sequentialResult, parallelResult);
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
