// Copyright 2023 Loginov Maxim
#include <gtest/gtest.h>
#include <iostream>
#include "task_2/loginov_m_gather/gather.h"

TEST(Gather_Realization, Test_With_Int_Vals) {
    MPI_Comm comm = MPI_COMM_WORLD;
    int rankProc;
    MPI_Comm_rank(comm, &rankProc);
    int numProc;
    MPI_Comm_size(comm, &numProc);
    int rootProc = 0;
    int n = 1000;
    int* sendArr = new int[n];
    int* rbuf = nullptr;
    int* refResult = nullptr;
    double t1, t2, t3, t4;
    for (int i = 0; i < n; i++) {
        sendArr[i] = rankProc + i;
    }
    if (rankProc == rootProc) {
        rbuf = reinterpret_cast<int*>(malloc(numProc * n * sizeof(int)));
        refResult = reinterpret_cast<int*>(malloc(numProc * n * sizeof(int)));
        t1 = MPI_Wtime();
    }
    int returnCode = Gather(sendArr, n, MPI_INT,
      rbuf, n, MPI_INT, rootProc, comm);
    if ((rankProc == rootProc) && (returnCode == MPI_SUCCESS)) {
        t2 = MPI_Wtime();
        t3 = MPI_Wtime();
    }
    MPI_Gather(sendArr, n, MPI_INT, refResult, n, MPI_INT, rootProc, comm);
    if ((rankProc == rootProc) && (returnCode == MPI_SUCCESS)) {
        t4 = MPI_Wtime();
        std::cout << "MyGatherTime = " << t2 - t1
          << "\t" << "OriginalGatherTime = " << t4 - t3 << std::endl;
        for (int i = 0; i < numProc * n; i++) {
            EXPECT_EQ(rbuf[i], refResult[i]);
        }
        free(rbuf);
        free(refResult);
    }
}

TEST(Gather_Realization, Test_With_Double_Vals) {
    MPI_Comm comm = MPI_COMM_WORLD;
    int rankProc;
    MPI_Comm_rank(comm, &rankProc);
    int numProc;
    MPI_Comm_size(comm, &numProc);
    int rootProc = 0;
    int n = 1000;
    double* sendArr = new double[n];
    double* rbuf = nullptr;
    double* refResult = nullptr;
    double t1, t2, t3, t4;
    for (int i = 0; i < n; i++) {
        sendArr[i] = rankProc + i / 2;
    }
    if (rankProc == rootProc) {
        rbuf = reinterpret_cast<double*>(malloc(numProc * n * sizeof(double)));
        refResult = reinterpret_cast<double*>
          (malloc(numProc * n * sizeof(double)));
        t1 = MPI_Wtime();
    }
    int returnCode = Gather(sendArr, n, MPI_DOUBLE,
      rbuf, n, MPI_DOUBLE, rootProc, comm);
    if ((rankProc == rootProc)
      && (returnCode == MPI_SUCCESS)) {
        t2 = MPI_Wtime();
        t3 = MPI_Wtime();
    }
    MPI_Gather(sendArr, n, MPI_DOUBLE,
      refResult, n, MPI_DOUBLE, rootProc, comm);
    if ((rankProc == rootProc) && (returnCode == MPI_SUCCESS)) {
        t4 = MPI_Wtime();
        std::cout << "MyGatherTime = " << t2 - t1
          << "\t" << "OriginalGatherTime = " << t4 - t3 << std::endl;
        for (int i = 0; i < numProc * n; i++) {
            EXPECT_EQ(rbuf[i], refResult[i]);
        }
        free(rbuf);
        free(refResult);
    }
}

