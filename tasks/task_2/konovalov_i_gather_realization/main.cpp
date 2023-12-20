// Copyright 2023 Konovalov Igor
#include <gtest/gtest.h>
#include <mpi.h>
#include <iostream>
#include "./gather_realization.h"

TEST(Gather_Realization, simple_int_test) {
  MPI_Comm comm = MPI_COMM_WORLD;

  int rankProc;
  int numProc;

  MPI_Comm_rank(comm, &rankProc);
  MPI_Comm_size(comm, &numProc);

  int rootProc = 0;
  int n = 10;
  int* send = new int[n];
  int* rbuf;
  int* refResult;

  for (int i = 0; i < n; i++) {
    sendArr[i] = rankProc;
  }

  if (rankProc == rootProc) {
    rbuf = static_cast<int*>(malloc(numProc * n * sizeof(int)));
    refResult = static_cast<int*>(malloc(numProc * n * sizeof(int)));
  }

  int returnCode = custom_gather(sendArr, n, MPI_INT, rbuf, n, MPI_INT, rootProc, comm);
  MPI_Gather(sendArr, n, MPI_INT, refResult, n, MPI_INT, rootProc, comm);
  if (rankProc == rootProc) {
    if (returnCode != MPI_SUCCESS)
      FAIL();
    for (int i = 0; i < numProc * n; i++) {
      EXPECT_EQ(rbuf[i], refResult[i]);
    }
    free(rbuf);
    free(refResult);
  }
}

TEST(Gather_Realization, simple_double_test) {
  MPI_Comm comm = MPI_COMM_WORLD;

  int rankProc;
  int numProc;

  MPI_Comm_rank(comm, &rankProc);
  MPI_Comm_size(comm, &numProc);

  int rootProc = 0;
  int n = 10;
  double* send = new int[n];
  double* rbuf;
  double* refResult;

  for (int i = 0; i < n; i++) {
    send[i] = std::static_cast<double>(rankProc);
  }

  if (rankProc == rootProc) {
    rbuf = static_cast<double*>(malloc(numProc * n * sizeof(double)));
    refResult = static_cast<double*>(malloc(numProc * n * sizeof(double)));
  }

  int returnCode = custom_gather(send, n, MPI_DOUBLE, rbuf, n, MPI_DOUBLE, rootProc, comm);
  MPI_Gather(send, n, MPI_DOUBLE, refResult, n, MPI_DOUBLE, rootProc, comm);
  if (rankProc == rootProc) {
    if (returnCode != MPI_SUCCESS)
      FAIL();
    for (int i = 0; i < numProc * n; i++) {
      EXPECT_EQ(rbuf[i], refResult[i]);
    }
    free(rbuf);
    free(refResult);
  }
}

TEST(Gather_Realization, simple_float_test) {
  MPI_Comm comm = MPI_COMM_WORLD;

  int rankProc;
  int numProc;

  MPI_Comm_rank(comm, &rankProc);
  MPI_Comm_size(comm, &numProc);

  int rootProc = 0;
  int n = 10;
  float* send = new int[n];
  float* rbuf;
  float* refResult;

  for (int i = 0; i < n; i++) {
    send[i] = std::static_cast<float>(rankProc);
  }

  if (rankProc == rootProc) {
    rbuf = static_cast<float*>(malloc(numProc * n * sizeof(float)));
    refResult = static_cast<float*>(malloc(numProc * n * sizeof(float)));
  }

  int returnCode = custom_gather(send, n, MPI_FLOAT, rbuf, n, MPI_FLOAT, rootProc, comm);
  MPI_Gather(send, n, MPI_FLOAT, refResult, n, MPI_FLOAT, rootProc, comm);
  if (rankProc == rootProc) {
    if (returnCode != MPI_SUCCESS)
      FAIL();
    for (int i = 0; i < numProc * n; i++) {
      EXPECT_EQ(rbuf[i], refResult[i]);
    }
    free(rbuf);
    free(refResult);
  }
}

TEST(Gather_Realization, count_error_test) {
  MPI_Comm comm = MPI_COMM_WORLD;

  int rankProc;
  int numProc;

  MPI_Comm_rank(comm, &rankProc);
  MPI_Comm_size(comm, &numProc);

  int rootProc = 0;
  int send[10];
  int* rbuf;

  for (int i = 0; i < 10; i++) {
    send[i] = rankProc;
  }

  if (rankProc == rootProc) {
    rbuf = static_cast<int*>(malloc(numProc * 10 * sizeof(int)));
  }

  int returnCode = custom_gather(send, 10, MPI_INT, rbuf, 50, MPI_INT, rootProc, comm);
  if (rankProc == rootProc) {
    EXPECT_EQ(MPI_ERR_COUNT, returnCode);
    free(rbuf);
  }
}

TEST(Gather_Realization, root_error_test) {
  MPI_Comm comm = MPI_COMM_WORLD;

  int rankProc;
  int numProc;

  MPI_Comm_rank(comm, &rankProc);
  MPI_Comm_size(comm, &numProc);

  int rootProc = -5;
  int send[10];
  int* rbuf;
  for (int i = 0; i < 10; i++) {
    send[i] = rankProc;
  }
  if (rankProc == rootProc) {
    rbuf = static_cast<int*>(malloc(numProc * 10 * sizeof(int)));
  }
  int returnCode = Gather(send, 10, MPI_INT, rbuf, 10, MPI_INT, rootProc, comm);
  if (rankProc == 0) {
    EXPECT_EQ(MPI_ERR_ROOT, returnCode);
    free(rbuf);
  }
}

int main(int argc, char** argv) {
  int result_code = 0;
  ::testing::InitGoogleTest(&argc, argv);
  ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();
  if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
    MPI_Abort(MPI_COMM_WORLD, -1);
  result_code = RUN_ALL_TESTS();
  MPI_Finalize();
  return result_code;
}
