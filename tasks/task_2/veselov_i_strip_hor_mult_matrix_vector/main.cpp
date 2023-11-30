// Copyright 2023 Veselov Ilya
#include <mpi.h>
#include <gtest/gtest.h>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/environment.hpp>
#include <boost/serialization/vector.hpp>
#include "task_2/veselov_i_strip_hor_mult_matrix_vector/stripHorMultMatrixVector.h"

TEST(Strip_Hor_Mult_Matrix_Vector, Test_) {
    /*//boost::mpi::communicator world;
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    std::vector<int> matrix, vec, resPar;
    const size_t rows = 3;
    const size_t cols = 3;
    //if (world.rank() == 0) {
    if (rank == 0) {
        matrix = {1, 2, 3,
                4, 5, 6,
                7, 8, 9};
        vec = {1, 2, 3};
    }
    resPar = matrix_vector_multiply_par(matrix, vec, rows, cols);
    //if (world.rank() == 0) {
    if (rank == 0) {
        std::vector<int> res = matrix_vector_multiply(matrix, vec, rows, cols);
        for (int i = 0; i < res.size(); i++) {
            ASSERT_EQ(res[i], resPar[i]);
            printf("res[%d] = %d , resPar[%d] = %d\n", i, res[i], i, resPar[i]);
        }
    }*/
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (size < 2) {
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    const size_t rows = 3;
    const size_t cols = 3;
    std::vector<std::vector<int>> matrix(rows, std::vector<int>(cols));
    std::vector<int> vec, resPar;
    int rowsPerProcess = rows / (size - 1);
    int extraRows = rows % (size - 1);
    if (rank == 0) {
        matrix[0] = {1, 2, 3};
        matrix[1] = {4, 5, 6};
        matrix[2] = {7, 8, 9};
        vec = {1, 2, 3};
    }
    resPar = matrix_vector_multiply_par(matrix, vec, rows, cols, rank, rowsPerProcess, extraRows);
    if (rank == 0) {
        std::vector<int> res = matrix_vector_multiply(matrix, vec, rows, cols);
        for (int i = 0; i < res.size(); i++) {
            ASSERT_EQ(res[i], resPar[i]);
            printf("res[%d] = %d , resPar[%d] = %d\n", i, res[i], i, resPar[i]);
        }
    }
}

TEST(Strip_Hor_Mult_Matrix_Vector, Test_random) {
    // boost::mpi::communicator world;
    /*int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    std::vector<int> matrix, vec, resPar;
    const size_t rows = 100;
    const size_t cols = 100;
    //if (world.rank() == 0) {
    if (rank == 0) {
        matrix = randomMatrix(rows, cols);
        vec = randomVector(cols);
    }
    resPar = matrix_vector_multiply_par(matrix, vec, rows, cols);
    //if (world.rank() == 0) {
    if (rank == 0) {
        std::vector<int> res = matrix_vector_multiply(matrix, vec, rows, cols);
        for (size_t i = 0; i < res.size(); ++i)
            ASSERT_EQ(res[i], resPar[i]);
    }*/
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (size < 2) {
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    const size_t rows = 100;
    const size_t cols = 100;
    std::vector<std::vector<int>> matrix(rows, std::vector<int>(cols));
    std::vector<int> vec, resPar;
    int rowsPerProcess = rows / (size - 1);
    int extraRows = rows % (size - 1);
    if (rank == 0) {
        matrix = randomMatrix(rows, cols);
        vec = randomVector(cols);
    }
    resPar = matrix_vector_multiply_par(matrix, vec, rows, cols, rank, rowsPerProcess, extraRows);
    if (rank == 0) {
        std::vector<int> res = matrix_vector_multiply(matrix, vec, rows, cols);
        for (size_t i = 0; i < res.size(); ++i)
            ASSERT_EQ(res[i], resPar[i]);
    }
}

TEST(Strip_Hor_Mult_Matrix_Vector, Test_random_positive) {
    // boost::mpi::communicator world;
    /*int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    std::vector<int> matrix, vec, resPar;
    const size_t rows = 75;
    const size_t cols = 50;
    //if (world.rank() == 0) {
    if (rank == 0) {
        matrix = randomMatrix(rows, cols, 1);
        vec = randomVector(cols, 1);
    }
    resPar = matrix_vector_multiply_par(matrix, vec, rows, cols);
    //if (world.rank() == 0) {
    if (rank == 0) {
        std::vector<int> res = matrix_vector_multiply(matrix, vec, rows, cols);
        for (size_t i = 0; i < res.size(); ++i)
            ASSERT_EQ(res[i], resPar[i]);
    }*/
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (size < 2) {
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    const size_t rows = 25;
    const size_t cols = 25;
    std::vector<std::vector<int>> matrix(rows, std::vector<int>(cols));
    std::vector<int> vec, resPar;
    int rowsPerProcess = rows / (size - 1);
    int extraRows = rows % (size - 1);
    if (rank == 0) {
        matrix = randomMatrix(rows, cols, 1);
        vec = randomVector(cols, 1);
    }
    resPar = matrix_vector_multiply_par(matrix, vec, rows, cols, rank, rowsPerProcess, extraRows);
    if (rank == 0) {
        std::vector<int> res = matrix_vector_multiply(matrix, vec, rows, cols);
        for (size_t i = 0; i < res.size(); ++i)
            ASSERT_EQ(res[i], resPar[i]);
    }
}

TEST(Strip_Hor_Mult_Matrix_Vector, Test_random_negative) {
    // boost::mpi::communicator world;
    /*int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    std::vector<int> matrix, vec, resPar;
    const size_t rows = 46;
    const size_t cols = 64;
    //if (world.rank() == 0) {
    if (rank == 0) {
        matrix = randomMatrix(rows, cols, -1000, 0);
        vec = randomVector(cols, -1000, 0);
    }
    resPar = matrix_vector_multiply_par(matrix, vec, rows, cols);
    //if (world.rank() == 0) {
    if (rank == 0) {
        std::vector<int> res = matrix_vector_multiply(matrix, vec, rows, cols);
        for (size_t i = 0; i < res.size(); ++i)
            ASSERT_EQ(res[i], resPar[i]);
    }*/
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (size < 2) {
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    const size_t rows = 46;
    const size_t cols = 64;
    std::vector<std::vector<int>> matrix(rows, std::vector<int>(cols));
    std::vector<int> vec, resPar;
    int rowsPerProcess = rows / (size - 1);
    int extraRows = rows % (size - 1);
    if (rank == 0) {
        matrix = randomMatrix(rows, cols, -1000, 0);
        vec = randomVector(cols, -1000, 0);
    }
    resPar = matrix_vector_multiply_par(matrix, vec, rows, cols, rank, rowsPerProcess, extraRows);
    if (rank == 0) {
        std::vector<int> res = matrix_vector_multiply(matrix, vec, rows, cols);
        for (size_t i = 0; i < res.size(); ++i)
            ASSERT_EQ(res[i], resPar[i]);
    }
}

TEST(Strip_Hor_Mult_Matrix_Vector, Test_null_matrix) {
    /*const size_t rows = 10;
    const size_t cols = 10;
    // boost::mpi::communicator world;
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    std::vector<int> matrix(rows * cols, 0), vec, resPar;
    //if (world.rank() == 0) {
    if (rank == 0) {
        vec = randomVector(cols);
    }
    resPar = matrix_vector_multiply_par(matrix, vec, rows, cols);
    //if (world.rank() == 0) {
    if (rank == 0) {
        std::vector<int> res = matrix_vector_multiply(matrix, vec, rows, cols);
        for (size_t i = 0; i < res.size(); ++i)
            ASSERT_EQ(res[i], resPar[i]);
    }*/
    const size_t rows = 10;
    const size_t cols = 10;
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (size < 2) {
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    std::vector<std::vector<int>> matrix(rows, std::vector<int>(cols, 0));
    std::vector<int> vec, resPar;
    int rowsPerProcess = rows / (size - 1);
    int extraRows = rows % (size - 1);
    if (rank == 0) {
        vec = randomVector(cols);
    }
    resPar = matrix_vector_multiply_par(matrix, vec, rows, cols, rank, rowsPerProcess, extraRows);
    if (rank == 0) {
        std::vector<int> res = matrix_vector_multiply(matrix, vec, rows, cols);
        for (size_t i = 0; i < res.size(); ++i)
            ASSERT_EQ(res[i], resPar[i]);
    }
}

TEST(Strip_Hor_Mult_Matrix_Vector, Test_matrix_diag) {
    /*const size_t rows = 15;
    const size_t cols = 10;
    //boost::mpi::communicator world;
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    std::vector<int> matrix(rows * cols), vec, resPar;
    //if (world.rank() == 0) {
    if (rank == 0) {
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
    //if (world.rank() == 0) {
    if (rank == 0) {
        std::vector<int> res = matrix_vector_multiply(matrix, vec, rows, cols);
        for (size_t i = 0; i < res.size(); ++i)
            ASSERT_EQ(res[i], resPar[i]);
    }*/
    const size_t rows = 15;
    const size_t cols = 15;
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (size < 2) {
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    std::vector<std::vector<int>> matrix(rows, std::vector<int>(cols, 0));
    std::vector<int> vec, resPar;
    int rowsPerProcess = rows / (size - 1);
    int extraRows = rows % (size - 1);
    if (rank == 0) {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (i == j)
                    matrix[i][j] = 1;
                else
                    matrix[i][j] = 0;
            }
        }
        vec = randomVector(cols);
    }
    resPar = matrix_vector_multiply_par(matrix, vec, rows, cols, rank, rowsPerProcess, extraRows);
    if (rank == 0) {
        std::vector<int> res = matrix_vector_multiply(matrix, vec, rows, cols);
        for (size_t i = 0; i < res.size(); ++i)
            ASSERT_EQ(res[i], resPar[i]);
    }
}

int main(int argc, char** argv) {
    // boost::mpi::environment env(argc, argv);
    // boost::mpi::communicator world;
    int result = 0;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners &listeners = ::testing::UnitTest::GetInstance()->listeners();
    if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, -1);
    // if (world.rank() != 0)
    //    delete listeners.Release(listeners.default_result_printer());
    result = RUN_ALL_TESTS();
    MPI_Finalize();
    return result;
    // return RUN_ALL_TESTS();
}
