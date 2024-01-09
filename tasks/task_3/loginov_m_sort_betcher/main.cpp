// Copyright 2023 by Loginov Maxim
#include <gtest/gtest.h>
#include "task_3/loginov_m_sort_betcher/sort_betcher.h"

TEST(Parallel_Operations_MPI, size_100) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int size = 100;
    std::vector<int> par(size);
    std::vector<int> sq(size);

    par = vec_gen(size);
    sq = par;

    q_sort_batcher_par(&par);
    if (rank == 0) {
        q_sort(&sq, 0, sq.size() - 1);
        ASSERT_EQ(par, sq);
    }
}

TEST(Parallel_Operations_MPI, size_1337) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int size = 1337;
    std::vector<int> a(size);
    std::vector<int> t(size);

    a = vec_gen(size);
    t = a;

    q_sort_batcher_par(&a);
    if (rank == 0) {
        q_sort(&t, 0, t.size() - 1);
        ASSERT_EQ(a, t);
    }
}

TEST(Parallel_Operations_MPI, size_2077) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int size = 2077;
    std::vector<int> a(size);
    std::vector<int> t(size);

    a = vec_gen(size);
    t = a;

    q_sort_batcher_par(&a);
    if (rank == 0) {
        q_sort(&t, 0, t.size() - 1);
        ASSERT_EQ(a, t);
    }
}

TEST(Parallel_Operations_MPI, size_2022) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int size = 2022;
    std::vector<int> a(size);
    std::vector<int> t(size);

    a = vec_gen(size);
    t = a;

    q_sort_batcher_par(&a);
    if (rank == 0) {
        q_sort(&t, 0, t.size() - 1);
        ASSERT_EQ(a, t);
    }
}

TEST(Parallel_Operations_MPI, size_20000) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int size = 20000;
    std::vector<int> a(size);
    std::vector<int> t(size);

    a = vec_gen(size);
    t = a;

    q_sort_batcher_par(&a);
    if (rank == 0) {
        q_sort(&t, 0, t.size() - 1);
        ASSERT_EQ(a, t);
    }
}
int main(int argc, char** argv) {
    int result_code = 0;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, -1);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }
    result_code = RUN_ALL_TESTS();
    MPI_Finalize();

    return result_code;
}
