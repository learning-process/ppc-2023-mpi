// Copyright 2023 Novostroev Ivan
#include <gtest/gtest.h>
#include "task_3/novostroev_i_matrix_mult_strassen/strassen.h"

void eq(const std::vector<double>& vec1, const std::vector<double>& vec2) {
    if (vec1.size() != vec2.size()) throw "non equal dimensions";
    for (std::size_t i = 0; i < vec1.size(); i++)
        EXPECT_NEAR(vec1[i], vec2[i], 0.0001);
}

TEST(TestsStrassenMPI, Test1on1MPI) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n = 1;
    std::vector<double> A = {2.0};
    std::vector<double> B = {3.0};

    std::vector<double> C = {6.0};
    std::vector<double> StrassenMPI;

    if (rank == 0) {
        StrassenMPI = strassenMPI(A, B, n, rank, size);
    }

    if (rank == 0) {
        eq(C, StrassenMPI);
    }
}

TEST(TestsStrassenMPI, Test2on2MPI) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n = 2;
    std::vector<double> A = {1.0, 2.0, 3.0, 4.0};
    std::vector<double> B = {5.0, 6.0, 7.0, 8.0};

    std::vector<double> C = {19.0, 22.0, 43.0, 50.0};
    std::vector<double> StrassenMPI;

    if (rank == 0) {
        StrassenMPI = strassenMPI(A, B, n, rank, size);
    }

    if (rank == 0) {
        eq(C, StrassenMPI);
    }
}

TEST(TestsStrassenMPI, Test4on4MPI) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n = 4;
    std::vector<double> A = genVec(n * n);
    std::vector<double> B = genVec(n * n);

    std::vector<double> C = mult(A, B, n);
    std::vector<double> StrassenMPI;

    if (rank == 0) {
        StrassenMPI = strassenMPI(A, B, n, rank, size);
    }

    if (rank == 0) {
        eq(C, StrassenMPI);
    }
}

TEST(TestsStrassenMPI, Test8on8MPI) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n = 8;
    std::vector<double> A = genVec(n * n);
    std::vector<double> B = genVec(n * n);

    std::vector<double> C = mult(A, B, n);
    std::vector<double> StrassenMPI;

    if (rank == 0) {
        StrassenMPI = strassenMPI(A, B, n, rank, size);
    }

    if (rank == 0) {
        eq(C, StrassenMPI);
    }
}

TEST(TestsStrassenMPI, Test16on16MPI) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n = 16;
    std::vector<double> A = genVec(n * n);
    std::vector<double> B = genVec(n * n);

    std::vector<double> C = mult(A, B, n);
    std::vector<double> StrassenMPI;

    if (rank == 0) {
        StrassenMPI = strassenMPI(A, B, n, rank, size);
    }

    if (rank == 0) {
        eq(C, StrassenMPI);
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
