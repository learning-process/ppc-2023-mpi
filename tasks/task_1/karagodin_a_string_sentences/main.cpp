// Copyright 2023 Karagodin Andrey

#include <mpi.h>
#include <gtest/gtest.h>
#include "./string_sentences.h"


TEST(SentenceCounterTest, BasicTest) {
    int rank = 0, size = 0, res;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::string input = "This is a small sentence. This is another sentence.";
    std::cout << input << std::endl;
    res = SentenceCounter::countSentences(input);
    if (rank == 0) {
        int resUnp = countFunc(input);
        ASSERT_EQ(res, resUnp);
  }
}

TEST(SentenceCounterTest, BiggerSentencesAmountTest) {
    int rank = 0, size = 0, res;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    std::string input = "This. Sentence. Has. More. Sentences!";
    std::cout << input << std::endl;
    res = SentenceCounter::countSentences(input);
    if (rank == 0) {
        int resUnp = countFunc(input);
        ASSERT_EQ(res, resUnp);
  }
}

TEST(SentenceCounterTest, BiggerSentencesTextTest) {
    int rank = 0, size = 0, res;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    std::string input = "This is a small sentence. ";
    std::string input2 = "This is another sentence but bigger. ";
    std::string input3 = "This sentence is actually huge though.";
    std::string hugeInput = "";
    hugeInput.append(input);
    hugeInput.append(input2);
    hugeInput.append(input3);
    std::cout << hugeInput << std::endl;
    res = SentenceCounter::countSentences(input);
    if (rank == 0) {
        int resUnp = countFunc(input);
        ASSERT_EQ(res, resUnp);
  }
}

TEST(SentenceCounterTest, MoreSignsSmallTextTest) {
    int rank = 0, size = 0, res;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    std::string input = "This is a small sentence! This is another sentence?";
    std::cout << input << std::endl;
    res = SentenceCounter::countSentences(input);
    if (rank == 0) {
        int resUnp = countFunc(input);
        ASSERT_EQ(res, resUnp);
  }
}

TEST(SentenceCounterTest, MoreSignsBigTextTest) {
    int rank = 0, size = 0, res;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
      std::string input = "This is a small sentence. ";
    std::string input2 = "This is another sentence but bigger! ";
    std::string input3 = "This sentence is actually huge though?";
    std::string hugeInput = "";
    hugeInput.append(input);
    hugeInput.append(input2);
    hugeInput.append(input3);
    std::cout << hugeInput << std::endl;
    res = SentenceCounter::countSentences(input);
    if (rank == 0) {
        int resUnp = countFunc(input);
        ASSERT_EQ(res, resUnp);
  }
}

TEST(SentenceCounterTest, SingleSignTextTest) {
    int rank = 0, size = 0, res;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    std::string input = "F.";
    std::cout << input << std::endl;
    res = SentenceCounter::countSentences(input);
    if (rank == 0) {
        int resUnp = countFunc(input);
        ASSERT_EQ(res, resUnp);
  }
}

int main(int argc, char** argv) {
    int result_code = 0;

  ::testing::InitGoogleTest(&argc, argv);
  ::testing::TestEventListeners &listeners =
      ::testing::UnitTest::GetInstance()->listeners();

  if (MPI_Init(&argc, &argv) != MPI_SUCCESS) MPI_Abort(MPI_COMM_WORLD, -1);
  result_code = RUN_ALL_TESTS();
  MPI_Finalize();

  return result_code;
}
