// Copyright 2023 Ivanchenko Aleksei
#include <gtest/gtest.h>
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>
#include <boost/mpi/timer.hpp>
#include "task_3/ivanchenko_a_global_search_strongin/global_search_strongin.h"

TEST(GLOBAL_SEARCH_STRONGIN, sequential) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double seqRes, paralRes, start, end, timeSeq, timeParallel;
    if (rank == 0) {
        seqRes = searchSequential(0.0, 3.0, 0.1);
        EXPECT_NEAR(seqRes, 0, 1e-2);
    }
}
TEST(GLOBAL_SEARCH_STRONGIN, parallel_1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double seqRes, paralRes, start, end, timeSeq, timeParallel;
    if (rank == 0) {
        start = MPI_Wtime();
        seqRes = searchSequential(-1.0, 1.0, 0.1);
        end = MPI_Wtime();
        timeSeq = end - start;
        start = MPI_Wtime();
    }
    paralRes = searchParallel(-1.0, 1.0, 0.1);
    if (rank == 0) {
        end = MPI_Wtime();
        timeParallel = end - start;
    }
}
TEST(GLOBAL_SEARCH_STRONGIN, parallel_2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double seqRes, paralRes, start, end, timeSeq, timeParallel;
    if (rank == 0) {
        start = MPI_Wtime();
        seqRes = searchSequential(-2.0, 2.0, 0.1);
        end = MPI_Wtime();
        timeSeq = end - start;
        start = MPI_Wtime();
    }
    paralRes = searchParallel(-2.0, 2.0, 0.1);
    if (rank == 0) {
        end = MPI_Wtime();
        timeParallel = end - start;
    }
}
TEST(GLOBAL_SEARCH_STRONGIN, parallel_3) {

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double seqRes, paralRes, start, end, timeSeq, timeParallel;
    if (rank == 0) {
        start = MPI_Wtime();
        seqRes = searchSequential(-3.0, 3.0, 0.1);
        end = MPI_Wtime();
        timeSeq = end - start;
        start = MPI_Wtime();
    }
    paralRes = searchParallel(-3.0, 3.0, 0.1);
    if (rank == 0) {
        end = MPI_Wtime();
        timeParallel = end - start;
    }
}
TEST(GLOBAL_SEARCH_STRONGIN, parallel_4) {

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double seqRes, paralRes, start, end, timeSeq, timeParallel;
    if (rank == 0) {
        start = MPI_Wtime();
        seqRes = searchSequential(-4.0, 4.0, 0.001);
        end = MPI_Wtime();
        timeSeq = end - start;
        start = MPI_Wtime();
    }
    paralRes = searchParallel(-4.0, 4.0, 0.001);
    if (rank == 0) {
        end = MPI_Wtime();
        timeParallel = end - start;
    }
}
TEST(GLOBAL_SEARCH_STRONGIN, parallel_5) {

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double seqRes, paralRes, start, end, timeSeq, timeParallel;
    if (rank == 0) {
        start = MPI_Wtime();
        seqRes = searchSequential(-5.0, 5.0, 0.1);
        end = MPI_Wtime();
        timeSeq = end - start;
        start = MPI_Wtime();
    }
    paralRes = searchParallel(-5.0, 5.0, 0.1);
    if (rank == 0) {
        end = MPI_Wtime();
        timeParallel = end - start;
    }
}
TEST(GLOBAL_SEARCH_STRONGIN, parallel_6) {

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double seqRes, paralRes, start, end, timeSeq, timeParallel;
    if (rank == 0) {
        start = MPI_Wtime();
        seqRes = searchSequential(-6.0, 6.0, 0.1);
        end = MPI_Wtime();
        timeSeq = end - start;
        start = MPI_Wtime();
    }
    paralRes = searchParallel(-6.0, 6.0, 0.1);
    if (rank == 0) {
        end = MPI_Wtime();
        timeParallel = end - start;

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
