// Copyright 2023 Isaev Dmitriy
#include <gtest/gtest.h>
#include <mpi.h>
#include <iostream>
#include "./sentences_count.h"

TEST(Parallel_Sentences_Count, Test1) {
  int rankProc = 0;
  int numProc = 0;

  MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
  MPI_Comm_size(MPI_COMM_WORLD, &numProc);

  std::string str("ABBBBBBBBB. HEpleeeeeeeeeee mee pleeeees. dadadadadadadadada?");

  int result = countParallel(str);

  if (rankProc == 0) {
    int resSeq = countSequence(str);
    ASSERT_EQ(result, resSeq);
  }
}

TEST(Parallel_Sentences_Count, Test2) {
  int rankProc = 0;
  int numProc = 0;

  MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
  MPI_Comm_size(MPI_COMM_WORLD, &numProc);

  std::string str(
      "ABBBBBBBBB HEpleeeeeeeeeee mee pleeeees dadadadad");

  int result = countParallel(str);

  if (rankProc == 0) {
    int resSeq = countSequence(str);
    ASSERT_EQ(result, resSeq);
    ASSERT_EQ(0, resSeq);
  }
}

TEST(Parallel_Sentences_Count, Test3) {
  int rankProc = 0;
  int numProc = 0;

  MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
  MPI_Comm_size(MPI_COMM_WORLD, &numProc);

  std::string str(
      "..........????...!!!!!!!");

  int result = countParallel(str);

  if (rankProc == 0) {
    int resSeq = countSequence(str);
    ASSERT_EQ(result, resSeq);
  }
}

TEST(Parallel_Sentences_Count, Test4) {
  int rankProc = 0;
  int numProc = 0;

  MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
  MPI_Comm_size(MPI_COMM_WORLD, &numProc);

  std::string str(1000, ' ');

  for (int i = 0; i < str.length(); i++) {
    str[i] = static_cast<char>(63 + (std::rand() % (90 - 63)));
  }

  int result = countParallel(str);

  if (rankProc == 0) {
    int resSeq = countSequence(str);
    ASSERT_EQ(result, resSeq);
  }
}

TEST(Parallel_Sentences_Count, Test5) {
  int rankProc = 0;
  int numProc = 0;

  MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
  MPI_Comm_size(MPI_COMM_WORLD, &numProc);

  std::string str("");

  int result = countParallel(str);

  if (rankProc == 0) {
    int resSeq = countSequence(str);
    ASSERT_EQ(result, resSeq);
  }
}

int main(int argc, char **argv) {
  int result_code = 0;

  ::testing::InitGoogleTest(&argc, argv);
  ::testing::TestEventListeners &listeners =
      ::testing::UnitTest::GetInstance()->listeners();

  if (MPI_Init(&argc, &argv) != MPI_SUCCESS) MPI_Abort(MPI_COMM_WORLD, -1);
  result_code = RUN_ALL_TESTS();
  MPI_Finalize();

  return result_code;
}
