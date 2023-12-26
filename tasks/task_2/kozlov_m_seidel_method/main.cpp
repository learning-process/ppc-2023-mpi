// Copyright 2023 Kozlov Mikhail
#include <gtest/gtest.h>
#include <vector>
#include "./seidel_method.h"
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>

TEST(GaussSeidelTest, SequentialVsMPI1) {
    // Test case 1
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int N = 3;
    const int MAX_ITER = 100;
    std::vector<std::vector<double>> A(N, std::vector<double>(N, 0.0));
    std::vector<double> b(N, 0.0);
    std::vector<double> xSequential(N, 0.0);

    A = {{4, -1, 0},
         {-1, 4, -1},
         {0, -1, 4}};
    b = {15, 10, 10};
    // Call sequential Gauss-Seidel
    if (rank == 0) {
        xSequential = seidel_seq(A, b, N);
    }

    // Call MPI Gauss-Seidel
    std::vector<double> xMPI(seidel_parallel(A, b, N));

    // Compare sequential and MPI solutions
    if (rank == 0) {
        for (int i = 0; i < N; ++i) {
            EXPECT_NEAR(xSequential[i], xMPI[i], 1e-2);
        }
    }
}

TEST(GaussSeidelTest, SequentialVsMPI2) {
    // Test case 2
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int N = 4;
    const int MAX_ITER = 100;
    std::vector<std::vector<double>> A(N, std::vector<double>(N, 0.0));
    std::vector<double> b(N, 0.0);
    std::vector<double> xSequential(N, 0.0);

    A = {{4, -1, 0, 0},
         {-1, 4, -1, 0},
         {0, -1, 4, -1},
         {0, 0, -1, 4}};
    b = {15, 10, 10, 5};

    // Call sequential Gauss-Seidel
    if (rank == 0) {
        xSequential = seidel_seq(A, b, N);
    }

    // Call MPI Gauss-Seidel
    std::vector<double> xMPI(seidel_parallel(A, b, N));

    // Compare sequential and MPI solutions
    if (rank == 0) {
        for (int i = 0; i < N; ++i) {
            EXPECT_NEAR(xSequential[i], xMPI[i], 1e-2);
        }
    }
}

TEST(GaussSeidelTest, SequentialVsMPI3) {
    // Test case 3
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int N = 5;
    const int MAX_ITER = 100;
    std::vector<std::vector<double>> A(N, std::vector<double>(N, 0.0));
    std::vector<double> b(N, 0.0);
    std::vector<double> xSequential(N, 0.0);

    A = {{4, -1, 0, 0, 0},
         {-1, 4, -1, 0, 0},
         {0, -1, 4, -1, 0},
         {0, 0, -1, 4, -1},
         {0, 0, 0, -1, 4}};
    b = {15, 10, 10, 5, 1};

    if (rank == 0) {
        xSequential = seidel_seq(A, b, N);
    }

    // Call MPI Gauss-Seidel
    std::vector<double> xMPI(seidel_parallel(A, b, N));

    // Compare sequential and MPI solutions
    if (rank == 0) {
        for (int i = 0; i < N; ++i) {
            EXPECT_NEAR(xSequential[i], xMPI[i], 1e-2);
        }
    }
}
TEST(GaussSeidelTest, SequentialVsMPI4) {
    // Test case 5
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int N = 6;
    const int MAX_ITER = 100;
    std::vector<std::vector<double>> A(N, std::vector<double>(N, 0.0));
    std::vector<double> b(N, 0.0);
    std::vector<double> xSequential(N, 0.0);

    A = {{4, -1, 0, 0, 0, 0},
         {-1, 4, -1, 0, 0, 0},
         {0, -1, 4, -1, 0, 0},
         {0, 0, -1, 4, -1, 0},
         {0, 0, 0, -1, 4, -1},
         {0, 0, 0, 0, -1, 4}};
    b = {15, 10, 10, 5, 1, 2};

    // Call sequential Gauss-Seidel
    if (rank == 0) {
        xSequential = seidel_seq(A, b, N);
    }

    // Call MPI Gauss-Seidel
    std::vector<double> xMPI(seidel_parallel(A, b, N));

    // Compare sequential and MPI solutions
    if (rank == 0) {
        for (int i = 0; i < N; ++i) {
            EXPECT_NEAR(xSequential[i], xMPI[i], 1e-2);
        }
    }
}
TEST(GaussSeidelTest, SequentialVsMPI5) {
    // Test case 4
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int N = 2;
    const int MAX_ITER = 100;
    std::vector<std::vector<double>> A(N, std::vector<double>(N, 0.0));
    std::vector<double> b(N, 0.0);
    std::vector<double> xSequential(N, 0.0);

    A = {{4, -1},
         {-1, 4}};
    b = {15, 10};


    // Call sequential Gauss-Seidel
    if (rank == 0) {
        xSequential = seidel_seq(A, b, N);
    }

    // Call MPI Gauss-Seidel
    std::vector<double> xMPI(seidel_parallel(A, b, N));

    // Compare sequential and MPI solutions
    if (rank == 0) {
        for (int i = 0; i < N; ++i) {
            EXPECT_NEAR(xSequential[i], xMPI[i], 1e-2);
        }
    }
}

int main(int argc, char** argv) {
    boost::mpi::environment env(argc, argv);
    boost::mpi::communicator world;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();
    if (world.rank() != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }
    return RUN_ALL_TESTS();
}
