// Copyright 2023 Kazantsev Evgeny
#include <vector>
#include <cmath>
#include <limits>
#include <string>
#include <random>
#include <iostream>
#include <algorithm>
#include <functional>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>
#include "../tasks/task_2/kazantsev_e_mul_tape_matrix/mul_tape_matrix.h"

std::vector<int> GetMatrixRand(int n) {
  std::vector<int> matrix(n * n);
  std::random_device d;
  std::mt19937 gen(d());
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      matrix[i * n + j] = gen() % 5;
    }
  }
  return matrix;
}
std::vector<int> SequentialMulMatrix(const std::vector<int>& a,
  const std::vector<int>& b, int n, int m, int m1) {
  int n1 = m;
  std::vector<int> c(n * m1, 0);
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m1; ++j) {
      for (int k = 0; k < m; ++k) {
        c[i * m1 + j] += a[i * n1 + k] * b[k * m1 + j];
      }
    }
  }
  return c;
}
  std::vector<int> parallelMatrixMul(std::vector<int>* c,
  std::vector<int>* d, int an, int am) {
  std::vector<int>& a = *c;
  std::vector<int>& b = *d;
  int comSize;
  int comRank;
  MPI_Comm_size(MPI_COMM_WORLD, &comSize);
  MPI_Comm_rank(MPI_COMM_WORLD, &comRank);

  if (an < comSize || comSize == 1) {
    if (comRank == 0) {
      return SequentialMulMatrix(a, b, an, am, an);
    } else {
        return std::vector<int>{};
      }
  }
  int bn = am;
  int bm = an;
  int n = an / comSize;
  int reminder = an % comSize;
  MPI_Bcast(b.data(), bn * bm, MPI_INT, 0, MPI_COMM_WORLD);
    std::vector<int> loc_v;
  if (comRank == 0) {
    if (n > 0) {
      for (int process = 1; process < comSize; ++process) {
        MPI_Send(a.data() + reminder * am + process * n * am,
          am * n, MPI_INT, process, 0, MPI_COMM_WORLD);
      }
    }
  }

  if (comRank == 0) {
    loc_v = std::vector<int>(a.begin(), a.begin() + n * am + reminder * am);
  } else {
      if (n > 0) {
        loc_v.resize(am * n);
        MPI_Status status;
        MPI_Recv(loc_v.data(), am * n, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
      }
    }

  std::vector<int> loc_res = SequentialMulMatrix(loc_v, b, loc_v.size() / am, am, bm);
  std::vector<int> res(an * bm);
  std::vector<int> sendcounts(comSize);
  std::vector<int> displs(comSize);
  for (int i = 0; i < comSize; ++i) {
    sendcounts[i] = (i == 0) ? n * am + reminder * am : n * am;
    displs[i] = (i == 0) ? 0 : (displs[i - 1] + sendcounts[i - 1]);
  }
  MPI_Gatherv(loc_res.data(), sendcounts[comRank], MPI_INT, res.data(),
    sendcounts.data(), displs.data(), MPI_INT, 0, MPI_COMM_WORLD);
  if (comRank == 0) {
    return res;
  } else {
      return std::vector<int> {};
    }
}
