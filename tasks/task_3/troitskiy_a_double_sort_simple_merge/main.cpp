// Copyright 2023 Troitskiy Alexandr

#include <gtest/gtest.h>
#include <mpi.h>
#include "./double_radix_sort.h"

void print_array(std::vector<double> a, int n) {
    std::cout << "================\n";
    for (int i = 0; i < n; i++) {
        std::cout << a[i] << ' ';
    }
    std::cout << "\n";
    std::cout << "================\n";
}

TEST(MPI_TESTS, Test_size_16) {
    // assemble
    int rank = 0;
    int proc_count = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &proc_count);
    int n = 16;
    std::vector<double> actual, expected;
    if (rank == 0) {
        actual = generateRandomDoubleVector(n, 1, 100);
        expected = actual;
        sort(expected.begin(), expected.end());
    }
    // act
    actual = DoubleRadixSortParallel(actual);
    // assert
    if (rank == 0) {
        for (int i = 0; i < n; i++) {
            ASSERT_EQ(actual[i], expected[i]);
        }
    }
}

TEST(MPI_TESTS, Test_size_48) {
    // assemble
    int rank = 0;
    int proc_count = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &proc_count);
    int n = 48;
    std::vector<double> actual, expected;
    if (rank == 0) {
        actual = generateRandomDoubleVector(n, 1, 100);
        expected = actual;
        sort(expected.begin(), expected.end());
    }
    // act
    actual = DoubleRadixSortParallel(actual);
    // assert
    if (rank == 0) {
        for (int i = 0; i < n; i++) {
            ASSERT_EQ(actual[i], expected[i]);
        }
    }
}


// TEST(MPI_TESTS, Test_only_negatives) {
//     // assemble
//     int rank = 0;
//     int proc_count = 0;
//     MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//     MPI_Comm_size(MPI_COMM_WORLD, &proc_count);
//     int n = 64;
//     std::vector<double> actual, expected;
//     if (rank == 0) {
//         actual = generateRandomDoubleVector(n, 1, 100);
//         for (int i = 0; i < n; i++) {
//             actual[i] *= -1;
//         }
//         expected = actual;
//         sort(expected.begin(), expected.end());
//     }
//     // act
//     actual = DoubleRadixSortParallel(actual);
//     // assert
//     if (rank == 0) {
//         for (int i = 0; i < n; i++) {
//             ASSERT_EQ(actual[i], expected[i]);
//         }
//     }
// }

TEST(MPI_TESTS, Test_1_element) {
    // assemble
    int rank = 0;
    int proc_count = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &proc_count);
    int n = 1;
    std::vector<double> actual, expected;
    if (rank == 0) {
        actual = generateRandomDoubleVector(n, 1, 100);
        expected = actual;
        sort(expected.begin(), expected.end());
    }
    // act
    actual = DoubleRadixSortParallel(actual);
    // assert
    if (rank == 0) {
        for (int i = 0; i < n; i++) {
            ASSERT_EQ(actual[i], expected[i]);
        }
    }
}

TEST(MPI_TESTS, Test_size_512) {
    // assemble
    int rank = 0;
    int proc_count = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &proc_count);
    int n = 512;
    std::vector<double> actual, expected;
    if (rank == 0) {
        actual = generateRandomDoubleVector(n, 1, 100);
        expected = actual;
        sort(expected.begin(), expected.end());
    }
    // act
    actual = DoubleRadixSortParallel(actual);
    // assert
    if (rank == 0) {
        for (int i = 0; i < n; i++) {
            ASSERT_EQ(actual[i], expected[i]);
        }
    }
}

int main(int argc, char** argv) {
    int result = 0;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners &listeners = ::testing::UnitTest::GetInstance()->listeners();

    if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, -1);
    int rankProc = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    if (rankProc != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }
    result = RUN_ALL_TESTS();
    MPI_Finalize();
    return result;
}
