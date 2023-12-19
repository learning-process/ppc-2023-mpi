//  Copyright 2023 Petrov Maksim

#include <gtest/gtest.h>
#include <mpi.h>
#include <vector>
#include "./dense_matr_cannon.h"

TEST(SEQUENTIAL_OPERATIONS, sequential_operations_example_4x4) {
    std::vector<double> first_matrix = { 1.1 , 2.3, 3.8, 5.1,
                                        7.2, 2.0, 3.5, -5.5,
                                        -9.1, 1.9, -5.3, 4.7,
                                        7.4, -8.1, 3.2, 7.7 };

    std::vector<double> second_matrix = { 8.9 , 3.8, 5.5, 7.7,
                                          1.2, 25.2, -23.5, 55.1,
                                          -99.8, 8.2, 9.6, 10.2,
                                          11.5, 85.3, 15.9, -33.2 };

    std::vector<double> expected_result = { -308.04, 528.33, 69.57, 4.64,
                                            -346.07, -362.69, -61.25, 383.94,
                                            504.28, 370.75, -70.85, -175.48,
                                            -174.67, 507.05, 384.2, -612.33 };
    std::vector<double> result(4 * 4, 0.0);
    getSequentialOperations(first_matrix, second_matrix, &result, 4);

    for (std::vector<double>::size_type i = 0; i < result.size(); ++i) {
        ASSERT_NEAR(result[i], expected_result[i], 0.001);
    }
}

TEST(PARALLEL_OPERATIONS, work_with_matirx_size_88) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<double>::size_type matrix_size = 88;
    std::vector<double> first_matrix(matrix_size * matrix_size, 0.0);
    std::vector<double> second_matrix(matrix_size * matrix_size, 0.0);

    if (rank == 0) {
        generate_rand_matrix(&first_matrix, matrix_size, 10, 100);
        generate_rand_matrix(&second_matrix, matrix_size, 10, 100);
    }

    std::vector<double> result = getParallelOperations(first_matrix, second_matrix, matrix_size);

    if (rank == 0) {
        std::vector<double> expected_result(matrix_size * matrix_size, 0.0);
        getSequentialOperations(first_matrix, second_matrix, &expected_result, matrix_size);

        for (std::vector<double>::size_type i = 0; i < result.size(); ++i) {
            ASSERT_NEAR(result[i], expected_result[i], 0.001);
        }
    }
}


TEST(PARALLEL_OPERATIONS, work_with_matirx_size_16) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<double>::size_type matrix_size = 16;
    std::vector<double> first_matrix(matrix_size * matrix_size, 0.0);
    std::vector<double> second_matrix(matrix_size * matrix_size, 0.0);

    if (rank == 0) {
        generate_rand_matrix(&first_matrix, matrix_size, -100, 100);
        generate_rand_matrix(&second_matrix, matrix_size, -100, 100);
    }

    std::vector<double> result = getParallelOperations(first_matrix, second_matrix, matrix_size);

    if (rank == 0) {
        std::vector<double> expected_result(matrix_size * matrix_size, 0.0);
        getSequentialOperations(first_matrix, second_matrix, &expected_result, matrix_size);

        for (std::vector<double>::size_type i = 0; i < result.size(); ++i) {
            ASSERT_NEAR(result[i], expected_result[i], 0.001);
        }
    }
}


TEST(PARALLEL_OPERATIONS, work_with_matirx_size_36) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<double>::size_type matrix_size = 36;
    std::vector<double> first_matrix(matrix_size * matrix_size, 0.0);
    std::vector<double> second_matrix(matrix_size * matrix_size, 0.0);

    if (rank == 0) {
        generate_rand_matrix(&first_matrix, matrix_size, 50, 100);
        generate_rand_matrix(&second_matrix, matrix_size, 50, 100);
    }

    std::vector<double> result = getParallelOperations(first_matrix, second_matrix, matrix_size);

    if (rank == 0) {
        std::vector<double> expected_result(matrix_size * matrix_size, 0.0);
        getSequentialOperations(first_matrix, second_matrix, &expected_result, matrix_size);

        for (std::vector<double>::size_type i = 0; i < result.size(); ++i) {
            ASSERT_NEAR(result[i], expected_result[i], 0.001);
        }
    }
}


TEST(PARALLEL_OPERATIONS, boost_with_matrix_100x100) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<double>::size_type matrix_size = 100;
    std::vector<double> first_matrix(matrix_size * matrix_size, 0.0);
    std::vector<double> second_matrix(matrix_size * matrix_size, 0.0);

    if (rank == 0) {
        generate_rand_matrix(&first_matrix, matrix_size, 100, 200);
        generate_rand_matrix(&second_matrix, matrix_size, 100, 200);
    }

    auto start_parallel_algorithm = MPI_Wtime();
    std::vector<double> result = getParallelOperations(first_matrix, second_matrix, matrix_size);
    auto finish_parallel_algorithm = MPI_Wtime();

    if (rank == 0) {
        std::vector<double> expected_result(matrix_size * matrix_size, 0.0);

        auto start_sequential_algorithm = MPI_Wtime();
        getSequentialOperations(first_matrix, second_matrix, &expected_result, matrix_size);
        auto finish_sequential_algorithm = MPI_Wtime();

        auto parallel_time = finish_parallel_algorithm - start_parallel_algorithm;
        auto sequential_time = finish_sequential_algorithm - start_sequential_algorithm;

        printf("Sequential Time = %lf\nParallel Time = %lf\nTime_Boost = %lf\n",
            sequential_time, parallel_time, sequential_time / parallel_time);
    }
}


TEST(PARALLEL_OPERATIONS, boost_with_matrix_444x444) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<double>::size_type matrix_size = 444;
    std::vector<double> first_matrix(matrix_size * matrix_size, 0.0);
    std::vector<double> second_matrix(matrix_size * matrix_size, 0.0);

    if (rank == 0) {
        generate_rand_matrix(&first_matrix, matrix_size, -100, 100);
        generate_rand_matrix(&second_matrix, matrix_size, -100, 100);
    }

    auto start_parallel_algorithm = MPI_Wtime();
    std::vector<double> result = getParallelOperations(first_matrix, second_matrix, matrix_size);
    auto finish_parallel_algorithm = MPI_Wtime();

    if (rank == 0) {
        std::vector<double> expected_result(matrix_size * matrix_size, 0.0);

        auto start_sequential_algorithm = MPI_Wtime();
        getSequentialOperations(first_matrix, second_matrix, &expected_result, matrix_size);
        auto finish_sequential_algorithm = MPI_Wtime();

        auto parallel_time = finish_parallel_algorithm - start_parallel_algorithm;
        auto sequential_time = finish_sequential_algorithm - start_sequential_algorithm;

        printf("Sequential Time = %lf\nParallel Time = %lf\nTime_Boost = %lf\n",
            sequential_time, parallel_time, sequential_time / parallel_time);
    }
}

int main(int argc, char** argv) {
    int result_code = 0;

    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, -1);
    result_code = RUN_ALL_TESTS();
    MPI_Finalize();

    return result_code;
}
