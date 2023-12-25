// Copyright 2023 Karagodin Andrey

#include <mpi.h>
#include <gtest/gtest.h>
#include "./string_sentences.h"


TEST(SentenceCounterTest, BasicTest) {
    int rank = 0, size = 0, res = 0;
    std::string input = "This is a small sentence. This is another sentence.";
    std::cout << input << std::endl;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);


    res = countSentences(input);
    if (rank == 0) {
        int resUnp = countFunc(input);
        ASSERT_EQ(res, resUnp);
  }
  printf("Process: %d, size: %d\n", rank, size);
}

TEST(SentenceCounterTest, BiggerSentencesAmountTest) {
    int rank = 0, size = 0, res = 0;
    std::string input = "This. Sentence. Has. More. Sentences!";
    std::cout << input << std::endl;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    res = countSentences(input);
    if (rank == 0) {
        int resUnp = countFunc(input);
        ASSERT_EQ(res, resUnp);
  }
  printf("Process: %d, size: %d\n", rank, size);
}

TEST(SentenceCounterTest, BiggerSentencesTextTest) {
    int rank = 0, size = 0, res = 0;
    std::string input = "This is a small sentence. ";
    std::string input2 = "This is another sentence but bigger. ";
    std::string input3 = "This sentence is actually huge though.";
    std::string hugeInput = "";
    std::cout << hugeInput << std::endl;
    hugeInput.append(input);
    hugeInput.append(input2);
    hugeInput.append(input3);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    res = countSentences(input);
    if (rank == 0) {
        int resUnp = countFunc(input);
        ASSERT_EQ(res, resUnp);
  }
  printf("Process: %d, size: %d\n", rank, size);
}

TEST(SentenceCounterTest, MoreSignsSmallTextTest) {
    int rank = 0, size = 0, res = 0;
    std::string input = "This is a small sentence! This is another sentence?";
    std::cout << input << std::endl;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    res = countSentences(input);
    if (rank == 0) {
        int resUnp = countFunc(input);
        ASSERT_EQ(res, resUnp);
  }
  printf("Process: %d, size: %d\n", rank, size);
}

TEST(SentenceCounterTest, MoreSignsBigTextTest) {
    int rank = 0, size = 0, res = 0;
    std::string input = "This is a small sentence. ";
    std::string input2 = "This is another sentence but bigger! ";
    std::string input3 = "This sentence is actually huge though?";
    std::string hugeInput = "";

    std::cout << hugeInput << std::endl;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    res = countSentences(input);
    if (rank == 0) {
        int resUnp = countFunc(input);
        ASSERT_EQ(res, resUnp);
  }
  printf("Process: %d, size: %d\n", rank, size);
}

TEST(SentenceCounterTest, SingleSignTextTest) {
    int rank = 0, size = 0, res = 0;
    std::string input = "FAILED.";
    std::cout << input << std::endl;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    res = countSentences(input);
    if (rank == 0) {
        int resUnp = countFunc(input);
        ASSERT_EQ(res, resUnp);
  }
  printf("Process: %d, size: %d\n", rank, size);
}

int main(int argc, char** argv) {
  int result_code = 0;

  ::testing::InitGoogleTest(&argc, argv);
  ::testing::TestEventListeners& listeners =
      ::testing::UnitTest::GetInstance()->listeners();

  MPI_Init(&argc, &argv);
  result_code = RUN_ALL_TESTS();
  MPI_Finalize();

  return result_code;
}
