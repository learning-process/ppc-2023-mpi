// Copyright 2023 Savotina Valeria
#include "task_3/savotina_v_configure_gradient/seq_configure_gradient.h"

double seqScalarProductVectors(const std::vector<double>& vecOne,
                               const std::vector<double>& vecTwo) {
  if (vecOne.size() != vecTwo.size() || vecOne.empty() || vecTwo.empty()) {
    throw std::invalid_argument{"invalid input argumets"};
  }

  return std::inner_product(vecOne.begin(), vecOne.end(), vecTwo.begin(), 0.0);
}

std::vector<double> seqMultMatrixOnVector(size_t n,
                                          const std::vector<double>& matr,
                                          const std::vector<double>& vec) {
  if (n == 0 || n * n != matr.size() || n != vec.size()) {
    throw std::invalid_argument{"invalid input argumets"};
  }

  std::vector<double> result(n, 0.0);

  for (int i = 0; i < n; ++i)
    for (int j = 0; j < n; ++j) result[i] += matr[i * n + j] * vec[j];

  return result;
}

std::vector<double> seqConfigureGradientMethod(size_t n,
                                               const std::vector<double>& A,
                                               const std::vector<double>& b,
                                               double tol, size_t countIter) {
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
  r0 = b - seqMultMatrixOnVector(n, A, x0);
  p0 = r0;

  for (size_t i = 0; i < countIter; ++i) {
    alpha = seqScalarProductVectors(r0, r0) /
            seqScalarProductVectors(p0, seqMultMatrixOnVector(n, A, p0));
    x1 = x0 + alpha * p0;
    r1 = b - seqMultMatrixOnVector(n, A, x1);
    err = sqrt(seqScalarProductVectors(r1, r1));

    if (err < tol) break;

    beta = seqScalarProductVectors(r1, r1) / seqScalarProductVectors(r0, r0);
    p1 = r1 + beta * p0;
    x0 = x1;
    r0 = r1;
    p0 = p1;
  }
  return x1;
}

bool isEqualVectors(const std::vector<double>& vecOne,
                    const std::vector<double>& vecTwo, double epsilon) {
  if (vecOne.size() != vecTwo.size()) return false;

  size_t size = vecOne.size();

  for (size_t i = 0; i < size; ++i) {
    if (!(abs(vecOne[i] - vecTwo[i]) <= epsilon)) return false;
  }

  return true;
}
