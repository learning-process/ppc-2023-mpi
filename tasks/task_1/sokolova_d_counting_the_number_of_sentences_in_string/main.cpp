  //  Copyright 2023 Sokolova Daria
#include "./counting_the_number_of_sentences_in_string.h"


TEST(ParallelCountSentencesTest, EmptyString) {
    std::string sentence = "";
    int result = parallelCountSentencesInString(sentence);
    EXPECT_EQ(result, 0);
}
TEST(ParallelCountSentencesTest, NoEndingPunctuation) {
    std::string sentence = "Sentence without ending punctuation";
    int result = parallelCountSentencesInString(sentence);
    EXPECT_EQ(result, 0);
}
TEST(ParallelCountSentencesTest, SpecialCharacters) {
    std::string sentence = "Sentence with special characters!@#$";
    int result = parallelCountSentencesInString(sentence);
    EXPECT_EQ(result, 1);
}
TEST(CountSentencesTest, MathematicalExpressions) {
    std::string sentence = "The square root of 25 is 5. 2+2=4, and 3+3 is 6.";
    int result = countSentences(sentence);
    EXPECT_EQ(result, 2);
}
std::string getRandomSentence() {
    srand(static_cast<unsigned int>(time(nullptr)));

    const std::string characters =
        "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,!? ";
    int sentenceLength = std::rand() % 100;
    std::string randomSentence;
    const std::string punctuations = ".!?";
    for (int i = 0; i < sentenceLength; ++i) {
        int charIndex = std::rand() % characters.size();
        randomSentence += characters[charIndex];
    }

    randomSentence[0] = std::toupper(randomSentence[0]);
    int punctuationIndex = std::rand() % punctuations.size();
    randomSentence += punctuations[punctuationIndex];

    return randomSentence;
}
TEST(ParallelCountSentencesInStringTest, RandomSentence) {
    std::string globalSentence;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    globalSentence = getRandomSentence();

    int globalSentenceCount = parallelCountSentencesInString(globalSentence);

    if (rank == 0) {
        int referenceCount = countSentences(globalSentence);
        ASSERT_EQ(referenceCount, globalSentenceCount);
    }
}
int main(int argc, char** argv) {
    boost::mpi::environment env(argc, argv);
    boost::mpi::communicator world;

    ::testing::InitGoogleTest(&argc, argv);
    int testResult = RUN_ALL_TESTS();

    return testResult;
}
