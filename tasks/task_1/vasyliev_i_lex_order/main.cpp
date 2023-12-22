// Copyright 2023 Vasyliev Ivan
#include <gtest/gtest.h>
#include <string>
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include "./lex_order.h"

TEST(LexicographicalOrder, NotEqSize1) {
    boost::mpi::communicator world;
    int acting_processes = world.size() - 1;
    const char* a = "qweqqq qweqq";
    const char* b = "qweqqq qweq";
    int p_cmp, s_cmp;
    if (world.rank() == 0) {
        p_cmp = parallel_lex_cmp(a, b, strlen(a), strlen(b));
        world.barrier();
        ASSERT_GT(p_cmp, 0);
    } else {
        parallel_lex_cmp(nullptr, nullptr, strlen(a), strlen(b));
        world.barrier();
    }
}

TEST(LexicographicalOrder, NotEqSize2) {
    boost::mpi::communicator world;
    int acting_processes = world.size() - 1;
    const char* a = "qweqqq qweq";
    const char* b = "qweqqq qweqq";
    int p_cmp, s_cmp;
    if (world.rank() == 0) {
        p_cmp = parallel_lex_cmp(a, b, strlen(a), strlen(b));
        world.barrier();
        ASSERT_LT(p_cmp, 0);
    } else {
        parallel_lex_cmp(nullptr, nullptr, strlen(a), strlen(b));
        world.barrier();
    }
}

TEST(LexicographicalOrder, ShorterString) {
    boost::mpi::communicator world;
    int acting_processes = world.size() - 1;
    const char* a = "qweqqq qweqq";
    const char* b = "qweqqq qweqw";
    int p_cmp, s_cmp;
    if (world.rank() == 0) {
        p_cmp = parallel_lex_cmp(a, b, strlen(a), strlen(b));
        world.barrier();
        ASSERT_LT(p_cmp, 0);
    } else {
        parallel_lex_cmp(nullptr, nullptr, strlen(a), strlen(b));
        world.barrier();
    }
}

TEST(LexicographicalOrder, BiggerString) {
    boost::mpi::communicator world;
    int acting_processes = world.size() - 1;
    const char* a = "qwewqq qweqq";
    const char* b = "qweqqq qweqw";
    int p_cmp, s_cmp;
    if (world.rank() == 0) {
        p_cmp = parallel_lex_cmp(a, b, strlen(a), strlen(b));
        world.barrier();
        ASSERT_GT(p_cmp, 0);
    } else {
        parallel_lex_cmp(nullptr, nullptr, strlen(a), strlen(b));
        world.barrier();
    }
}

TEST(LexicographicalOrder, EqualString) {
    boost::mpi::communicator world;
    int acting_processes = world.size() - 1;
    const char* a = "qwewqq qweqq";
    const char* b = "qwewqq qweqq";
    int p_cmp, s_cmp;
    if (world.rank() == 0) {
        p_cmp = parallel_lex_cmp(a, b, strlen(a), strlen(b));
        world.barrier();
        ASSERT_EQ(p_cmp, 0);
    } else {
        parallel_lex_cmp(nullptr, nullptr, strlen(a), strlen(b));
        world.barrier();
    }
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
