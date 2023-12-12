  // Copyright 2023 Mirzakhmedov Alexander
#include <mpi.h>
#include <gtest/gtest.h>
#include <vector>
#include "./gauss_horiz.h"


TEST(Solving_Method_Gauss_Ribbon_Horizontal_Scheme, Test_matrix_100x100) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double> global_vec;
    const int count_size_vector = 100;
    std::vector<double> matr(count_size_vector * count_size_vector);
    std::vector<double> right_vect(count_size_vector);
    matr =  CreateMatrix(count_size_vector);
    right_vect = CreateVector(count_size_vector);
    std::vector<double> matr_vect = UnionMatrVect(matr, right_vect, count_size_vector);
    global_vec = GaussParallels(matr_vect, count_size_vector);
    if (rank == 0) {
        std::vector<double> res = GaussConsequent(count_size_vector);
        for (int i = 0; i < count_size_vector; i++) {
            ASSERT_NEAR(res[i], global_vec[i], 0.0000001);
        }
    }
}

TEST(Solving_Method_Gauss_Ribbon_Horizontal_Scheme, Test_matrix_6x6) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double> global_vec;
    const int count_size_vector = 6;
    std::vector<double> matr(count_size_vector * count_size_vector);
    std::vector<double> right_vect(count_size_vector);
    matr =  CreateMatrix(count_size_vector);
    right_vect = CreateVector(count_size_vector);
    std::vector<double> matr_vect = UnionMatrVect(matr, right_vect, count_size_vector);
    global_vec = GaussParallels(matr_vect, count_size_vector);
    if (rank == 0) {
        std::vector<double> res = GaussConsequent(count_size_vector);
        for (int i = 0; i < count_size_vector; i++) {
            ASSERT_NEAR(res[i], global_vec[i], 0.0000001);
        }
    }
}

TEST(Solving_Method_Gauss_Ribbon_Horizontal_Scheme, Test_matrix_3x3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double> global_vec;
    const int count_size_vector = 3;
    std::vector<double> matr(count_size_vector * count_size_vector);
    std::vector<double> right_vect(count_size_vector);
    matr =  CreateMatrix(count_size_vector);
    right_vect = CreateVector(count_size_vector);
    std::vector<double> matr_vect = UnionMatrVect(matr, right_vect, count_size_vector);
    global_vec = GaussParallels(matr_vect, count_size_vector);
    if (rank == 0) {
        std::vector<double> res = GaussConsequent(count_size_vector);
        for (int i = 0; i < count_size_vector; i++) {
            ASSERT_NEAR(res[i], global_vec[i], 0.0000001);
        }
    }
}

TEST(Solving_Method_Gauss_Ribbon_Horizontal_Scheme, Test_matrix_10x10) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double> global_vec;
    const int count_size_vector = 10;
    std::vector<double> matr(count_size_vector * count_size_vector);
    std::vector<double> right_vect(count_size_vector);
    matr =  CreateMatrix(count_size_vector);
    right_vect = CreateVector(count_size_vector);
    std::vector<double> matr_vect = UnionMatrVect(matr, right_vect, count_size_vector);
    global_vec = GaussParallels(matr_vect, count_size_vector);
    if (rank == 0) {
        std::vector<double> res = GaussConsequent(count_size_vector);
        for (int i = 0; i < count_size_vector; i++) {
            ASSERT_NEAR(res[i], global_vec[i], 0.0000001);
        }
    }
}

TEST(Solving_Method_Gauss_Ribbon_Horizontal_Scheme, Test_matrix_1x1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double> global_vec;
    const int count_size_vector = 1;
    std::vector<double> matr(count_size_vector * count_size_vector);
    std::vector<double> right_vect(count_size_vector);
    matr = CreateMatrix(count_size_vector);
    right_vect = CreateVector(count_size_vector);
    std::vector<double> matr_vect = UnionMatrVect(matr, right_vect, count_size_vector);
    global_vec = GaussParallels(matr_vect, count_size_vector);
    if (rank == 0) {
        std::vector<double> res = GaussConsequent(count_size_vector);
        for (int i = 0; i < count_size_vector; i++) {
            ASSERT_NEAR(res[i], global_vec[i], 0.0000001);
        }
    }
}

TEST(Solving_Method_Gauss_Ribbon_Horizontal_Scheme, Test_matrix_2x2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double> global_vec;
    const int count_size_vector = 2;
    std::vector<double> matr(count_size_vector * count_size_vector);
    std::vector<double> right_vect(count_size_vector);
    matr = CreateMatrix(count_size_vector);
    right_vect = CreateVector(count_size_vector);
    std::vector<double> matr_vect = UnionMatrVect(matr, right_vect, count_size_vector);
    global_vec = GaussParallels(matr_vect, count_size_vector);
    if (rank == 0) {
        std::vector<double> res = GaussConsequent(count_size_vector);
        for (int i = 0; i < count_size_vector; i++) {
            ASSERT_NEAR(res[i], global_vec[i], 0.0000001);
        }
    }
}


int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);
    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();
    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());
    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
