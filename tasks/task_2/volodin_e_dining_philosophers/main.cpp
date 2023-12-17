// Copyright 2023 Volodin Evgeniy
#include <gtest/gtest.h>
#include "task_2/volodin_e_dining_philosophers/philosophers.h"


TEST(Parallel_Operations_MPI, Plus) {
    int proc_rank, proc_num;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
    std::vector<int> lunch_begin;
    std::vector<int> lunch_end;
    if (proc_rank == 0) {
        lunch_begin.resize(proc_num);
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<int> spaghetti(5, 1000);
        for (int i = 1; i < proc_num; ++i) {
            lunch_begin[i] = spaghetti(rng);
        }
        lunch_end = lunch_begin;
    }

    MPI_Barrier(MPI_COMM_WORLD);

    dinner(lunch_end.data(), "+");

    if (proc_rank == 0) {
        for (int i = 1; i < proc_num; ++i) {
            int temp = eat_alone(i, lunch_begin[i], "+");\
            ASSERT_EQ(temp, lunch_end[i]);
        }
    }
}

TEST(Parallel_Operations_MPI, Minus) {
    int proc_rank, proc_num;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
    std::vector<int> lunch_begin;
    std::vector<int> lunch_end;
    if (proc_rank == 0) {
        lunch_begin.resize(proc_num);
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<int> spaghetti(5, 1000);
        for (int i = 1; i < proc_num; ++i) {
            lunch_begin[i] = spaghetti(rng);
        }
        lunch_end = lunch_begin;
    }

    MPI_Barrier(MPI_COMM_WORLD);

    dinner(lunch_end.data(), "-");

    if (proc_rank == 0) {
        for (int i = 1; i < proc_num; ++i) {
            int temp = eat_alone(i, lunch_begin[i], "-");\
            ASSERT_EQ(temp, lunch_end[i]);
        }
    }
}

TEST(Parallel_Operations_MPI, Multiply) {
    int proc_rank, proc_num;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
    std::vector<int> lunch_begin;
    std::vector<int> lunch_end;
    if (proc_rank == 0) {
        lunch_begin.resize(proc_num);
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<int> spaghetti(5, 1000);
        for (int i = 1; i < proc_num; ++i) {
            lunch_begin[i] = spaghetti(rng);
        }
        lunch_end = lunch_begin;
    }

    MPI_Barrier(MPI_COMM_WORLD);

    dinner(lunch_end.data(), "*");

    if (proc_rank == 0) {
        for (int i = 1; i < proc_num; ++i) {
            int temp = eat_alone(i, lunch_begin[i], "*");\
            ASSERT_EQ(temp, lunch_end[i]);
        }
    }
}

TEST(Parallel_Operations_MPI, Power) {
    int proc_rank, proc_num;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
    std::vector<int> lunch_begin;
    std::vector<int> lunch_end;
    if (proc_rank == 0) {
        lunch_begin.resize(proc_num);
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<int> spaghetti(5, 1000);
        for (int i = 1; i < proc_num; ++i) {
            lunch_begin[i] = spaghetti(rng);
        }
        lunch_end = lunch_begin;
    }

    MPI_Barrier(MPI_COMM_WORLD);

    dinner(lunch_end.data(), "**");

    if (proc_rank == 0) {
        for (int i = 1; i < proc_num; ++i) {
            int temp = eat_alone(i, lunch_begin[i], "**");\
            ASSERT_EQ(temp, lunch_end[i]);
        }
    }
}

TEST(Parallel_Operations_MPI, Division) {
    int proc_rank, proc_num;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
    std::vector<int> lunch_begin;
    std::vector<int> lunch_end;
    if (proc_rank == 0) {
        lunch_begin.resize(proc_num);
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<int> spaghetti(5, 1000);
        for (int i = 1; i < proc_num; ++i) {
            lunch_begin[i] = spaghetti(rng);
        }
        lunch_end = lunch_begin;
    }

    MPI_Barrier(MPI_COMM_WORLD);

    dinner(lunch_end.data(), "/");

    if (proc_rank == 0) {
        for (int i = 1; i < proc_num; ++i) {
            int temp = eat_alone(i, lunch_begin[i], "/");\
            ASSERT_EQ(temp, lunch_end[i]);
        }
    }
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);
    int result_code = 0;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();
    int proc_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    if (proc_rank != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }
    result_code = RUN_ALL_TESTS();
    MPI_Finalize();
    return result_code;
    return 0;
}
