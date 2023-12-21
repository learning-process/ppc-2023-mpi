// Copyright 2023 Ivanchenko Aleksei
#include <gtest/gtest.h>
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>
#include <boost/mpi/timer.hpp>
#include "task_3/ivanchenko_a_global_search_strongin/global_search_strongin.h"

TEST(Parallel_Operations_MPI, parallel_1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double start, end, timeSeq, timeParallel;
    if (rank == 0) {
        start = MPI_Wtime();
        searchSequential(-1.0, 1.0, 0.001);
        end = MPI_Wtime();
        timeSeq = end - start;
        std::cout << "Sequential algorithm time: " << timeSeq << std::endl;
        start = MPI_Wtime();
    }
    searchParallel(-10.0, 10.0, 0.001);
    if (rank == 0) {
        end = MPI_Wtime();
        timeParallel = end - start;
        std::cout << "Parallel algorithm time: " << timeParallel << std::endl;
        std::cout << "Effective " << timeSeq / timeParallel << std::endl;
    }
}
TEST(Parallel_Operations_MPI, parallel_10) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double start, end, timeSeq, timeParallel;
    if (rank == 0) {
        start = MPI_Wtime();
        searchSequential(-10.0, 10.0, 0.001);
        end = MPI_Wtime();
        timeSeq = end - start;
        std::cout << "Sequential algorithm time: " << timeSeq << std::endl;
        start = MPI_Wtime();
    }
    searchParallel(-10.0, 10.0, 0.001);
    if (rank == 0) {
        end = MPI_Wtime();
        timeParallel = end - start;
        std::cout << "Parallel algorithm time: " << timeParallel << std::endl;
        std::cout << "Effective " << timeSeq / timeParallel << std::endl;
    }
}
TEST(Parallel_Operations_MPI, parallel_50) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double start, end, timeSeq, timeParallel;
    if (rank == 0) {
        start = MPI_Wtime();
        searchSequential(-50.0, 50.0, 0.001);
        end = MPI_Wtime();
        timeSeq = end - start;
        std::cout << "Sequential algorithm time: " << timeSeq << std::endl;
        start = MPI_Wtime();
    }
    searchParallel(-50.0, 50.0, 0.001);
    if (rank == 0) {
        end = MPI_Wtime();
        timeParallel = end - start;
        std::cout << "Parallel algorithm time: " << timeParallel << std::endl;
        std::cout << "Effective " << timeSeq / timeParallel << std::endl;
    }
}
TEST(Parallel_Operations_MPI, parallel_100) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double start, end, timeSeq, timeParallel;
    if (rank == 0) {
        start = MPI_Wtime();
        searchSequential(-100.0, 100.0, 0.001);
        end = MPI_Wtime();
        timeSeq = end - start;
        std::cout << "Sequential algorithm time: " << timeSeq << std::endl;
        start = MPI_Wtime();
    }
    searchParallel(-100.0, 100.0, 0.001);
    if (rank == 0) {
        end = MPI_Wtime();
        timeParallel = end - start;
        std::cout << "Parallel algorithm time: " << timeParallel << std::endl;
        std::cout << "Effective " << timeSeq / timeParallel << std::endl;
    }
}
TEST(Parallel_Operations_MPI, parallel_sum_101) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double start, end, timeSeq, timeParallel;
    if (rank == 0) {
        start = MPI_Wtime();
        searchSequential(-101.0, 101.0, 0.001);
        end = MPI_Wtime();
        timeSeq = end - start;
        std::cout << "Sequential algorithm time: " << timeSeq << std::endl;
        start = MPI_Wtime();
    }
    searchParallel(-101.0, 101.0, 0.001);
    if (rank == 0) {
        end = MPI_Wtime();
        timeParallel = end - start;
        std::cout << "Parallel algorithm time: " << timeParallel << std::endl;
        std::cout << "Effective " << timeSeq / timeParallel << std::endl;
    }
}
TEST(Parallel_Operations_MPI, parallel_501) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double start, end, timeSeq, timeParallel;
    if (rank == 0) {
        start = MPI_Wtime();
        searchSequential(-501.0, 501.0, 0.001);
        end = MPI_Wtime();
        timeSeq = end - start;
        std::cout << "Sequential algorithm time: " << timeSeq << std::endl;
        start = MPI_Wtime();
    }
    searchParallel(-501.0, 501.0, 0.001);
    if (rank == 0) {
        end = MPI_Wtime();
        timeParallel = end - start;
        std::cout << "Parallel algorithm time: " << timeParallel << std::endl;
        std::cout << "Effective " << timeSeq / timeParallel << std::endl;
    }
}
TEST(Parallel_Operations_MPI, parallel_1001) {
     int rank;
     MPI_Comm_rank(MPI_COMM_WORLD, &rank);
     double start, end, timeSeq, timeParallel;
     if (rank == 0) {
       start = MPI_Wtime();
       searchSequential(-1001.0, 1001.0, 0.001);
       end = MPI_Wtime();
       timeSeq = end - start;
       std::cout << "Sequential algorithm time: " << timeSeq << std::endl;
       start = MPI_Wtime();
     }
     searchParallel(-1001.0, 1001.0, 0.001);
     if (rank == 0) {
       end = MPI_Wtime();
       timeParallel = end - start;
       std::cout << "Parallel algorithm time: " << timeParallel << std::endl;
       std::cout << "Effective " << timeSeq / timeParallel << std::endl;
     }
}
TEST(Parallel_Operations_MPI, parallel_2001) {
     int rank;
     MPI_Comm_rank(MPI_COMM_WORLD, &rank);
     double start, end, timeSeq, timeParallel;
     if (rank == 0) {
       start = MPI_Wtime();
       searchSequential(-2001.0, 2001.0, 0.001);
       end = MPI_Wtime();
       timeSeq = end - start;
       std::cout << "Sequential algorithm time: " << timeSeq << std::endl;
       start = MPI_Wtime();
     }
     searchParallel(-2001.0, 2001.0, 0.001);
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