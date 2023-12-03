// Copyright 2023 Safarov Nurlan

#include "gtest/gtest.h"
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "task_1/safarov_n_non_matching_symbols/number_of_different_symbols.h"

TEST(Mismatched_Symbols, Test_1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::string first_string = "world";
    std::string second_string = "world";

    int result = numberOfNonMatchingCharacters_Parallel(&first_string, &second_string);

    if (rank == 0) {
        ASSERT_EQ(result, numberOfNonMatchingCharacters_Consistently(first_string, second_string));
    }
}

TEST(Mismatched_Symbols, Test_2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::string first_string = "VKTeam";
    std::string second_string = "vTKmaes";

    int result = numberOfNonMatchingCharacters_Parallel(&first_string, &second_string);

    if (rank == 0) {
        ASSERT_EQ(result, numberOfNonMatchingCharacters_Consistently(first_string, second_string));
    }
}

TEST(Mismatched_Symbols, Test_3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::string first_string = "studentbixr";
    std::string second_string = "student";

    int result = numberOfNonMatchingCharacters_Parallel(&first_string, &second_string);

    if (rank == 0) {
        ASSERT_EQ(result, numberOfNonMatchingCharacters_Consistently(first_string, second_string));
    }
}

TEST(Mismatched_Symbols, Test_4_Random_Strings) {
    int rank;
    std::string first_string, second_string;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    first_string = generateRandomString(10);
#ifdef _WIN32
    Sleep(1);
#else
    usleep(1000);
#endif
    second_string = generateRandomString(10);

    int result = numberOfNonMatchingCharacters_Parallel(&first_string, &second_string);

    if (rank == 0) {
        ASSERT_EQ(result, numberOfNonMatchingCharacters_Consistently(first_string, second_string));
    }
}

TEST(Mismatched_Symbols, Test_5_Random_Strings) {
    int rank;
    std::string first_string, second_string;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    first_string = generateRandomString(5);
#ifdef _WIN32
    Sleep(1);
#else
    usleep(1000);
#endif
    second_string = generateRandomString(2);

    int result = numberOfNonMatchingCharacters_Parallel(&first_string, &second_string);

    if (rank == 0) {
        ASSERT_EQ(result, numberOfNonMatchingCharacters_Consistently(first_string, second_string));
    }
}

TEST(Mismatched_Symbols, Test_6_Random_Strings) {
    int rank;
    std::string first_string, second_string;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    first_string = generateRandomString(4);
#ifdef _WIN32
    Sleep(1);
#else
    usleep(1000);
#endif
    second_string = generateRandomString(6);

    int result = numberOfNonMatchingCharacters_Parallel(&first_string, &second_string);

    if (rank == 0) {
        ASSERT_EQ(result, numberOfNonMatchingCharacters_Consistently(first_string, second_string));
    }
}

TEST(Mismatched_Symbols, Test_7_Random_Strings) {
    int rank;
    std::string first_string, second_string;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    first_string = generateRandomString(5);
    second_string = generateRandomString(5);

    int result = numberOfNonMatchingCharacters_Parallel(&first_string, &second_string);

    if (rank == 0) {
        ASSERT_EQ(result, numberOfNonMatchingCharacters_Consistently(first_string, second_string));
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    if (MPI_Init(&argc, &argv) != MPI_SUCCESS) MPI_Abort(MPI_COMM_WORLD, -1);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }
    int exec_code = RUN_ALL_TESTS();
    MPI_Finalize();
    return exec_code;
}
