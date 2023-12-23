// Copyright 2023 Pivovarov Alexey
#include <gtest/gtest.h>
#include "./letter_count.h"
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>


TEST(CountLetterTest, RandomString) {
    boost::mpi::environment env;
    boost::mpi::communicator world;
    int size = 10;
    std::string str = generateRandomRow(size);
    int parallel_result = count_letters_par(str);

    if (world.rank() == 0) {
        int sequential_result = count_letters_seq(str);
        ASSERT_EQ(parallel_result, sequential_result);
    }
}

TEST(CountLetterTest, SingleLetterString) {
    boost::mpi::environment env;
    boost::mpi::communicator world;

    std::string str = "12314123a9784579762344364";
    int parallel_result = count_letters_par(str);

    if (world.rank() == 0) {
        int sequential_result = count_letters_seq(str);
        ASSERT_EQ(parallel_result, sequential_result);
    }
}

TEST(CountLetterTest, AllLettersString) {
    boost::mpi::environment env;
    boost::mpi::communicator world;

    std::string str = "HelloWorld";
    int parallel_result = count_letters_par(str);

    if (world.rank() == 0) {
        int sequential_result = count_letters_seq(str);
        ASSERT_EQ(parallel_result, sequential_result);
    }
}

TEST(CountLetterTest, HalfLettersHalfSymbolsString) {
    boost::mpi::environment env;
    boost::mpi::communicator world;

    std::string str = "a1b2c3d4e5f6";
    int parallel_result = count_letters_par(str);

    if (world.rank() == 0) {
        int sequential_result = count_letters_seq(str);
        ASSERT_EQ(parallel_result, sequential_result);
    }
}

TEST(CountLetterTest, NoLettersString) {
    boost::mpi::environment env;
    boost::mpi::communicator world;

    std::string str = "1234567890";
    int parallel_result = count_letters_par(str);

    if (world.rank() == 0) {
        int sequential_result = count_letters_seq(str);
        ASSERT_EQ(parallel_result, sequential_result);
    }
}

int main(int argc, char **argv) {
    boost::mpi::environment env(argc, argv);
    boost::mpi::communicator world;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners &listeners = ::testing::UnitTest::GetInstance()->listeners();
    if (world.rank() != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }
    return RUN_ALL_TESTS();
}
