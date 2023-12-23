// Copyright 2023 Podyachikh Mikhail
#include <gtest/gtest.h>
#include <mpi.h>
#include <utility>
#include "./reduce.h"

TEST(Reduce, Test_one) {
    int rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int result, result_mpi;
    reduce(&rank, &result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&rank, &result_mpi, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        ASSERT_EQ(result, result_mpi);
        ASSERT_EQ(result, world_size * (world_size - 1) / 2);
    }
}

TEST(Reduce, Test_one_root) {
    int rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    std::default_random_engine re;
    std::uniform_int_distribution<int> rnd(0, world_size - 1);
    int root = rnd(re);
    int result, result_mpi;
    reduce(&rank, &result, 1, MPI_INT, MPI_SUM, root, MPI_COMM_WORLD);
    MPI_Reduce(&rank, &result_mpi, 1, MPI_INT, MPI_SUM, root, MPI_COMM_WORLD);
    if (rank == root) {
        ASSERT_EQ(result, result_mpi);
        ASSERT_EQ(result, world_size * (world_size - 1) / 2);
    }
}

TEST(Reduce, Test_Loc_Op) {
    int rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    std::default_random_engine re;
    std::uniform_int_distribution<int> rnd(-1e8, 1e8);
    std::pair<int, int> a ={rnd(re), rnd(re)};
    std::pair<int, int> result, result_mpi;

    reduce(&a, &result, 1, MPI_2INT, MPI_MAXLOC, 0, MPI_COMM_WORLD);
    MPI_Reduce(&a, &result_mpi, 1, MPI_2INT, MPI_MAXLOC, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        ASSERT_EQ(result, result_mpi);
    }
}

TEST(Reduce, Test_Bit_Op) {
    int rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    std::default_random_engine re;
    std::uniform_int_distribution<int> rnd(0, 1e8);
    int a = rnd(re);
    int result, result_mpi;

    reduce(&a, &result, 1, MPI_INT, MPI_BAND, 0, MPI_COMM_WORLD);
    MPI_Reduce(&a, &result_mpi, 1, MPI_INT, MPI_BAND, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        ASSERT_EQ(result, result_mpi);
    }
}

TEST(Reduce, Test_Random_Sum) {
    int rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int n = 1e5;
    std::vector<double> a(n), result(n), result_mpi(n);
    FillRandom<double>(a.data(), a.size(), 0, 1);

    MPI_Barrier(MPI_COMM_WORLD);
    double time, time_mpi;
    if (rank == 0)
        time = MPI_Wtime();
    reduce(a.data(), result.data(), n, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0)
        time = MPI_Wtime() - time;

    if (rank == 0)
        time_mpi = MPI_Wtime();
    MPI_Reduce(a.data(), result_mpi.data(), n, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0)
        time_mpi = MPI_Wtime() - time_mpi;

    if (rank == 0) {
        std::cout << "Test_Random_Sum:\n\tReduce time: " << time << ", MPI_Reduce time: " << time_mpi << "\n";
        for (int i = 0; i < n; i++) {
            ASSERT_LT(abs(result[i] - result_mpi[i]), 1e-8);
        }
    }
}

TEST(Reduce, Test_Random_Prod) {
    int rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int n = 1e5;
    std::vector<int64_t> a(n), result(n), result_mpi(n);
    FillRandom<int64_t>(a.data(), a.size(), 0, 100);

    MPI_Barrier(MPI_COMM_WORLD);
    double time, time_mpi;
    if (rank == 0)
        time = MPI_Wtime();
    reduce(a.data(), result.data(), n, MPI_LONG_LONG, MPI_PROD, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0)
        time = MPI_Wtime() - time;

    if (rank == 0)
        time_mpi = MPI_Wtime();
    MPI_Reduce(a.data(), result_mpi.data(), n, MPI_LONG_LONG, MPI_PROD, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0)
        time_mpi = MPI_Wtime() - time_mpi;

    if (rank == 0) {
        std::cout << "Test_Random_Prod:\n\tReduce time: " << time << ", MPI_Reduce time: " << time_mpi << "\n";
        for (int i = 0; i < n; i++) {
            ASSERT_EQ(result[i], result_mpi[i]);
        }
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners &listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    RETURN_IF_ERR(MPI_Init(&argc, &argv));

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }

    int result_code = RUN_ALL_TESTS();

    MPI_Finalize();
    return result_code;
}
