// Copyright 2023 Ivanchenko Aleksei
#include <gtest/gtest.h>
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>
#include <boost/mpi/timer.hpp>
#include "task_3/ivanchenko_a_global_search_strongin/global_search_strongin.h"

double f(double x) {
    return 1.563e-6 * x*x*x*x - 0.25*x*x;
}

TEST(GLOBAL_SEARCH_STRONGIN, sequential) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double seqRes, paralRes, start, end, timeSeq, timeParallel;
    if (rank == 0) {
        seqRes = searchSequential(f, 0.0, 3.0, 0.01);
        EXPECT_NEAR(seqRes, 0, 1e-2);
    }
}
TEST(GLOBAL_SEARCH_STRONGIN, parallel_1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double seqRes, paralRes, start, end, timeSeq, timeParallel;
    if (rank == 0) {
        start = MPI_Wtime();
        seqRes = searchSequential(f, -1.0, 1.0, 0.01);
        end = MPI_Wtime();
        timeSeq = end - start;
        std::cout << "Sequential algorithm time: " << timeSeq << std::endl;
        start = MPI_Wtime();
    }
    paralRes = searchParallel(f, -1.0, 1.0, 0.01);
    if (rank == 0) {
        end = MPI_Wtime();
        timeParallel = end - start;
        std::cout << "Parallel algorithm time: " << timeParallel << std::endl;
        std::cout << "Effective " << timeSeq / timeParallel << std::endl;
    }
}
TEST(GLOBAL_SEARCH_STRONGIN, parallel_5) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double seqRes, paralRes, start, end, timeSeq, timeParallel;
    if (rank == 0) {
        start = MPI_Wtime();
        seqRes = searchSequential(f, -5.0, 5.0, 0.01);
        end = MPI_Wtime();
        timeSeq = end - start;
        std::cout << "Sequential algorithm time: " << timeSeq << std::endl;
        start = MPI_Wtime();
    }
    paralRes = searchParallel(f, -5.0, 5.0, 0.01);
    if (rank == 0) {
        end = MPI_Wtime();
        timeParallel = end - start;
        std::cout << "Parallel algorithm time: " << timeParallel << std::endl;
        std::cout << "Effective " << timeSeq / timeParallel << std::endl;
    }
}
TEST(GLOBAL_SEARCH_STRONGIN, parallel_10) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double seqRes, paralRes, start, end, timeSeq, timeParallel;
    if (rank == 0) {
        start = MPI_Wtime();
        seqRes = searchSequential(f, -10.0, 10.0, 0.01);
        end = MPI_Wtime();
        timeSeq = end - start;
        std::cout << "Sequential algorithm time: " << timeSeq << std::endl;
        start = MPI_Wtime();
    }
    paralRes = searchParallel(f, -10.0, 10.0, 0.01);
    if (rank == 0) {
        end = MPI_Wtime();
        timeParallel = end - start;
        std::cout << "Parallel algorithm time: " << timeParallel << std::endl;
        std::cout << "Effective " << timeSeq / timeParallel << std::endl;
    }
}
TEST(GLOBAL_SEARCH_STRONGIN, parallel_50) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double seqRes, paralRes, start, end, timeSeq, timeParallel;
    if (rank == 0) {
        start = MPI_Wtime();
        seqRes = searchSequential(f, -50.0, 50.0, 0.001);
        end = MPI_Wtime();
        timeSeq = end - start;
        std::cout << "Sequential algorithm time: " << timeSeq << std::endl;
        start = MPI_Wtime();
    }
    paralRes = searchParallel(f, -50.0, 50.0, 0.001);
    if (rank == 0) {
        end = MPI_Wtime();
        timeParallel = end - start;
        std::cout << "Parallel algorithm time: " << timeParallel << std::endl;
        std::cout << "Effective " << timeSeq / timeParallel << std::endl;
    }
}
TEST(GLOBAL_SEARCH_STRONGIN, parallel_100) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double seqRes, paralRes, start, end, timeSeq, timeParallel;
    if (rank == 0) {
        start = MPI_Wtime();
        seqRes = searchSequential(f, -100.0, 100.0, 0.01);
        end = MPI_Wtime();
        timeSeq = end - start;
        std::cout << "Sequential algorithm time: " << timeSeq << std::endl;
        start = MPI_Wtime();
    }
    paralRes = searchParallel(f, -100.0, 100.0, 0.01);
    if (rank == 0) {
        end = MPI_Wtime();
        timeParallel = end - start;
        std::cout << "Parallel algorithm time: " << timeParallel << std::endl;
        std::cout << "Effective " << timeSeq / timeParallel << std::endl;
    }
}
TEST(GLOBAL_SEARCH_STRONGIN, parallel_201) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double seqRes, paralRes, start, end, timeSeq, timeParallel;
    if (rank == 0) {
        start = MPI_Wtime();
        seqRes = searchSequential(f, -201.0, 201.0, 0.01);
        end = MPI_Wtime();
        timeSeq = end - start;
        std::cout << "Sequential algorithm time: " << timeSeq << std::endl;
        start = MPI_Wtime();
    }
    paralRes = searchParallel(f, -201.0, 201.0, 0.01);
    if (rank == 0) {
        end = MPI_Wtime();
        timeParallel = end - start;
        std::cout << "Parallel algorithm time: " << timeParallel << std::endl;
        std::cout << "Effective " << timeSeq / timeParallel << std::endl;
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners &listeners =
        ::testing::UnitTest::GetInstance()->listeners();
    if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, -1);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }
    int exec_code = RUN_ALL_TESTS();
    MPI_Finalize();
    return exec_code;
}
