// Copyright 2024 Efimov Andrey
#include <gtest/gtest.h>
#include "task_1/efimov_a_number_of_violations_in_the_vector/number_of_orderly_violations.h"


TEST(Test_num_of_orderly_violations, Test_Sorted_Vector) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> vec(20);
    if (rank == 0) {
        for (size_t i = 0; i < vec.size(); ++i) {
            vec[i] = vec.size() - i - 1;
        }
    }
    int par_num = numberOfOrderValids_ASync(vec, vec.size());
    if (rank == 0) {
        ASSERT_EQ(par_num, vec.size() - 1);
    }
}

TEST(Test_num_of_orderly_violations, Test_Vector_With_Odd_Size) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> vec;
    const int vec_length = 125;
    if (rank == 0) {
        vec = make_random_vector(vec_length);
    }
    int  par_num = numberOfOrderValids_ASync(vec, vec_length);
    if (rank == 0) {
        int seq_num = numberOfOrderValids_Sync(vec);
        ASSERT_EQ(seq_num, par_num);
    }
}

TEST(Test_num_of_orderly_violations, Test_Vector_With_One_Element) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> vec;
    const int vec_length = 1;
    if (rank == 0) {
        vec = make_random_vector(vec_length);
    }
    int  par_num = numberOfOrderValids_ASync(vec, vec_length);
    if (rank == 0) {
        ASSERT_EQ(par_num, 0);
    }
}

TEST(Test_num_of_orderly_violations, Test_Gotten_Vector) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> vec = { 9, 5, 2, 57, 2, 47, 1, 6, 3,
    5, 2, 67, 2, 4, 73, 2, 5 };
    int  par_num = numberOfOrderValids_ASync(vec, vec.size());
    if (rank == 0) {
        ASSERT_EQ(par_num, 8);
    }
}

TEST(Test_num_of_orderly_violations, Test_Vector_With_Identical_Elements) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> vec(20, 5);
    int  par_num = numberOfOrderValids_ASync(vec, vec.size());
    if (rank == 0) {
        ASSERT_EQ(par_num, 0);
    }
}

TEST(Test_num_of_orderly_violations, Test_Only_Gen_Procs) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> vec;
    const int vec_length = 256;
    if (rank == 0) {
        vec = make_random_vector(vec_length);
    }
    int  par_num = numberOfOrderValids_ASync(vec, vec_length);
    if (rank == 0) {
        int seq_num = numberOfOrderValids_Sync(vec);
        ASSERT_EQ(seq_num, par_num);
    }
}

TEST(Test_num_of_orderly_violations, Test_Ordered_Vector) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> vec(20);
    if (rank == 0) {
        for (size_t i = 0; i < vec.size(); ++i) {
            vec[i] = i;
        }
    }
    int  par_num = numberOfOrderValids_ASync(vec, vec.size());
    if (rank == 0) {
        ASSERT_EQ(par_num, 0);
    }
}

TEST(Test_num_of_orderly_violations, Test_Vector_With_Big_Size) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> vec;
    const int vec_length = 10000;
    if (rank == 0) {
        vec = make_random_vector(vec_length);
    }
    int  par_num = numberOfOrderValids_ASync(vec, vec_length);
    if (rank == 0) {
        int seq_num = numberOfOrderValids_Sync(vec);
        ASSERT_EQ(seq_num, par_num);
    }
}

TEST(Test_num_of_orderly_violations, Test_Vector_With_Zero_Elements) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> vec;
    const int vec_length = 0;
    if (rank == 0) {
        vec = make_random_vector(vec_length);
    }
    int  par_num = numberOfOrderValids_ASync(vec, vec_length);
    if (rank == 0) {
        ASSERT_EQ(par_num, 0);
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
