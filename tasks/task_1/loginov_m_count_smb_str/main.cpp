// Copyright 2023 Simonyan Suren
#include <gtest/gtest.h>
#include <math.h>
#include "task_1/loginov_m_count_smb_str/count_smb_str.h"

TEST(Parallel_Letter_Frequency_count, Test1) {
    int rankProc = 0;
    int numProc = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    std::string str("Hello world!");

    char target = 'l';
    int result = calcFreqPar(str, target);

    if (rankProc == 0) {
        int resSeq = calcFreqSeq(str, target);
        ASSERT_EQ(result, resSeq);
    }
}

TEST(Parallel_Letter_Frequency_count, Test2) {
    int rankProc = 0;
    int numProc = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    std::string str("");

    char target = 'l';
    int result = calcFreqPar(str, target);

    if (rankProc == 0) {
        int resSeq = calcFreqSeq(str, target);
        ASSERT_EQ(result, resSeq);
    }
}

TEST(Parallel_Letter_Frequency_count, Test3) {
    int rankProc = 0;
    int numProc = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    std::string str("Kono Dio da!@#$)(*&%");

    char target = 'l';
    int result = calcFreqPar(str, target);

    if (rankProc == 0) {
        int resSeq = calcFreqSeq(str, target);
        ASSERT_EQ(result, resSeq);
    }
}

TEST(Parallel_Letter_Frequency_count, Test4) {
    int rankProc = 0;
    int numProc = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    std::string str("llllllllllllllll");

    char target = 'l';
    int result = calcFreqPar(str, target);

    if (rankProc == 0) {
        int resSeq = calcFreqSeq(str, target);
        ASSERT_EQ(result, resSeq);
    }
}

TEST(Parallel_Letter_Frequency_count, Test5) {
    int rankProc = 0;
    int numProc = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    std::string str(1000, ' ');

    for (int i = 0; i < str.length(); i++) {
        str[i] = static_cast<char>(63 + (std::rand() % (90 - 63)));
    }

    char target = 'L';
    int result = calcFreqPar(str, target);

    if (rankProc == 0) {
        int resSeq = calcFreqSeq(str, target);
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
