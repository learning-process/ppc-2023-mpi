// Copyright 2023 Lysanova Julia
#include <gtest/gtest.h>
#include <mpi.h>

#include "./graham_convex_hull.h"

TEST(Graham_Convex_Hull, Graham_Method) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<Point> global_vec;
    std::vector<Point> global_vec_answer;

    if (rank == 0) {
        global_vec = {{0, 1}, {0, 4}, {2, 7}, {2, 16}, {3, 16}, {3, 16}, {5, 1}, {5, 12}, {5, 17}, {7, 12}};
        global_vec_answer = {{0, 1}, {0, 4}, {2, 16}, {5, 17}, {7, 12}, {5, 1}};
        std::vector<Point> not_parallel = GrahamMethod(global_vec);
        ASSERT_EQ(global_vec_answer.size(), not_parallel.size());
        for (std::vector<int>::size_type i = 0; i < global_vec_answer.size(); ++i) {
            ASSERT_EQ(global_vec_answer[i].x, not_parallel[i].x);
            ASSERT_EQ(global_vec_answer[i].y, not_parallel[i].y);
        }
    }
}

TEST(Graham_Convex_Hull, GrahamParallel_Method_1) {
    int rank, Commsize;
    MPI_Comm_size(MPI_COMM_WORLD, &Commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<Point> global_vec;
    const std::vector<int>::size_type size = 12;

    if (rank == 0) {
        global_vec = random(size);
    }

    std::vector<Point> parallel = parallelGrahamMethod(global_vec, size);
    if (rank == 0) {
        std::vector<Point> not_parallel = GrahamMethod(global_vec);
        ASSERT_EQ(parallel.size(), not_parallel.size());
        for (std::vector<int>::size_type i = 0; i < parallel.size(); ++i) {
            ASSERT_EQ(parallel[i].x, not_parallel[i].x);
            ASSERT_EQ(parallel[i].y, not_parallel[i].y);
        }
    }
}

TEST(Graham_Convex_Hull, GrahamParallel_Method_2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<Point> global_vec;
    const std::vector<int>::size_type size = 71;

    if (rank == 0) {
        global_vec = random(size);
    }
    std::vector<Point> parallel = parallelGrahamMethod(global_vec, size);
    if (rank == 0) {
        std::vector<Point> not_parallel = GrahamMethod(global_vec);
        ASSERT_EQ(parallel.size(), not_parallel.size());
        for (std::vector<int>::size_type i = 0; i < parallel.size(); ++i) {
            ASSERT_EQ(parallel[i].x, not_parallel[i].x);
            ASSERT_EQ(parallel[i].y, not_parallel[i].y);
        }
    }
}

TEST(Graham_Convex_Hull, GrahamParallel_Method_3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<Point> global_vec;
    const std::vector<int>::size_type size = 123;

    if (rank == 0) {
        global_vec = random(size);
    }

    std::vector<Point> parallel = parallelGrahamMethod(global_vec, size);

    if (rank == 0) {
        std::vector<Point> not_parallel = GrahamMethod(global_vec);
        ASSERT_EQ(parallel.size(), not_parallel.size());
        for (std::vector<int>::size_type i = 0; i < parallel.size(); ++i) {
            ASSERT_EQ(parallel[i].x, not_parallel[i].x);
            ASSERT_EQ(parallel[i].y, not_parallel[i].y);
        }
    }
}

TEST(Graham_Convex_Hull, GrahamParallel_Method_4) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<Point> global_vec;
    const std::vector<int>::size_type size = 500;

    if (rank == 0) {
        global_vec = random(size);
    }

    std::vector<Point> parallel = parallelGrahamMethod(global_vec, size);

    if (rank == 0) {
        std::vector<Point> not_parallel = GrahamMethod(global_vec);
        ASSERT_EQ(parallel.size(), not_parallel.size());
        for (std::vector<int>::size_type i = 0; i < parallel.size(); ++i) {
            ASSERT_EQ(parallel[i].x, not_parallel[i].x);
            ASSERT_EQ(parallel[i].y, not_parallel[i].y);
        }
    }
}

int main(int argc, char** argv) {
    int result_code = 0;
    int rank = 0;

    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    if (MPI_Init(&argc, &argv) != MPI_SUCCESS) MPI_Abort(MPI_COMM_WORLD, -1);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }

    result_code = RUN_ALL_TESTS();
    MPI_Finalize();

    return result_code;
}
