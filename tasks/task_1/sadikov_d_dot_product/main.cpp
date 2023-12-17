// Copyright 2023 Sadikov Damir
#include <gtest/gtest.h>
#include <vector>
#include "./dot_product.h"
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>

TEST(Parallel_Dot_Product_MPI, Test_Random) {
    boost::mpi::communicator world;
    std::vector<int> A, B;
    const int vector_size = 120;

    if (world.rank() == 0) {
        A = getRandomVector(vector_size);
        B = getRandomVector(vector_size);
    }

    int global_sum = getParallelDotProduct(A, B, vector_size);

    if (world.rank() == 0) {
        int reference_sum = getSequentialDotProduct(A, B);
        ASSERT_EQ(reference_sum, global_sum);
    }
}

TEST(Parallel_Dot_Product_MPI, Test_Random_Small_Vector) {
    boost::mpi::communicator world;
    std::vector<int> A, B;
    const int vector_size = 3;

    if (world.rank() == 0) {
        A = getRandomVector(vector_size);
        B = getRandomVector(vector_size);
    }

    int global_sum = getParallelDotProduct(A, B, vector_size);

    if (world.rank() == 0) {
        int reference_sum = getSequentialDotProduct(A, B);
        ASSERT_EQ(reference_sum, global_sum);
    }
}

TEST(Parallel_Dot_Product_MPI, Test_Random_Empty_Vector) {
    boost::mpi::communicator world;
    std::vector<int> A, B;
    const int vector_size = 0;

    if (world.rank() == 0) {
        A = getRandomVector(vector_size);
        B = getRandomVector(vector_size);
    }

    int global_sum = getParallelDotProduct(A, B, vector_size);

    if (world.rank() == 0) {
        int reference_sum = getSequentialDotProduct(A, B);
        ASSERT_EQ(reference_sum, global_sum);
    }
}

TEST(Parallel_Dot_Product_MPI, Test_Random_Bigger_Vector) {
    boost::mpi::communicator world;
    std::vector<int> A, B;
    const int vector_size = 10'007;

    if (world.rank() == 0) {
        A = getRandomVector(vector_size);
        B = getRandomVector(vector_size);
    }

    int global_sum = getParallelDotProduct(A, B, vector_size);

    if (world.rank() == 0) {
        int reference_sum = getSequentialDotProduct(A, B);
        ASSERT_EQ(reference_sum, global_sum);
    }
}

TEST(Parallel_Dot_Product_MPI, Test_Specified_Vector) {
    boost::mpi::communicator world;
    std::vector<int> A;
    std::vector<int> B;
    int vector_size = 5;

    if (world.rank() == 0) {
        A = { -30, 83, -90, -16, 12 };
        B = { -59, 30, 25, -84, -84 };
    }

    int global_sum = getParallelDotProduct(A, B, vector_size);

    if (world.rank() == 0) {
        int reference_sum = 2346;
        ASSERT_EQ(reference_sum, global_sum);
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
