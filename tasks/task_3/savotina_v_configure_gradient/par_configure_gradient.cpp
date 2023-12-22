// Copyright 2023 Savotina Valeria
#include "task_3/savotina_v_configure_gradient/par_configure_gradient.h"

std::vector<double> multMatrixOnVectorChunks(size_t mMatr, size_t n,
                                             const std::vector<double>& matr,
                                             const std::vector<double>& vec) {
  std::vector<double> result(mMatr, 0.0);

  for (size_t i = 0; i < mMatr; ++i)
    for (size_t j = 0; j < n; ++j) result[i] += matr[i * n + j] * vec[j];

  return result;
}

std::vector<double> parMultMatrixOnVector(size_t n,
                                          const std::vector<double>& matr,
                                          const std::vector<double>& vec,
                                          MPI_Comm comm) {
  if (n == 0 || n * n != matr.size() || n != vec.size()) {
    throw std::invalid_argument{"invalid input argumets"};
  }

  int rank = 0;
  int sizeWorld = 0;

  MPI_Comm_rank(comm, &rank);
  MPI_Comm_size(comm, &sizeWorld);

  int chunk = n / sizeWorld;
  int tail = n % sizeWorld;

  std::vector<int> sendCountsMatr(sizeWorld, chunk * n);
  std::vector<int> displsMatr(sizeWorld, 0);

  std::vector<double> globalResult(n, 0.0);
  std::vector<int> recvCountsRes(sizeWorld, chunk);
  std::vector<int> displsRes(sizeWorld, 0);

  for (int i = 0; i < sizeWorld; ++i) {
    if (i < tail) {
      sendCountsMatr[i] += n;
      ++recvCountsRes[i];
    }
    displsMatr[i] = i == 0 ? 0 : displsMatr[i - 1] + sendCountsMatr[i - 1];
    displsRes[i] = i == 0 ? 0 : displsRes[i - 1] + recvCountsRes[i - 1];
  }

  std::vector<double> localMatr(sendCountsMatr[rank]);
  MPI_Scatterv(matr.data(), sendCountsMatr.data(), displsMatr.data(),
               MPI_DOUBLE, localMatr.data(), sendCountsMatr[rank], MPI_DOUBLE,
               0, comm);

  auto localResult =
      multMatrixOnVectorChunks(sendCountsMatr[rank] / n, n, localMatr, vec);
  MPI_Allgatherv(localResult.data(), localResult.size(), MPI_DOUBLE,
                 globalResult.data(), recvCountsRes.data(), displsRes.data(),
                 MPI_DOUBLE, comm);

  return globalResult;
}

double parScalarProductVectors(const std::vector<double>& vecOne,
                               const std::vector<double>& vecTwo,
                               MPI_Comm comm) {
  if (vecOne.size() != vecTwo.size() || vecOne.empty() || vecTwo.empty()) {
    throw std::invalid_argument{"invalid input argumets"};
  }

  int rank = 0;
  int sizeWorld = 0;
  size_t size = vecOne.size();

  MPI_Comm_rank(comm, &rank);
  MPI_Comm_size(comm, &sizeWorld);

  int chunk = size / sizeWorld;
  int tail = size % sizeWorld;

  std::vector<int> sendCounts(sizeWorld, chunk);
  std::vector<int> displs(sizeWorld, 0);

  for (int i = 0; i < sizeWorld; ++i) {
    if (i < tail) ++sendCounts[i];
    displs[i] = i == 0 ? 0 : displs[i - 1] + sendCounts[i - 1];
  }

  std::vector<double> localVecOne(sendCounts[rank]);
  std::vector<double> localVecTwo(sendCounts[rank]);

  MPI_Scatterv(vecOne.data(), sendCounts.data(), displs.data(), MPI_DOUBLE,
               localVecOne.data(), sendCounts[rank], MPI_DOUBLE, 0, comm);
  MPI_Scatterv(vecTwo.data(), sendCounts.data(), displs.data(), MPI_DOUBLE,
               localVecTwo.data(), sendCounts[rank], MPI_DOUBLE, 0, comm);

  double localSum = std::inner_product(localVecOne.begin(), localVecOne.end(),
                                       localVecTwo.begin(), 0.0);
  double globalSum = 0.0;

  MPI_Allreduce(&localSum, &globalSum, 1, MPI_DOUBLE, MPI_SUM, comm);

  return globalSum;
}

std::vector<double> parConfigureGradientMethod(size_t n,
                                               const std::vector<double>& A,
                                               const std::vector<double>& b,
                                               MPI_Comm comm, double tol,
                                               size_t countIter) {
  if (n == 0 || n * n != A.size() || n != b.size() || tol == 0.0 ||
      countIter == 0) {
    throw std::invalid_argument{"invalid input argumets"};
  }

  std::vector<double> r0(n, 0.0);
  std::vector<double> r1(n, 0.0);

  std::vector<double> p0(n, 0.0);
  std::vector<double> p1(n, 0.0);

  std::vector<double> x1(n, 0.0);
  std::vector<double> x0(n, 0.0);

  double alpha = 0.0;
  double beta = 0.0;
  double err = 0.0;

  x1 = x0;
  r0 = b - parMultMatrixOnVector(n, A, x0, comm);
  p0 = r0;

  for (size_t i = 0; i < countIter; ++i) {
    alpha = parScalarProductVectors(r0, r0, comm) /
            parScalarProductVectors(p0, parMultMatrixOnVector(n, A, p0, comm),
                                    comm);
    x1 = x0 + alpha * p0;
    r1 = b - parMultMatrixOnVector(n, A, x1, comm);
    err = sqrt(parScalarProductVectors(r1, r1, comm));

    if (err < tol) break;

    beta = parScalarProductVectors(r1, r1, comm) /
           parScalarProductVectors(r0, r0, comm);
    p1 = r1 + beta * p0;
    x0 = x1;
    r0 = r1;
    p0 = p1;
  }
  return x1;
}
