// Copyright 2023 Fedotov Kirill
#include <gtest/gtest.h>
#include <mpi.h>
#include <vector>
#include <string>
#include "../tasks/task_1/fedotov_k_word_count/word_count.h"

TEST(Words_Count_MPI, Just_Count) {
    std::string str = "She has seen this scene before. It had come to her in dreams many times before.";
    int res = SimpleCount(str);
    ASSERT_EQ(res, 16);
}

TEST(Words_Count_MPI, Empty_String_Words_Count_no_throw) {
    std::string str = "";
    ASSERT_NO_THROW(SimpleCount(str));
}

TEST(Words_Count_MPI, Parallel_Count) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::string testStr = "In fact, it was difficult for anyone to come up with a date they had first appeared.";
    int res = ParallelCount(testStr);
    if (rank == 0) {
        ASSERT_EQ(res, 17);
    }
}
TEST(Words_Count_MPI, Parallel_Count_Complicated_String) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::string testStr = "Dragons don't exist they said.";
    int res = ParallelCount(testStr);
    if (rank == 0) {
        ASSERT_EQ(res, 5);
    }
}
TEST(Words_Count_MPI, Parallel_Count_Generated_String) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::string testStr = getLongString(50);
    int res = ParallelCount(testStr);
    if (rank == 0) {
        ASSERT_EQ(res, 50);
    }
}

TEST(Words_Count_MPI, Parallel_Count_Generated_Long_String) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::string testStr = getLongString(2500);
    int res = ParallelCount(testStr);
    if (rank == 0) {
        ASSERT_EQ(res, 2500);
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();

    if (rank != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }

    int result = RUN_ALL_TESTS();
    MPI_Finalize();
    return result;
}