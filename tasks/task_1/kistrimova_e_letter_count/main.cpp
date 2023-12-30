// Copyright 2023 Kistrimova Ekaterina
#include <gtest/gtest.h>
#include "./letter_count.h"
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>


TEST(CountLetterTest, RandomString) {
    boost::mpi::environment env;
    boost::mpi::communicator world;
    int size = 10;
    std::string str = generateRandomRow(size);
    int par_result = count_letters_par(str);

    if (world.rank() == 0) {
        int seq_result = count_letters_seq(str);
        ASSERT_EQ(par_result, seq_result);
    }
}

TEST(CountLetterTest, SingleLetterString) {
    boost::mpi::environment env;
    boost::mpi::communicator world;

    std::string str = "7571395385f747474747";
    int par_result = count_letters_par(str);

    if (world.rank() == 0) {
        int seq_result = count_letters_seq(str);
        ASSERT_EQ(par_result, seq_result);
    }
}

TEST(CountLetterTest, AllLettersString) {
    boost::mpi::environment env;
    boost::mpi::communicator world;

    std::string str = "MyNameIsKate";
    int par_result = count_letters_par(str);

    if (world.rank() == 0) {
        int seq_result = count_letters_seq(str);
        ASSERT_EQ(par_result, seq_result);
    }
}

TEST(CountLetterTest, HalfLettersHalfSymbolsString) {
    boost::mpi::environment env;
    boost::mpi::communicator world;

    std::string str = "a1a1a1a1a1a";
    int par_result = count_letters_par(str);

    if (world.rank() == 0) {
        int seq_result = count_letters_seq(str);
        ASSERT_EQ(par_result, seq_result);
    }
}

TEST(CountLetterTest, NoLettersString) {
    boost::mpi::environment env;
    boost::mpi::communicator world;

    std::string str = "122333444455555";
    int par_result = count_letters_par(str);

    if (world.rank() == 0) {
        int seq_result = count_letters_seq(str);
        ASSERT_EQ(par_result, seq_result);
    }
}

int main(int argc, char **argv) {
    boost::mpi::environment env(argc, argv);
    boost::mpi::communicator world;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners &listeners =
        ::testing::UnitTest::GetInstance()->listeners();
    if (world.rank() != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }
    return RUN_ALL_TESTS();
}
