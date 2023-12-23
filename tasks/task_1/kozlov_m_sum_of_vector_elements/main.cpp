// Copyright 2023 Kozlov Mikhail
#include <gtest/gtest.h>
#include <vector>
#include "./sum_of_vector_elements.h"
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>

TEST(Parallel_Sum_Vec_Of_Elem, Test_Sum_1) {
    int ProcNum, ProcRank;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    std::vector<int> test_vec{1, 10, 132, 133, 145, 1678, 189, 190, 91, 19};

    int par_sum = sum_of_vec_elem_par(test_vec);

    if (ProcRank == 0) {
        int seq_sum = sum_of_vec_elem_seq(test_vec);
        ASSERT_EQ(par_sum, seq_sum);
    }
}

TEST(Parallel_Sum_Vec_Of_Elem, Test_Sum_0) {
    int ProcNum, ProcRank;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    std::vector<int> test_vec{0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    int par_sum = sum_of_vec_elem_par(test_vec);

    if (ProcRank == 0) {
        int seq_sum = sum_of_vec_elem_seq(test_vec);
        ASSERT_EQ(par_sum, seq_sum);
    }
}

TEST(Parallel_Sum_Vec_Of_Elem, Test_Empty_Vec) {
    int ProcNum, ProcRank;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    std::vector<int> test_vec{};

    int par_sum = sum_of_vec_elem_par(test_vec);

    if (ProcRank == 0) {
        int seq_sum = sum_of_vec_elem_seq(test_vec);
        ASSERT_EQ(par_sum, seq_sum);
    }
}

TEST(Parallel_Sum_Vec_Of_Elem, Test_Random_Elements_1) {
    int ProcNum, ProcRank;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    std::vector<int> test_vec;
    test_vec = get_random_vector(100);

    int par_sum = sum_of_vec_elem_par(test_vec);

    if (ProcRank == 0) {
        int seq_sum = sum_of_vec_elem_seq(test_vec);
        ASSERT_EQ(par_sum, seq_sum);
    }
}

TEST(Parallel_Sum_Vec_Of_Elem, Test_Random_Elements_2) {
    int ProcNum, ProcRank;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    std::vector<int> test_vec;
    test_vec = get_random_vector(1000);

    int par_sum = sum_of_vec_elem_par(test_vec);

    if (ProcRank == 0) {
        int seq_sum = sum_of_vec_elem_seq(test_vec);
        ASSERT_EQ(par_sum, seq_sum);
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
