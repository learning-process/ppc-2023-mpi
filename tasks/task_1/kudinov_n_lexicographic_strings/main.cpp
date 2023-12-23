// Copyright 2023 Kudinov Nikita
#include <gtest/gtest.h>
#include <boost/mpi.hpp>

#include "task_1/kudinov_n_lexicographic_strings/lexicographic_strings.h"

void base_test(const std::string& left, const std::string& right, const bool expect) {
    boost::mpi::communicator world;

    bool result_parallel = are_strings_lexicographically_ordered_parallel(left, right);

    if (world.rank() == 0) {
        bool result_sequential = are_strings_lexicographically_ordered_sequential(left, right);

        ASSERT_EQ(result_sequential, result_parallel);

        if (expect) {
            ASSERT_TRUE(result_sequential);
            ASSERT_TRUE(result_parallel);
        } else {
            ASSERT_FALSE(result_sequential);
            ASSERT_FALSE(result_parallel);
        }
    }
}

TEST(Parallel_Operations_MPI, Equal_Strings) {
    base_test("testTESTtest", "testTESTtest", true);
}

TEST(Parallel_Operations_MPI, Lexicographically_Ordered_Strings_Same_Size) {
    base_test("ABCDEFG", "ABCDEFH", true);
}

TEST(Parallel_Operations_MPI, Lexicographically_Ordered_Strings_Different_Size) {
    base_test("ABCDEFG", "ABCDEFHAAAAAAAA", true);
}

TEST(Parallel_Operations_MPI, Not_Lexicographically_Ordered_Strings_Same_Size) {
    base_test("ABCDEFH", "ABCDEFG", false);
}

TEST(Parallel_Operations_MPI, Not_Lexicographically_Ordered_Strings_Different_Size) {
    base_test("ABCDEFHAAAAAAAA", "ABCDEFG", false);
}

int main(int argc, char** argv) {
    boost::mpi::environment env(argc, argv);
    boost::mpi::communicator world;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();

    if (world.rank() != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }

    return RUN_ALL_TESTS();
}
