// Copyright 2023 Kalinin Alexandr
#include <gtest/gtest.h>
#include "task_1/kalinin_a_lex_strings/lex_strings.h"

TEST(MPI_TEST, Test1) {
    int rankProc = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    std::string str1 = "abc";
    std::string str2 = "abc";

    int result = parallelLexicographicStrings(str1, str2);
    if (rankProc == 0) {
        int resSeq = checkOrder(str1, str2);
        ASSERT_EQ(result, resSeq);
    }
}

TEST(MPI_TEST, Test2) {
    int rankProc = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    std::string str1 = "bbb";
    std::string str2 = "aaa";

    int result = parallelLexicographicStrings(str1, str2);
    if (rankProc == 0) {
        int resSeq = checkOrder(str1, str2);
        ASSERT_EQ(result, resSeq);
    }
}

TEST(MPI_TEST, Test3) {
    int rankProc = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    std::string str1 = " ";
    std::string str2 = " ";

    int result = parallelLexicographicStrings(str1, str2);
    if (rankProc == 0) {
        int resSeq = checkOrder(str1, str2);
        ASSERT_EQ(result, resSeq);
    }
}

TEST(MPI_TEST, Test4) {
    int rankProc = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    std::string str1 = "banana";
    std::string str2 = "apple";

    int result = parallelLexicographicStrings(str1, str2);
    if (rankProc == 0) {
        int resSeq = checkOrder(str1, str2);
        ASSERT_EQ(result, resSeq);
    }
}

TEST(MPI_TEST, Test5) {
    int rankProc = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    std::string str1 = "bb";
    std::string str2 = "aa";

    int result = parallelLexicographicStrings(str1, str2);
    if (rankProc == 0) {
        int resSeq = checkOrder(str1, str2);
        ASSERT_EQ(result, resSeq);
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
