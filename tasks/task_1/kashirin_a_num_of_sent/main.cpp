// Copyright 2023 Kashirin Alexander
#include <gtest/gtest.h>
#include <mpi.h>
#include <iostream>
#include "./num_of_sent.h"

TEST(MPI_TEST, Test_no_sent) {
    int rankProc = 0;
    int numProc = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    std::string str = "sentence";
    int result = numPar(str);
    if (rankProc == 0) {
        std::vector<char> vec(str.begin(), str.end());
        int resSeq = numSeq(vec);
        ASSERT_EQ(result, resSeq);
    }
}

TEST(MPI_TEST, Test_one_sent) {
    int rankProc = 0;
    int numProc = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);
    std::string str = "sentence1.";

    int result = numPar(str);

    if (rankProc == 0) {
        std::vector<char> vec(str.begin(), str.end());
        int resSeq = numSeq(vec);
        ASSERT_EQ(result, resSeq);
    }
}

TEST(MPI_TEST, Test_empty_string) {
    int rankProc = 0;
    int numProc = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    std::string str = "";
    int result = numPar(str);

    if (rankProc == 0) {
        std::vector<char> vec(str.begin(), str.end());
        int resSeq = numSeq(vec);
        ASSERT_EQ(result, resSeq);
    }
}

TEST(MPI_TEST, Test_random_string) {
    int rankProc = 0;
    int numProc = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    std::string str = "";
    generateRandomString(str, 20);

    int result = numPar(str);

    if (rankProc == 0) {
        std::vector<char> vec(str.begin(), str.end());
        int resSeq = numSeq(vec);
        ASSERT_EQ(result, resSeq);
    }
}

TEST(MPI_TEST, Test_all_dots) {
    int rankProc = 0;
    int numProc = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    std::string str = ". ... .";

    int result = numPar(str);

    if (rankProc == 0) {
        std::vector<char> vec(str.begin(), str.end());
        int resSeq = numSeq(vec);
        ASSERT_EQ(result, resSeq);
    }
}

TEST(MPI_TEST, Test_three_sent) {
    int rankProc = 0;
    int numProc = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    std::string str = "sentence1. sentence2? sentence3!";

    int result = numPar(str);

    if (rankProc == 0) {
        std::vector<char> vec(str.begin(), str.end());
        int resSeq = numSeq(vec);
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
    result_code = RUN_ALL_TESTS();
    MPI_Finalize();

    return result_code;
}
