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
TEST(CountSentencesTest, SentencesWithPunctuation) {
    std::string sentence = "This is the first sentence."
        " What about the second sentence ? This is the third sentence!";
    int result = countSentences(sentence);
    EXPECT_EQ(result, 3);
}
TEST(CountSentencesTest, MathematicalExpressions) {
    std::string sentence = "The square root of 25 is 5. 2+2=4, and 3+3 is 6.";
    int result = countSentences(sentence);
    EXPECT_EQ(result, 2);
}
TEST(CountSentencesTest, SpecialSymbols) {
    std::string sentence = "This is the first sentence. @%$";
    int result = countSentences(sentence);
    EXPECT_EQ(result, 1);
}
int main(int argc, char** argv) {
    boost::mpi::environment env(argc, argv);
    boost::mpi::communicator world;

    ::testing::InitGoogleTest(&argc, argv);
    int testResult = RUN_ALL_TESTS();

    return testResult;
}
