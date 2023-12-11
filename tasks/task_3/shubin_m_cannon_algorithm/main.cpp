// Copyright 2023 Shubin Mikhail
#include <gtest/gtest.h>
#include <random>
#include "./cannon_algorithm.h"

TEST(Cannon_Algorithm, Test_identity_mat_mult_normal_mat) {
  int ProcRank, ProcNum;
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  int N = 7;
  std::vector<double> A(N * N);
  std::vector<double> B(N * N);
  std::vector<double> C(N * N);

  if (ProcRank == 0) {
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        A[i*N+j] = i * j;
        if (i == j) {
          B[i*N+j] = 1;
        } else {
          B[i*N+j] = 0;
        }
      }
    }
  }

  mat_mult_cannon(N, A, B, &C);
  if (ProcRank == 0) {
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        C[i*N+j] = round_double(C[i*N+j], 6);
      }
    }
  }

  if (ProcRank == 0) {
    ASSERT_EQ(A, C);
  }
}

TEST(Cannon_Algorithm, Test_zero_mat_mult_normal_mat) {
  int ProcRank, ProcNum;
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  int N = 7;
  std::vector<double> A(N * N);
  std::vector<double> B(N * N);
  std::vector<double> C(N * N);

  if (ProcRank == 0) {
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        A[i*N+j] = i * j;
        B[i*N+j] = 0;
      }
    }
  }

  mat_mult_cannon(N, A, B, &C);
  if (ProcRank == 0) {
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        C[i*N+j] = round_double(C[i*N+j], 6);
      }
    }
  }

  if (ProcRank == 0) {
    ASSERT_EQ(B, C);
  }
}


TEST(Cannon_Algorithm, Test_mat_mult_on_itself) {
  int ProcRank, ProcNum;
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  int N = 7;
  std::vector<double> A(N * N);
  std::vector<double> B(N * N);
  std::vector<double> C_cons(N * N);
  std::vector<double> C_parr(N * N);

  if (ProcRank == 0) {
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        A[i*N+j] = i * j;
        B[i*N+j] = i * j;
      }
    }
  }

  mat_mult_cons(N, A, B, &C_cons);
  mat_mult_cannon(N, A, B, &C_parr);
  if (ProcRank == 0) {
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        C_cons[i*N+j] = round_double(C_cons[i*N+j], 6);
        C_parr[i*N+j] = round_double(C_parr[i*N+j], 6);
      }
    }
  }

  if (ProcRank == 0) {
    ASSERT_EQ(C_cons, C_parr);
  }
}

TEST(Cannon_Algorithm, Test_positive_mats_mult) {
  std::random_device dev;
  std::mt19937 gen(dev());
  std::uniform_real_distribution<> distrib(0.0, 100.0);

  int ProcRank, ProcNum;
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  int N = 7;
  std::vector<double> A(N * N);
  std::vector<double> B(N * N);
  std::vector<double> C_cons(N * N);
  std::vector<double> C_parr(N * N);

  if (ProcRank == 0) {
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        A[i*N+j] = distrib(gen);
        B[i*N+j] = distrib(gen);
      }
    }
  }

  mat_mult_cons(N, A, B, &C_cons);
  mat_mult_cannon(N, A, B, &C_parr);
  if (ProcRank == 0) {
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        C_cons[i*N+j] = round_double(C_cons[i*N+j], 6);
        C_parr[i*N+j] = round_double(C_parr[i*N+j], 6);
      }
    }
  }

  if (ProcRank == 0) {
    ASSERT_EQ(C_cons, C_parr);
  }
}

TEST(Cannon_Algorithm, Test_rand_mats_mult) {
  std::random_device dev;
  std::mt19937 gen(dev());
  std::uniform_real_distribution<> distrib(-100.0, 100.0);

  int ProcRank, ProcNum;
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  int N = 7;
  std::vector<double> A(N * N);
  std::vector<double> B(N * N);
  std::vector<double> C_cons(N * N);
  std::vector<double> C_parr(N * N);

  if (ProcRank == 0) {
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        A[i*N+j] = distrib(gen);
        B[i*N+j] = distrib(gen);
      }
    }
  }

  mat_mult_cons(N, A, B, &C_cons);
  mat_mult_cannon(N, A, B, &C_parr);
  if (ProcRank == 0) {
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        C_cons[i*N+j] = round_double(C_cons[i*N+j], 6);
        C_parr[i*N+j] = round_double(C_parr[i*N+j], 6);
      }
    }
  }

  if (ProcRank == 0) {
    ASSERT_EQ(C_cons, C_parr);
  }
}

int main(int argc, char** argv) {
    int resultCode = 0;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, -1);
    resultCode = RUN_ALL_TESTS();
    MPI_Finalize();

    return resultCode;
}