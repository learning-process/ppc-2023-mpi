// Copyright 2023 Borisov Saveliy
#include <gtest/gtest.h>
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include "./orderly_violations.h"

TEST(Violations_Numbers_MPI, Test_Random_Vector) {
    boost::mpi::communicator world;
    std::vector<int> global_vector;
    const int vector_size = 120;

    if (world.rank() == 0) {
        global_vector = getRandomVector(vector_size);
    }

    int global_violations_number = getViolationsInParallel(global_vector, vector_size);

    if (world.rank() == 0) {
        int reference_violations_number = getViolationsSequentially(global_vector);
        ASSERT_EQ(reference_violations_number, global_violations_number);
    }
}

TEST(Violations_Numbers_MPI, Test_Random_Vector_With_Non_Multiple_Size) {
    boost::mpi::communicator world;
    std::vector<int> global_vector;
    const int vector_size = 123;

    if (world.rank() == 0) {
        global_vector = getRandomVector(vector_size);
    }

    int global_violations_number = getViolationsInParallel(global_vector, vector_size);

    if (world.rank() == 0) {
        int reference_violations_number = getViolationsSequentially(global_vector);
        ASSERT_EQ(reference_violations_number, global_violations_number);
    }
}

TEST(Violations_Numbers_MPI, Test_Random_Vector_With_One_Element) {
    boost::mpi::communicator world;
    std::vector<int> global_vector;
    const int vector_size = 1;

    if (world.rank() == 0) {
        global_vector = getRandomVector(vector_size);
    }

    int global_violations_number = getViolationsInParallel(global_vector, vector_size);

    if (world.rank() == 0) {
        int reference_violations_number = getViolationsSequentially(global_vector);
        ASSERT_EQ(reference_violations_number, 0);
        ASSERT_EQ(reference_violations_number, global_violations_number);
    }
}

TEST(Violations_Numbers_MPI, Test_Empty_Vector) {
    boost::mpi::communicator world;
    std::vector<int> global_vector;

    int global_violations_number = getViolationsInParallel(global_vector, 0);

    if (world.rank() == 0) {
        int reference_violations_number = getViolationsSequentially(global_vector);
        ASSERT_EQ(reference_violations_number, 0);
        ASSERT_EQ(reference_violations_number, global_violations_number);
    }
}

TEST(Violations_Numbers_MPI, Test_Manual_Filled_Vector_1) {
    boost::mpi::communicator world;
    std::vector<int> global_vector{ 5, 4, 6, 2, 8, 9, 0 };

    int global_violations_number = getViolationsInParallel(global_vector, 7);

    if (world.rank() == 0) {
        int reference_violations_number = getViolationsSequentially(global_vector);
        ASSERT_EQ(reference_violations_number, 3);
        ASSERT_EQ(reference_violations_number, global_violations_number);
    }
}

TEST(Violations_Numbers_MPI, Test_Manual_Filled_Vector_2) {
    boost::mpi::communicator world;
    std::vector<int> global_vector{ 1, 4, 6, 2, 8, 7 };

    int global_violations_number = getViolationsInParallel(global_vector, 6);

    if (world.rank() == 0) {
        int reference_violations_number = getViolationsSequentially(global_vector);
        ASSERT_EQ(reference_violations_number, 2);
        ASSERT_EQ(reference_violations_number, global_violations_number);
    }
}

TEST(Violations_Numbers_MPI, Test_Manual_Filled_Vector_With_One_Element) {
    boost::mpi::communicator world;
    std::vector<int> global_vector{ 1 };

    int global_violations_number = getViolationsInParallel(global_vector, 1);

    if (world.rank() == 0) {
        int reference_violations_number = getViolationsSequentially(global_vector);
        ASSERT_EQ(reference_violations_number, 0);
        ASSERT_EQ(reference_violations_number, global_violations_number);
    }
}

TEST(Violations_Numbers_MPI, Test_Vector_With_All_Same_Elements) {
    boost::mpi::communicator world;
    std::vector<int> global_vector{ 5, 5, 5, 5, 5, 5, 5 };

    int global_violations_number = getViolationsInParallel(global_vector, 7);

    if (world.rank() == 0) {
        int reference_violations_number = getViolationsSequentially(global_vector);
        ASSERT_EQ(reference_violations_number, 0);
        ASSERT_EQ(reference_violations_number, global_violations_number);
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
