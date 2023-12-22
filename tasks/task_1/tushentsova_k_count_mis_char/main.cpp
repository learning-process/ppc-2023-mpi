// Copyright 2023 Tushentsova Karina
#include <gtest/gtest.h>
#include <mpi.h>
#include <iostream>
#include "task_1/tushentsova_k_count_mis_char/count_mis_char.h"

TEST(StringCountTest, Test_1) {
    int rank = 0, size = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::string str1 = "Don't stop!";
    std::string str2 = "Don't stop!";

    int parallelCount = countNonMatchingChars(str1, str2);

    if (rank == 0) {
        int sequentiaCount = parCountNonMatchingChars(str1, str2);
        ASSERT_EQ(parallelCount, sequentiaCount);
    }    
}

TEST(StringCountTest, Test_2) {
    int rank = 0, size = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::string str1 = "Don't stop!";
    std::string str2 = "Don't stop";

    int parallelCount = countNonMatchingChars(str1, str2);

    if (rank == 0) {
        int sequentiaCount = parCountNonMatchingChars(str1, str2);
        ASSERT_EQ(parallelCount, sequentiaCount);
    }
}

TEST(StringCountTest, Test_3) {
    int rank = 0, size = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::string str1 = "Don't Stop!";
    std::string str2 = "Don't stop!";

    int parallelCount = countNonMatchingChars(str1, str2);

    if (rank == 0) {
        int sequentiaCount = parCountNonMatchingChars(str1, str2);
        ASSERT_EQ(parallelCount, sequentiaCount);
    }
}

TEST(StringCountTest, Test_4) {
    int rank = 0, size = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::string str1 = "Don't Stop!";
    std::string str2 = "Don't shop!";

    int parallelCount = countNonMatchingChars(str1, str2);

    if (rank == 0) {
        int sequentiaCount = parCountNonMatchingChars(str1, str2);
        ASSERT_EQ(parallelCount, sequentiaCount);
    }
}

TEST(StringCountTest, Test_5) {
    int rank = 0, size = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::string str1 = "Don't Stop!";
    std::string str2 = "";

    int parallelCount = countNonMatchingChars(str1, str2);

    if (rank == 0) {
        int sequentiaCount = parCountNonMatchingChars(str1, str2);
        ASSERT_EQ(parallelCount, sequentiaCount);
    }
}

int main(int argc, char **argv) {
    int resultCode = 0;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, -1);
    resultCode = RUN_ALL_TESTS();
    MPI_Finalize();

    return resultCode;
}
