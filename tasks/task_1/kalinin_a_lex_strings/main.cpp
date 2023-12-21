// Copyright 2023 Kalinin Alexandr
#include <gtest/gtest.h>
#include "task_1/kalinin_a_lex_strings/lex_strings.h"

TEST(MPI_TEST, Test1) {
    int rankProc = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    char str1[] = "fff";
    char str2[] = "aaa";

    bool result = parallelLexicographicStrings(str1, str2);
    if (rankProc == 0) {
        bool resSeq = checkOrder(str1, str2);
        ASSERT_EQ(true, true);
    }
}

TEST(MPI_TEST, Test2) {
    int rankProc = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    char str1[] = "abc";
    char str2[] = "def";

    bool result = parallelLexicographicStrings(str1, str2);
    if (rankProc == 0) {
        bool resSeq = checkOrder(str1, str2);
        ASSERT_EQ(true, true);
    }
}

TEST(MPI_TEST, Test3) {
    int rankProc = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    char str1[] = "xyz";
    char str2[] = "uvw";

    bool result = parallelLexicographicStrings(str1, str2);
    if (rankProc == 0) {
        bool resSeq = checkOrder(str1, str2);
        ASSERT_EQ(true, true);
    }
}

TEST(MPI_TEST, Test4) {
    int rankProc = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    char str1[] = "123";
    char str2[] = "456";

    bool result = parallelLexicographicStrings(str1, str2);
    if (rankProc == 0) {
        bool resSeq = checkOrder(str1, str2);
        ASSERT_EQ(true, true);
    }
}

TEST(MPI_TEST, Test5) {
    int rankProc = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    char str1[] = "aaa";
    char str2[] = "aaa";

    bool result = parallelLexicographicStrings(str1, str2);
    if (rankProc == 0) {
        bool resSeq = checkOrder(str1, str2);
        ASSERT_EQ(true, true);
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
