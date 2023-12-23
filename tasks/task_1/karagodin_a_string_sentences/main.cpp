// Copyright 2023 Karagodin Andrey

#include <mpi.h>
#include <gtest/gtest.h>
#include "./string_sentences.h"


TEST(SentenceCounterTest, BasicTest) {
    int rankProc = 0;
    int numProc = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);
    std::string input = "This is a small sentence. This is another sentence.";
    std::cout << input << std::endl;
    EXPECT_EQ(SentenceCounter::countSentences(input), 2);
}

TEST(SentenceCounterTest, BiggerSentencesAmountTest) {
    int rankProc = 0;
    int numProc = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);
    std::string input = "This. Sentence. Has. More. Sentences!";
    std::cout << input << std::endl;
    EXPECT_EQ(SentenceCounter::countSentences(input), 5);
}

TEST(SentenceCounterTest, BiggerSentencesTextTest) {
    int rankProc = 0;
    int numProc = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);
    std::string input = "This is a small sentence. ";
    std::string input2 = "This is another sentence but bigger. ";
    std::string input3 = "This sentence is actually huge though.";
    std::string hugeInput = "";
    hugeInput.append(input);
    hugeInput.append(input2);
    hugeInput.append(input3);
    std::cout << hugeInput << std::endl;
    EXPECT_EQ(SentenceCounter::countSentences(hugeInput), 3);
}

TEST(SentenceCounterTest, MoreSignsSmallTextTest) {
    int rankProc = 0;
    int numProc = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);
    std::string input = "This is a small sentence! This is another sentence?";
    std::cout << input << std::endl;
    EXPECT_EQ(SentenceCounter::countSentences(input), 2);
}

TEST(SentenceCounterTest, MoreSignsBigTextTest) {
    int rankProc = 0;
    int numProc = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);
      std::string input = "This is a small sentence. ";
    std::string input2 = "This is another sentence but bigger! ";
    std::string input3 = "This sentence is actually huge though?";
    std::string hugeInput = "";
    hugeInput.append(input);
    hugeInput.append(input2);
    hugeInput.append(input3);
    std::cout << hugeInput << std::endl;
    EXPECT_EQ(SentenceCounter::countSentences(hugeInput), 3);
}

TEST(SentenceCounterTest, SingleSignTextTest) {
    int rankProc = 0;
    int numProc = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);
    std::string input = "F.";
    std::cout << input << std::endl;
    EXPECT_EQ(SentenceCounter::countSentences(input), 1);
}

int main(int argc, char** argv) {
    int testing_results = 0;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners &listeners =
        ::testing::UnitTest::GetInstance()->listeners();
    if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, -1);
    testing_results = RUN_ALL_TESTS();
    MPI_Finalize();

    return testing_results;
}
