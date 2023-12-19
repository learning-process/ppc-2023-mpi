// Copyright 2023 Morgachev Stepan
#include <gtest/gtest.h>
#include "./conjugate_gradient_method.h"

TEST(Conjugate_Gradient_Method, Test_Scalar_Multiply) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<double> firstVector = { 2, 4, 6 };
    std::vector<double> secondVector = { 0, -1, 3 };
    double result = 14;

    if (rank == 0) {
        double scalar = scalarMultiply(firstVector, secondVector);
        ASSERT_EQ(result, scalar);
    }
}

TEST(Conjugate_Gradient_Method, Test_Multiply_Matrix_To_Vector) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<double> vector = { 1, 1, 1 };
    std::vector<double> matrix = { 0, 2, -1, 3, 1, 2, 0, 5, 2 };
    std::vector<double> result = { 1, 6, 7 };

    if (rank == 0) {
        std::vector<double> multiply = multiplyMatrixToVector(matrix, vector);
        for (int i = 0; i < multiply.size(); i++) {
            ASSERT_EQ(result[i], multiply[i]);
        }
    }
}

TEST(Conjugate_Gradient_Method, Test_Serial_Conjugate_Method) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<double> vector = {7, 4};
    std::vector<double> matrix = {1, 5, 3, -2};
    std::vector<double> answer = {2, 1};

    if (rank == 0) {
        std::vector<double> conjugateMethod =
            serialConjugateGradient(matrix, vector, 2);
        for (int i = 0; i < 2; i++) {
            ASSERT_NEAR(answer[i], conjugateMethod[i], 0.5);
        }
    }
}

TEST(Conjugate_Gradient_Method, Test_Parallel_Conjugate_Method) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int size = 2;
    std::vector<double> vector = {7, 4};
    std::vector<double> matrix = { 1, 5, 3, -2 };
    std::vector<double> answer = { 2, 1 };

    std::vector<double> parRes =
        parallelConjugateGradient(matrix, vector, size);
    if (rank == 0) {
        for (size_t i = 0; i < parRes.size(); i++)
            ASSERT_NEAR(answer[i], parRes[i], 1);
    }
}

TEST(Conjugate_Gradient_Method, Test_Serial_And_Parallel) {
    int rank;
    int size = 30;
    std::vector<double> vector;
    std::vector<double> matrix;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    vector = fillVectorRandomNumbers(size, 1, 20);
    matrix = fillMatrixRandomNumbers(size, 1, 20);

    std::vector<double> parRes =
        parallelConjugateGradient(matrix, vector, size);
    if (rank == 0) {
        std::vector<double> linRes =
            serialConjugateGradient(matrix, vector, size);
        for (size_t i = 0; i < linRes.size(); i++) {
            ASSERT_NEAR(linRes[i], parRes[i], 0.7);
        }
    }
}

int main(int argc, char** argv) {
    int result_code = 0;

    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, -1);
    result_code = RUN_ALL_TESTS();
    MPI_Finalize();

    return result_code;
}
