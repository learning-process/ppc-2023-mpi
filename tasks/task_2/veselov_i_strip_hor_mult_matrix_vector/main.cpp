// Copyright 2023 Veselov Ilya
#include <mpi.h>
#include <vector>
#include <gtest/gtest.h>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/environment.hpp>
#include <boost/serialization/vector.hpp>
#include "task_2/veselov_i_strip_hor_mult_matrix_vector/stripHorMultMatrixVector.h"

//TEST(Strip_Hor_Mult_Matrix_Vector, Test_) {
    /*int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (size < 2) {
        printf("%d\n", size);
        MPI_Abort(MPI_COMM_WORLD, -1);
    }
    const size_t rows = 3;
    const size_t cols = 3;
    std::vector<std::vector<int>> matrix(rows, std::vector<int>(cols));
    std::vector<int> vec, resPar;
    int rowsPerProcess = rows / (size);
    int extraRows = rows % (size);
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
    }*/
    /*int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    const size_t rows = 3, cols = 3;
    int *matrix = nullptr, *vec = nullptr, *resPar = nullptr;
    if (rank == 0) {
        vec = new int[cols];
        matrix = new int[rows * cols];
        resPar = new int[rows];
        for (int i = 0; i < rows * cols; i++) {
            matrix[i] = i + 1;
            printf("%d\n", matrix[i]);
        }
        for (int i = 0; i < cols; i++)
            vec[i] = i + 1;   
    }
    matrix_vector_multiply_par(matrix, vec, resPar, rows, cols, rank, size);
    for (int i = 0; i < rows; i++)
        printf("--%d\n", resPar[i]);
    if (rank == 0) {
        int* res = nullptr;
        res = new int[rows];
        matrix_vector_multiply(matrix, vec, res, rows, cols);
        for (int i = 0; i < rows; i++) {
            printf("%d %d", res[i], resPar[i]);
            ASSERT_EQ(res[i], resPar[i]);
        }
        delete[] res;
    }
    delete[] matrix;
    delete[] vec;
    delete[] resPar;*/
    /*int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    const size_t rows = 3, cols = 3;
    std::vector<int> matrix, vec, resPar;
    //if (rank == 0) {
        matrix.resize(rows * cols);
        matrix = {1, 2, 3, 4, 5, 6, 7, 8, 9};
        vec.resize(cols);
        vec = {1, 2, 3};
        resPar.resize(rows);
    //}
    matrix_vector_multiply_par(matrix, vec, resPar, rows, cols, rank, size);
    for (int i = 0; i < rows; i++)
        printf("--%d\n", resPar[i]);
    if (rank == 0) {
        std::vector<int> res(rows);
        matrix_vector_multiply(matrix, vec, res, rows, cols);
        for (int i = 0; i < res.size(); i++) {
            ASSERT_EQ(res[i], resPar[i]);
        }
    }
}*/

TEST(Strip_Hor_Mult_Matrix_Vector, Test_random) {
    /*int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    const size_t rows = 100;
    const size_t cols = 100;
    std::vector<std::vector<int>> matrix(rows, std::vector<int>(cols));
    std::vector<int> vec, resPar;
    int rowsPerProcess = rows / (size);
    int extraRows = rows % (size);
    if (rank == 0) {
        matrix = randomMatrix(rows, cols);
        vec = randomVector(cols);
    }
    resPar = matrix_vector_multiply_par(matrix, vec, rows, cols, rank, rowsPerProcess, extraRows);
    if (rank == 0) {
        std::vector<int> res = matrix_vector_multiply(matrix, vec, rows, cols);
        for (size_t i = 0; i < res.size(); ++i)
            ASSERT_EQ(res[i], resPar[i]);
    }*/
    /*int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    const size_t rows = 100, cols = 100;
    int *matrix = nullptr, *vec = nullptr, *resPar = nullptr;
    vec = new int[cols];
    matrix = new int[rows * cols];
    if (rank == 0) {
        randomMatrix(matrix, rows, cols);
        randomVector(vec, cols);
    }
    resPar = new int[rows];
    matrix_vector_multiply_par(matrix, vec, resPar, rows, cols, rank, size);
    if (rank == 0) {
        int* res;
        matrix_vector_multiply(matrix, vec, res, rows, cols);
        for (int i = 0; i < rows; i++) {
            ASSERT_EQ(res[i], resPar[i]);
        }
    }*/
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    printf("--%d--\n", size);
    const size_t rows = 4, cols = 4;
    std::vector<int> matrix(rows * cols), vec(cols), resPar(rows), res(rows);
    if (rank == 0) {
        //matrix.resize(rows * cols);
        matrix = randomMatrix(matrix, rows, cols);
        //vec.resize(cols);
        vec = randomVector(vec, cols);
        //resPar.resize(rows);
        //res.resize(rows);
    }
    //MPI_Bcast(matrix.data(), rows * cols, MPI_INT, 0, MPI_COMM_WORLD);
    //MPI_Bcast(vec.data(), cols, MPI_INT, 0, MPI_COMM_WORLD);
    res = matrix_vector_multiply_par(matrix, vec, resPar, rows, cols, rank, size);
    //MPI_Gather(resPar.data(), rows, MPI_INT, res.data(), rows, MPI_INT, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        std::vector<int> res_(rows);
        matrix_vector_multiply(matrix, vec, res_, rows, cols);
        for (int i = 0; i < res_.size(); i++) {
            ASSERT_EQ(res_[i], res[i]);
        }
    }
}

