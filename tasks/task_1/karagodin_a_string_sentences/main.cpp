// Copyright 2023 Karagodin Andrey

#include <mpi.h>
#include <gtest/gtest.h>
#include "./string_sentences.h"


TEST(SentenceCounterTest, BasicTest) {
    std::string input = "This is a sentence. This is another sentence.";
    EXPECT_EQ(SentenceCounter::countSentences(input), 2);
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
