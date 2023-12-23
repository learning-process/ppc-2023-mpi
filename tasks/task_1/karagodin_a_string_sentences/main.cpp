// Copyright 2023 Karagodin Andrey

#include <mpi.h>
#include <gtest/gtest.h>
#include "./string_sentences.h"


TEST(SentenceCounterTest, BasicTest) {
    std::string input = "This is a small sentence. This is another sentence.";
    EXPECT_EQ(SentenceCounter::countSentences(input), 2);
}

TEST(SentenceCounterTest, BiggerSentencesAmountTest) {
    std::string input = "This. Sentence. Has. More. Sentences!";
    EXPECT_EQ(SentenceCounter::countSentences(input), 5);
}

TEST(SentenceCounterTest, BiggerSentencesTextTest) {
    std::string input = "This is a small sentence. ";
    std::string input2 = "This is another sentence but bigger. ";
    std::string input3 = "This sentence is actually huge though.";
    std::string hugeInput = input + input2 + input3;
    EXPECT_EQ(SentenceCounter::countSentences(hugeInput), 3);
}

TEST(SentenceCounterTest, MoreSignsSmallTextTest) {
    std::string input = "This is a small sentence! This is another sentence?";
    EXPECT_EQ(SentenceCounter::countSentences(input), 2);
}

TEST(SentenceCounterTest, MoreSignsBigTextTest) {
      std::string input = "This is a small sentence. ";
    std::string input2 = "This is another sentence but bigger! ";
    std::string input3 = "This sentence is actually huge though?";
    std::string hugeInput = "";
    hugeInput.append(input);
    hugeInput.append(input2);
    hugeInput.append(input3);
    EXPECT_EQ(SentenceCounter::countSentences(hugeInput), 3);
}

TEST(SentenceCounterTest, SingleSignTextTest) {
    std::string input = "F.";
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
