// Copyright 2023 Lebedeva Ksenia
#include <gtest/gtest.h>
#include <mpi.h>
#include <vector>
#include <numeric>
#include "./words_count.h"

TEST(Count_Of_Words, Test_Simple) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::string str;
    const int size = 13;
    if (rank == 0) {
        str = "Simple string";
    }

    int count = getWordsCountParallel(str, size);

    if (rank == 0) {
        ASSERT_EQ(2, count);
    }
}

TEST(Count_Of_Words, Test_Random_String) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::string str;
    const int size = 120;
    if (rank == 0) {
        str = getRandString(size);
    }

    int count = getWordsCountParallel(str, size);

    if (rank == 0) {
        int reference_count = getWordsCountSequentially(str, size);
        ASSERT_EQ(reference_count, count);
    }
}

TEST(Count_Of_Words, Test_Empty_String) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::string str;
    const int size = 0;
    if (rank == 0) {
        str = "";
    }

    int count = getWordsCountParallel(str, size);

    if (rank == 0) {
        ASSERT_EQ(0, count);
    }
}

TEST(Count_Of_Words, Test_String_Without_Words) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::string str;
    const int size = 17;
    if (rank == 0) {
        str = "                 ";
    }

    int count = getWordsCountParallel(str, size);

    if (rank == 0) {
        ASSERT_EQ(0, count);
    }
}

TEST(Count_Of_Words, Test_One_Word) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::string str;
    const int size = 45;
    if (rank == 0) {
        str = "jhgfdwxcvgftDFERTYUprisencolinensinainciusol";
    }

    int count = getWordsCountParallel(str, size);

    if (rank == 0) {
        ASSERT_EQ(1, count);
    }
}

TEST(Count_Of_Words, Test_Short_String) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::string str;
    const int size = 1;
    if (rank == 0) {
        str = "i";
    }

    int count = getWordsCountParallel(str, size);

    if (rank == 0) {
        ASSERT_EQ(1, count);
    }
}

TEST(Count_Of_Words, Test_Large_String) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::string str;
    const int size = 10000;
    if (rank == 0) {
        str = getRandString(size);
    }

    int count = getWordsCountParallel(str, size);

    if (rank == 0) {
        int reference_count = getWordsCountSequentially(str, size);
        ASSERT_EQ(reference_count, count);
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();
    if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, -1);
    int rcode = RUN_ALL_TESTS();
    MPI_Finalize();
    return rcode;
}
