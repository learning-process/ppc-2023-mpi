// Copyright 2023 Durandin Vladimir

#include "task_3/durandin_v_Strongin_global_search/Strongin_global_search.h"


double f(double x) { return x * x; }

double global_search::seq_Strongin_alghorithm(double x0, double x1,
                                              double eps) {
  std::vector<double> x;
  std::vector<double> y;
  double lipshM = 0.0;
  double lipshm;
  double R = 0.0;
  int interval = 0;

  if (x1 > x0) {
    x.push_back(x0);
    x.push_back(x1);
  } else {
    x.push_back(x1);
    x.push_back(x0);
  }

  while (true) {
    for (size_t i = 0ull; i < static_cast<int>(x.size()); i++) {
      y.push_back(f(x[i]));
    }
    for (size_t i = 0ull; i < static_cast<int>(x.size()) - 1ull; i++) {
      double lipsh = std::abs((y[i + 1ull] - y[i]) / (x[i + 1ull] - x[i]));
      if (lipsh > lipshM) {
        lipshM = lipsh;
        lipshm = lipsh + lipsh;
        double tempR =
            lipshm * (x[i + 1] - x[i]) +
            pow((y[i + 1] - y[i]), 2) / (lipshm * (x[i + 1] - x[i])) -
            2 * (y[i + 1] + y[i]);
        if (tempR > R) {
          R = tempR;
          interval = i;
        }
      }
    }
    if (x[interval + 1] - x[interval] <= eps) {
      return y[interval + 1];
    }
    double newX = 0.0;
    newX = (x[interval + 1] - x[interval]) / 2 + x[interval] +
           (y[interval + 1] - y[interval]) / (2 * lipshm);
    x.push_back(newX);
    sort(x.begin(), x.end());
    y.clear();
  }
}

double global_search::paral_Strongin_algorithm(double x0, double x1,
                                               double eps) {
  int size, rank;
  MPI_Status status;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    std::vector<double> x;
    x.push_back(x0);
    x.push_back(x1);

    while (true) {
      sort(x.begin(), x.end());

      int part = static_cast<int>(x.size() - 1) / size;
      int remain = static_cast<int>(x.size() - 1) % size;

      for (int i = 1; i < size; ++i) {
        MPI_Send(x.data() + remain + i * part, 1, MPI_DOUBLE, i, 0,
                 MPI_COMM_WORLD);
      }

      double lipshM = 0, lipshm, lipsh;
      for (int i = 0; i < part + remain; ++i) {
        lipsh = std::abs((f(x[i + 1]) - f(x[i])) / (x[i + 1] - x[i]));
        if (lipsh > lipshM) {
          lipshM = lipsh;
          lipshm = 2 * lipsh;
        }
      }
      if (part > 0) {
        for (int i = 1; i < size; i++) {
          MPI_Recv(&lipsh, 1, MPI_DOUBLE, i, MPI_ANY_TAG, MPI_COMM_WORLD,
                   &status);
          if (lipsh > lipshM) {
            lipshM = lipsh;
            lipshm = 2 * lipsh;
          }
        }
      }

      int interval = 0;
      double tempR;
      double R = 0.0;
      for (int i = 0; i < static_cast<int>(x.size()) - 1; i++) {
        tempR = lipshm * (x[i + 1] - x[i]) +
                pow((f(x[i + 1]) - f(x[i])), 2) / (lipshm * (x[i + 1] - x[i])) -
                2 * (f(x[i + 1]) + f(x[i]));
        if (tempR > R) {
          R = tempR;
          interval = i;
        }
      }

      if (x[interval + 1] - x[interval] <= eps) {
        for (int i = 1; i < size; ++i)
          MPI_Send(x.data(), 1, MPI_DOUBLE, i, 1, MPI_COMM_WORLD);
        return x[interval + 1];
      }

      double newX = (x[interval] + x[interval + 1]) / 2 -
                    (f(x[interval + 1]) - f(x[interval])) / (lipshm + lipshm);
      x.push_back(newX);
    }
  } else {
    int part = 0;
    while (true) {
      MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      MPI_Get_count(&status, MPI_DOUBLE, &part);

      std::vector<double> x(part + 1);
      MPI_Recv(x.data(), 1, MPI_DOUBLE, 0, MPI_ANY_TAG, MPI_COMM_WORLD,
               &status);

      if (status.MPI_TAG == 1)
        return 0;

      double lipshM = 0, lipshm = 1.0, lipsh;
      if (part != 0) {
        for (int i = 0; i < static_cast<int>(x.size()) - 1; ++i) {
          lipsh = (std::abs(f(x[i + 1]) - f(x[i]))) / (x[i + 1] - x[i]);
          if (lipsh > lipshM) {
            lipshM = lipsh;
            lipshm = lipshM + lipshM;
          }
        }
      }
      MPI_Send(&lipshm, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }
    return 0;
  }
}
