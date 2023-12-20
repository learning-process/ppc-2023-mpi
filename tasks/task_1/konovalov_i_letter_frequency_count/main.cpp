// Copyright 2023 Konovalov Igor
#include <gtest/gtest.h>
#include <mpi.h>
#include <iostream>
#include "letter_frequency.h"

TEST(Parallel_Letter_Frequency_Calculation, simple_test) {
    int rankProc = 0;
    int numProc = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    const char* str = "qwertyy";

    char letter = 'y';
    double resPar = letterFreqCalcPar(str, letter);

    if (rankProc == 0) {
        double resSeq = letterFreqCalcSeq(str, letter);
        ASSERT_EQ(resPar, resSeq);
    }
}


TEST(Parallel_Letter_Frequency_Calculation, empty_string_test) {
    int rankProc = 0;
    int numProc = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    const char* str = "";

    char letter = 'y';
    double resPar = letterFreqCalcPar(str, letter);

    if (rankProc == 0) {
        double resSeq = letterFreqCalcSeq(str, letter);
        ASSERT_EQ(resPar, resSeq);
    }
}

TEST(Parallel_Letter_Frequency_Calculation, frequency_eq_1_test) {
    int rankProc = 0;
    int numProc = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    std::string str("yyyyyyy");

    char letter = 'y';
    double resPar = letterFreqCalcPar(str, letter);

    if (rankProc == 0) {
        double resSeq = letterFreqCalcSeq(str, letter);
        ASSERT_EQ(resPar, resSeq);
    }
}

int main(int argc, char** argv) {
    int result_code = 0;

    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, -1);
    result_code = RUN_ALL_TESTS();
    MPI_Finalize();

    return result_code;
}