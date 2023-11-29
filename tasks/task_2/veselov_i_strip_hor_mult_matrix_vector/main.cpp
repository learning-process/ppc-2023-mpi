// Copyright 2023 Veselov Ilya
#include <gtest/gtest.h>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/environment.hpp>
#include <boost/serialization/vector.hpp>
#include "task_2/veselov_i_strip_hor_mult_matrix_vector/stripHorMultMatrixVector.h"

TEST(Strip_Hor_Mult_Matrix_Vector, Test_) {
    boost::mpi::communicator world;
    std::vector<int> matrix, vec, resPar;
    const size_t rows = 3;
    const size_t cols = 3;
    if (world.rank() == 0) {
        matrix = {1, 2, 3,
                4, 5, 6,
                7, 8, 9};
        vec = {1, 2, 3};
    }
    resPar = matrix_vector_multiply_par(matrix, vec, rows, cols);
    if (world.rank() == 0) {
        std::vector<int> res = matrix_vector_multiply(matrix, vec, rows, cols);
        for (int i = 0; i < res.size(); i++)
            ASSERT_EQ(res[i], resPar[i]);
    }
}

TEST(Strip_Hor_Mult_Matrix_Vector, Test_random) {
    boost::mpi::communicator world;
    std::vector<int> matrix, vec, resPar;
    const size_t rows = 100;
    const size_t cols = 100;
    if (world.rank() == 0) {
        matrix = randomMatrix(rows, cols);
        vec = randomVector(cols);
    }
    resPar = matrix_vector_multiply_par(matrix, vec, rows, cols);
    if (world.rank() == 0) {
        std::vector<int> res = matrix_vector_multiply(matrix, vec, rows, cols);
        for (size_t i = 0; i < res.size(); ++i)
            ASSERT_EQ(resPar[i], res[i]);
    }
}

TEST(Strip_Hor_Mult_Matrix_Vector, Test_random_positive) {
    boost::mpi::communicator world;
    std::vector<int> matrix, vec, resPar;
    const size_t rows = 75;
    const size_t cols = 50;
    if (world.rank() == 0) {
        matrix = randomMatrix(rows, cols, 1);
        vec = randomVector(cols, 1);
    }
    resPar = matrix_vector_multiply_par(matrix, vec, rows, cols);
    if (world.rank() == 0) {
        std::vector<int> res = matrix_vector_multiply(matrix, vec, rows, cols);
        for (size_t i = 0; i < res.size(); ++i)
            ASSERT_EQ(resPar[i], res[i]);
    }
}

TEST(Strip_Hor_Mult_Matrix_Vector, Test_random_negative) {
    boost::mpi::communicator world;
    std::vector<int> matrix, vec, resPar;
    const size_t rows = 46;
    const size_t cols = 64;
    if (world.rank() == 0) {
        matrix = randomMatrix(rows, cols, -1000, 0);
        vec = randomVector(cols, -1000, 0);
    }
    resPar = matrix_vector_multiply_par(matrix, vec, rows, cols);
    if (world.rank() == 0) {
        std::vector<int> res = matrix_vector_multiply(matrix, vec, rows, cols);
        for (size_t i = 0; i < res.size(); ++i)
            ASSERT_EQ(resPar[i], res[i]);
    }
}

TEST(Strip_Hor_Mult_Matrix_Vector, Test_null_matrix) {
    const size_t rows = 10;
    const size_t cols = 10;
    boost::mpi::communicator world;
    std::vector<int> matrix(rows * cols, 0), vec, resPar;
    if (world.rank() == 0) {
        vec = randomVector(cols);
    }
    resPar = matrix_vector_multiply_par(matrix, vec, rows, cols);
    if (world.rank() == 0) {
        std::vector<int> res = matrix_vector_multiply(matrix, vec, rows, cols);
        for (size_t i = 0; i < res.size(); ++i)
            ASSERT_EQ(resPar[i], res[i]);
    }
}

TEST(Strip_Hor_Mult_Matrix_Vector, Test_matrix_diag) {
    const size_t rows = 15;
    const size_t cols = 10;
    boost::mpi::communicator world;
    std::vector<int> matrix(rows * cols), vec, resPar;
    if (world.rank() == 0) {
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++) {
                if (i == j)
                    matrix[i * cols + j] = 1;
                else
                    matrix[i * cols + j] = 0;
            }
        vec = randomVector(cols);
    }
    resPar = matrix_vector_multiply_par(matrix, vec, rows, cols);
    if (world.rank() == 0) {
        std::vector<int> res = matrix_vector_multiply(matrix, vec, rows, cols);
        for (size_t i = 0; i < res.size(); ++i)
            ASSERT_EQ(resPar[i], res[i]);
    }
}

int main(int argc, char** argv) {
    boost::mpi::environment env(argc, argv);
    boost::mpi::communicator world;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners &listeners = ::testing::UnitTest::GetInstance()->listeners();
    if (world.rank() != 0)
        delete listeners.Release(listeners.default_result_printer());
    return RUN_ALL_TESTS();
}
