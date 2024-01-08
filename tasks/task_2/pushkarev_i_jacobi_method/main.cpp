// Copyright 2023 Pushkarev Ilya

#include <gtest/gtest.h>
#include <mpi.h>
#include <iostream>
#include "./jacobi_method.h"

TEST(Parallel_Jacobi_method, Exception_matrix_wrong_size) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<double> mat{1, 2, 3, 4, 5, 6, 7, 8};
    std::vector<double> b{1, 1, 1};


    ASSERT_ANY_THROW(Jacobi(mat, b, 1));
}

TEST(Parallel_Jacobi_method, Exception_matrix_zeros) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<double> mat{1, 2, 3, 4, 5, 6, 7, 8, 0};
    std::vector<double> b{1, 1, 1};

    ASSERT_ANY_THROW(Jacobi(mat, b, 1));
}


TEST(Parallel_Jacobi_method, Task1_N2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<double> mat{10, 15, 5, 20};
    std::vector<double> b{0, 10};
    std::vector<double> res = Jacobi(mat, b, 1);
    std::vector<double> expect {0, 0.5};

    if (rank == 0) {
        for (int i = 0; i < res.size(); i++) {
            ASSERT_EQ(res[i], expect[i]);
        }
    }
}


TEST(Parallel_Jacobi_method, Task1_N3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<double> mat{1, 2, 3, 1, 2, 4, 1, 3, 3};
    std::vector<double> b{1, 5, 3};
    std::vector<double> res = Jacobi(mat, b, 1);

    std::vector<double> expect {1, 2.5, 1};

    if (rank == 0) {
        for (int i = 0; i < res.size(); i++) {
            ASSERT_EQ(res[i], expect[i]);
        }
    }
}

TEST(Parallel_Jacobi_method, Task1_N4) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<double> mat{1, 2, 3, 4, 1, 2, 4, 6, 1, 3, 3, 10, 0, 5, 7, 9};
    std::vector<double> b{2, 4, 6, 10};
    std::vector<double> res = Jacobi(mat, b, 1);

    std::vector<double> expect {2, 2, 2, 1.1111111111111112};

    if (rank == 0) {
        for (int i = 0; i < res.size(); i++) {
            ASSERT_EQ(res[i], expect[i]);
        }
    }
}

int main(int argc, char** argv) {
    int result_code = 0;
    int rankProc = 0;
    int numProc = 0;
    ::testing::InitGoogleTest(&argc, argv);

    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    if (MPI_Init(&argc, &argv) != MPI_SUCCESS) MPI_Abort(MPI_COMM_WORLD, -1);
    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);

    if (rankProc != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }

    result_code = RUN_ALL_TESTS();
    MPI_Finalize();

    return result_code;
}