TEST(Gather_Realization, Test_With_Float_Vals) {
    MPI_Comm comm = MPI_COMM_WORLD;
    int rankProc;
    MPI_Comm_rank(comm, &rankProc);
    int numProc;
    MPI_Comm_size(comm, &numProc);
    int rootProc = 0;
    int n = 1000;
    float* sendArr = new float[n];
    float* rbuf = nullptr;
    float* refResult = nullptr;
    double t1, t2, t3, t4;
    for (int i = 0; i < n; i++) {
        sendArr[i] = rankProc + i / 2;
    }
    if (rankProc == rootProc) {
        rbuf = reinterpret_cast<float*>(malloc(numProc * n * sizeof(float)));
        refResult = reinterpret_cast<float*>
          (malloc(numProc * n * sizeof(float)));
        t1 = MPI_Wtime();
    }
    int returnCode = Gather(sendArr,
      n, MPI_FLOAT, rbuf, n, MPI_FLOAT, rootProc, comm);
    if ((rankProc == rootProc) && (returnCode == MPI_SUCCESS)) {
        t2 = MPI_Wtime();
        t3 = MPI_Wtime();
    }
    MPI_Gather(sendArr, n, MPI_FLOAT, refResult, n, MPI_FLOAT, rootProc, comm);
    if ((rankProc == rootProc) && (returnCode == MPI_SUCCESS)) {
        t4 = MPI_Wtime();
        std::cout << "MyGatherTime = " << t2 - t1
          << "\t" << "OriginalGatherTime = " << t4 - t3 << std::endl;
        for (int i = 0; i < numProc * n; i++) {
            EXPECT_EQ(rbuf[i], refResult[i]);
        }
        free(rbuf);
        free(refResult);
    }
}

TEST(Gather_Realization, Test_Count_Error) {
    MPI_Comm comm = MPI_COMM_WORLD;
    int rankProc;
    MPI_Comm_rank(comm, &rankProc);
    int numProc;
    MPI_Comm_size(comm, &numProc);
    int rootProc = 0;
    float sendArr[100];
    float* rbuf = nullptr;
    for (int i = 0; i < 100; i++) {
        sendArr[i] = rankProc + i / 2;
    }
    if (rankProc == rootProc) {
        rbuf = reinterpret_cast<float*>(malloc(numProc * 100 * sizeof(float)));
    }
    int returnCode = Gather(sendArr, 100,
      MPI_FLOAT, rbuf, 50, MPI_FLOAT, rootProc, comm);
    if (rankProc == rootProc) {
        EXPECT_EQ(MPI_ERR_COUNT, returnCode);
        free(rbuf);
    }
}
TEST(Gather_Realization, Test_Root_Error) {
    MPI_Comm comm = MPI_COMM_WORLD;
    int rankProc;
    MPI_Comm_rank(comm, &rankProc);
    int numProc;
    MPI_Comm_size(comm, &numProc);
    int rootProc = -5;
    float sendArr[100];
    float* rbuf = nullptr;
    for (int i = 0; i < 100; i++) {
        sendArr[i] = rankProc + i / 2;
    }
    if (rankProc == rootProc) {
        rbuf = reinterpret_cast<float*>(malloc(numProc * 100 * sizeof(float)));
    }
    int returnCode = Gather(sendArr,
      100, MPI_FLOAT, rbuf, 100, MPI_FLOAT, rootProc, comm);
    if (rankProc == 0) {
        EXPECT_EQ(MPI_ERR_ROOT, returnCode);
        free(rbuf);
    }
}
TEST(Gather_Realization, Test_Type_Error) {
    MPI_Comm comm = MPI_COMM_WORLD;
    int rankProc;
    MPI_Comm_rank(comm, &rankProc);
    int numProc;
    MPI_Comm_size(comm, &numProc);
    int rootProc = 0;
    float sendArr[100];
    float* rbuf = nullptr;
    for (int i = 0; i < 100; i++) {
        sendArr[i] = rankProc + i / 2;
    }
    if (rankProc == rootProc) {
        rbuf = reinterpret_cast<float*>(malloc(numProc * 100 * sizeof(float)));
    }
    int returnCode = Gather(sendArr, 100,
      MPI_CHAR, rbuf, 100, MPI_CHAR, rootProc, comm);
    if (rankProc == rootProc) {
        EXPECT_EQ(MPI_ERR_TYPE, returnCode);
        free(rbuf);
    }
}

int main(int argc, char** argv) {
    int result_code = 0;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, -1);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }
    result_code = RUN_ALL_TESTS();
    MPI_Finalize();

    return result_code;
}