TEST(Strip_Hor_Mult_Matrix_Vector, Test_random_positive) {
    /*int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    const size_t rows = 25;
    const size_t cols = 25;
    std::vector<std::vector<int>> matrix(rows, std::vector<int>(cols));
    std::vector<int> vec, resPar;
    int rowsPerProcess = rows / (size);
    int extraRows = rows % (size);
    if (rank == 0) {
        matrix = randomMatrix(rows, cols, 1);
        vec = randomVector(cols, 1);
    }
    resPar = matrix_vector_multiply_par(matrix, vec, rows, cols, rank, rowsPerProcess, extraRows);
    if (rank == 0) {
        std::vector<int> res = matrix_vector_multiply(matrix, vec, rows, cols);
        for (size_t i = 0; i < res.size(); ++i)
            ASSERT_EQ(res[i], resPar[i]);
    }*/
    /*int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    const size_t rows = 25, cols = 25;
    int *matrix, *vec, *resPar;
    if (rank == 0) {
        randomMatrix(matrix, rows, cols, 1);
        randomVector(vec, cols, 1);
    }
    resPar = new int[rows];
    matrix_vector_multiply_par(matrix, vec, resPar, rows, cols, rank, size);
    if (rank == 0) {
        int* res;
        matrix_vector_multiply(matrix, vec, res, rows, cols);
        for (int i = 0; i < rows; i++) {
            ASSERT_EQ(res[i], resPar[i]);
        }
    }*/
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    const size_t rows = 8, cols = 8;
    std::vector<int> matrix(rows * cols), vec(cols), resPar(rows);
    if (rank == 0) {
        //matrix.resize(rows * cols);
        matrix = randomMatrix(matrix, rows, cols, 1);
        //vec.resize(cols);
        vec = randomVector(vec, cols, 1);
        //resPar.resize(rows);
    }
    resPar = matrix_vector_multiply_par(matrix, vec, resPar, rows, cols, rank, size);
    if (rank == 0) {
        std::vector<int> res(rows);
        matrix_vector_multiply(matrix, vec, res, rows, cols);
        for (int i = 0; i < res.size(); i++) {
            ASSERT_EQ(res[i], resPar[i]);
        }
    }
}

TEST(Strip_Hor_Mult_Matrix_Vector, Test_random_negative) {
    /*int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    const size_t rows = 46;
    const size_t cols = 64;
    std::vector<std::vector<int>> matrix(rows, std::vector<int>(cols));
    std::vector<int> vec, resPar;
    int rowsPerProcess = rows / (size);
    int extraRows = rows % (size);
    if (rank == 0) {
        matrix = randomMatrix(rows, cols, -1000, 0);
        vec = randomVector(cols, -1000, 0);
    }
    resPar = matrix_vector_multiply_par(matrix, vec, rows, cols, rank, rowsPerProcess, extraRows);
    if (rank == 0) {
        std::vector<int> res = matrix_vector_multiply(matrix, vec, rows, cols);
        for (size_t i = 0; i < res.size(); ++i)
            ASSERT_EQ(res[i], resPar[i]);
    }*/
    /*int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    const size_t rows = 46, cols = 64;
    int *matrix = nullptr, *vec = nullptr, *resPar = nullptr;
    vec = new int[cols];
    matrix = new int[rows * cols];
    if (rank == 0) {
        randomMatrix(matrix, rows, cols, -1000, 0);
        randomVector(vec, cols, -1000, 0);
    }
    resPar = new int[rows];
    matrix_vector_multiply_par(matrix, vec, resPar, rows, cols, rank, size);
    if (rank == 0) {
        int* res;
        matrix_vector_multiply(matrix, vec, res, rows, cols);
        for (int i = 0; i < rows; i++) {
            ASSERT_EQ(res[i], resPar[i]);
        }
    }*/
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    const size_t rows = 8, cols = 8;
    std::vector<int> matrix(rows * cols), vec(cols), resPar(rows);
    if (rank == 0) {
        //matrix.resize(rows * cols);
        matrix = randomMatrix(matrix, rows, cols, -1000, 0);
        //vec.resize(cols);
        vec = randomVector(vec, cols, -1000, 0);
        //resPar.resize(rows);
    }
    resPar = matrix_vector_multiply_par(matrix, vec, resPar, rows, cols, rank, size);
    if (rank == 0) {
        std::vector<int> res(rows);
        matrix_vector_multiply(matrix, vec, res, rows, cols);
        for (int i = 0; i < res.size(); i++) {
            ASSERT_EQ(res[i], resPar[i]);
        }
    }
}

