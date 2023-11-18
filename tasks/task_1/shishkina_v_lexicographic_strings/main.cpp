// Copyright 2023 Shishkina Valeria
#include <gtest/gtest.h>
#include <mpi.h>

#include <iostream>

#include "./lexicographic_strings.h"

TEST(MPI_TEST, Test1) {
  int rankProc = 0;
  // int numProc = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
  // MPI_Comm_size(MPI_COMM_WORLD, &numProc);
  char str1[] = "";
  char str2[] = "aaa";

  int result = parallelLexicographicStrings(str1, str2);
  if (rankProc == 0) {
    int resSeq = isLexicograpic(str1, str2);
    ASSERT_EQ(result, resSeq);
  }
}

TEST(MPI_TEST, Test2) {
  int rankProc = 0;
  // int numProc = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
  // MPI_Comm_size(MPI_COMM_WORLD, &numProc);
  char str1[] = "aaa";
  char str2[] = "";

  int result = parallelLexicographicStrings(str1, str2);
  if (rankProc == 0) {
    int resSeq = isLexicograpic(str1, str2);
    ASSERT_EQ(result, resSeq);
  }
}

TEST(MPI_TEST, Test3) {
  int rankProc = 0;
  // int numProc = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
  // MPI_Comm_size(MPI_COMM_WORLD, &numProc);
  char str1[] = "abcde";
  char str2[] = "abcdef";

  int result = parallelLexicographicStrings(str1, str2);
  if (rankProc == 0) {
    int resSeq = isLexicograpic(str1, str2);
    ASSERT_EQ(result, resSeq);
  }
}

TEST(MPI_TEST, Test4) {
  int rankProc = 0;
  // int numProc = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
  // MPI_Comm_size(MPI_COMM_WORLD, &numProc);
  char str1[] = "banana";
  char str2[] = "apple";

  int result = parallelLexicographicStrings(str1, str2);
  if (rankProc == 0) {
    int resSeq = isLexicograpic(str1, str2);
    ASSERT_EQ(result, resSeq);
  }
}

TEST(MPI_TEST, Test5) {
  int rankProc = 0;
  // int numProc = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
  // MPI_Comm_size(MPI_COMM_WORLD, &numProc);
  char str1[] = "";
  char str2[] = "";

  int result = parallelLexicographicStrings(str1, str2);
  if (rankProc == 0) {
    int resSeq = isLexicograpic(str1, str2);
    ASSERT_EQ(result, resSeq);
  }
}

TEST(MPI_TEST, Test6) {
  int rankProc = 0;
  // int numProc = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
  // MPI_Comm_size(MPI_COMM_WORLD, &numProc);
  char str1[] = "apple";
  char str2[] = "banana";

  int result = parallelLexicographicStrings(str1, str2);
  if (rankProc == 0) {
    int resSeq = isLexicograpic(str1, str2);
    ASSERT_EQ(result, resSeq);
  }
}

TEST(MPI_TEST, Test7) {
  int rankProc = 0;
  // int numProc = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
  // MPI_Comm_size(MPI_COMM_WORLD, &numProc);
  char str1[] = "aba";
  char str2[] = "bbb";

  int result = parallelLexicographicStrings(str1, str2);
  if (rankProc == 0) {
    int resSeq = isLexicograpic(str1, str2);
    ASSERT_EQ(result, resSeq);
  }
}

int main(int argc, char* argv[]) {
  MPI_Init(&argc, &argv);
  int result_code = 0;
  ::testing::InitGoogleTest(&argc, argv);
  ::testing::TestEventListeners& listeners =
      ::testing::UnitTest::GetInstance()->listeners();
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
