// Copyright 2023 Soloninko Andrey
#include <gtest/gtest.h>
#include <mpi.h>

#include <iostream>

#include "./Algo.h"

TEST(Non_Matching_Symbols, Test_1_Def) {
    int rankProc = 0;
    int numProc = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    std::string s1 = "hello";
    std::string s2 = "halwo";

    int result = GetStringDifPar(s1, s2);

    if (rankProc == 0) {
        int result_non_par_t1 = GetStringDifDef(s1, s2);
        ASSERT_EQ(result, result_non_par_t1);
    }
}

TEST(Non_Matching_Symbols, Test_2_World) {
    int rankProc = 0;
    int numProc = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    std::string s1 = "World";
    std::string s2 = "dlroW";

    int result = GetStringDifPar(s1, s2);

    if (rankProc == 0) {
        int result_non_par_t2 = GetStringDifDef(s1, s2);
        ASSERT_EQ(result, result_non_par_t2);
    }
}

TEST(Non_Matching_Symbols, Test_3_Same) {
    int rankProc = 0;
    int numProc = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    std::string s1 = "World";
    std::string s2 = "World";

    int result = GetStringDifPar(s1, s2);

    if (rankProc == 0) {
        int result_non_par_t3 = GetStringDifDef(s1, s2);
        ASSERT_EQ(result, result_non_par_t3);
    }
}

TEST(Non_Matching_Symbols, Test_4) {
    int rankProc = 0;
    int numProc = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    std::string s1 = "ripoejgpojb gjdoirepoigjpe";
    std::string s2 = "qwertyuiopasdfghjklzxcvbnm";

    int result = GetStringDifPar(s1, s2);

    if (rankProc == 0) {
        int result_non_par_t4 = GetStringDifDef(s1, s2);
        ASSERT_EQ(result, result_non_par_t4);
    }
}

TEST(Non_Matching_Symbols, Test_5_Random_String) {
    int rankProc = 0;
    int numProc = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    std::string s1 = random_string(10);
    std::string s2 = random_string(10);

    int result = GetStringDifPar(s1, s2);

    if (rankProc == 0) {
        int result_non_par_t5 = GetStringDifDef(s1, s2);
        ASSERT_EQ(result, result_non_par_t5);
    }
}

int main(int argc, char **argv) {
    int res_c = 0;

    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners &listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    if (MPI_Init(&argc, &argv) != MPI_SUCCESS) MPI_Abort(MPI_COMM_WORLD, -1);
    res_c = RUN_ALL_TESTS();
    MPI_Finalize();

    return res_c;
}