TEST(Strip_Hor_Mult_Matrix_Vector, Test_null_matrix) {
    /*const size_t rows = 10;
    const size_t cols = 10;
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    std::vector<std::vector<int>> matrix(rows, std::vector<int>(cols, 0));
    std::vector<int> vec, resPar;
    int rowsPerProcess = rows / (size);
    int extraRows = rows % (size);
    if (rank == 0) {
        vec = randomVector(cols);
    }
    resPar = matrix_vector_multiply_par(matrix, vec, rows, cols, rank, rowsPerProcess, extraRows);
    if (rank == 0) {
        std::vector<int> res = matrix_vector_multiply(matrix, vec, rows, cols);
        for (size_t i = 0; i < res.size(); ++i)
            ASSERT_EQ(res[i], resPar[i]);
    }*/
    /*int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    const size_t rows = 10, cols = 10;
    int *matrix = nullptr, *vec = nullptr, *resPar = nullptr;
    vec = new int[cols];
    matrix = new int[rows * cols];
    if (rank == 0) {
        matrix = new int[rows * cols];
        for (int i = 0; i < rows * cols; i++)
            matrix[i] = 0;
        randomVector(vec, cols);
    }
    resPar = new int[rows];
    matrix_vector_multiply_par(matrix, vec, resPar, rows, cols, rank, size);
    if (rank == 0) {
        int* res;
        matrix_vector_multiply(matrix, vec, res, rows, cols);
        for (int i = 0; i < rows; i++) {
            ASSERT_EQ(res[i], resPar[i]);
        }
    }*/
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    const size_t rows = 10, cols = 10;
    std::vector<int> matrix(rows * cols, 0), vec(cols), resPar(rows);
    if (rank == 0) {
        //matrix.resize(rows * cols, 0);
        //vec.resize(cols);
        vec = randomVector(vec, cols);
        //resPar.resize(rows);
    }
    resPar = matrix_vector_multiply_par(matrix, vec, resPar, rows, cols, rank, size);
    if (rank == 0) {
        std::vector<int> res(rows);
        matrix_vector_multiply(matrix, vec, res, rows, cols);
        for (int i = 0; i < res.size(); i++) {
            ASSERT_EQ(res[i], resPar[i]);
        }
    }
}

TEST(Strip_Hor_Mult_Matrix_Vector, Test_matrix_diag) {
    /*const size_t rows = 15;
    const size_t cols = 15;
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    std::vector<std::vector<int>> matrix(rows, std::vector<int>(cols, 0));
    std::vector<int> vec, resPar;
    int rowsPerProcess = rows / (size);
    int extraRows = rows % (size);
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
    }*/
    /*int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    const size_t rows = 15, cols = 15;
    int *matrix = nullptr, *vec = nullptr, *resPar = nullptr;
    vec = new int[cols];
    matrix = new int[rows * cols];
    if (rank == 0) {
        matrix = new int[rows * cols];
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++) {
                if (i == j)
                    matrix[i * rows + j] = 1;
                else
                    matrix[i * cols + j] = 0;    
            }
        randomVector(vec, cols);
    }
    resPar = new int[rows];
    matrix_vector_multiply_par(matrix, vec, resPar, rows, cols, rank, size);
    if (rank == 0) {
        int* res;
        matrix_vector_multiply(matrix, vec, res, rows, cols);
        for (int i = 0; i < rows; i++) {
            ASSERT_EQ(res[i], resPar[i]);
        }
    }*/
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    const size_t rows = 16, cols = 16;
    std::vector<int> matrix(rows * cols), vec(cols), resPar(rows);
    if (rank == 0) {
        //matrix.resize(rows * cols, 0);
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++) {
                if (i == j)
                    matrix[i * rows + j] = 1;
            }
        //vec.resize(cols);
        vec = randomVector(vec, cols);
        //resPar.resize(rows);
    }
    resPar = matrix_vector_multiply_par(matrix, vec, resPar, rows, cols, rank, size);
    if (rank == 0) {
        std::vector<int> res(rows);
        matrix_vector_multiply(matrix, vec, res, rows, cols);
        for (int i = 0; i < res.size(); i++) {
            ASSERT_EQ(res[i], resPar[i]);
        }
    }
}

int main(int argc, char** argv) {
    // boost::mpi::environment env(argc, argv);
    // boost::mpi::communicator world;
    MPI_Init(&argc, &argv);
    int result = 0, rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners &listeners = ::testing::UnitTest::GetInstance()->listeners();
    // if (world.rank() != 0)
    //    delete listeners.Release(listeners.default_result_printer());
    if (rank != 0)
        delete listeners.Release(listeners.default_result_printer());
    result = RUN_ALL_TESTS();
    MPI_Finalize();
    return result;
    // return RUN_ALL_TESTS();
}
