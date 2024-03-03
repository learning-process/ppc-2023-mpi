// Copyright 2024 Efimov Andrey
#include "task_2/efimov_a_picture_smoothing/picture_smoothing.h"
#include <mpi.h>
#include <vector>
#include <random>


std::vector<int> get_image(const int rows, const int cols) {
  if (rows < 0 || cols < 0)
    throw - 1;
  std::mt19937 gen;
  gen.seed(static_cast<unsigned int>(time(0)));
  std::vector<int> Matrix(rows * cols);
  for (int i = 0; i < rows * cols; i++)
    Matrix[i] = gen() % 256;
  return Matrix;
}

std::vector<int> seq_smoothing(const std::vector<int>& img,
int rows, int cols, int correct) {
  std::vector <int> smoothedResult(rows * cols);
  int mid = 0;
  for (int i = 0; i < rows * cols; i++) {
    mid += img[i];
  }
  mid = mid / (rows * cols);
  float correction = 1.0 + static_cast<float>(correct) / 100;
  std::vector <int> palette(256);
  for (int i = 0; i < 256; i++) {
    int delta = i - mid;
    palette[i] = mid - correction * delta;
    if (palette[i] > 256) {
      palette[i] = 256;
    }
    if (palette[i] < 0) {
      palette[i] = 0;
    }
  }
  for (int i = 0; i < rows * cols; i++) {
    smoothedResult[i] = palette[img[i]];
  }
  return smoothedResult;
}

std::vector<int> par_smoothing(const std::vector<int>& img,
int rows, int cols, int correct) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int det = rows / size;
  int rem = rows % size;

  std::vector<int> smoothedResult(rows * cols);
  std::vector<int> Data(det * cols);

  int mid = 0;
  int middle = 0;
  if (rank == 0) {
    if (det > 0) {
      for (int proc = 1; proc < size; proc++) {
        MPI_Send(&img[(det * proc + rem) * cols], det * cols,
        MPI_INT, proc, 1, MPI_COMM_WORLD);
      }
    }
  } else if (det > 0) {
    MPI_Status status;
    MPI_Recv(&Data[0], det * cols, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
  }

  if (rank == 0) {
    MPI_Status status;
    Data = std::vector<int>(img.begin(), img.begin() + (det + rem) * cols);
    for (int i = 0; i < (det + rem) * cols; i++) {
      mid += Data[i];
    }
    if (det > 0) {
      for (int proc = 1; proc < size; proc++) {
        MPI_Recv(&middle, 1, MPI_INT, proc, 0, MPI_COMM_WORLD, &status);
        mid += middle;
      }
    }
    mid = mid / (cols * rows);
  } else if (det > 0) {
    for (int i = 0; i < det * cols; i++) {
      mid += Data[i];
    }
    MPI_Send(&mid, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
  }

  MPI_Bcast(&mid, 1, MPI_INT, 0, MPI_COMM_WORLD);
  int det_ = 256 / size;
  int rem_ = 256 % size;
  std::vector <int> palette(256);
  float correction = 1.0 + static_cast<float>(correct) / 100;
  std::vector <int> tempA(det_);
  if (rank == 0) {
    MPI_Status status;
    for (int i = 0; i < det_ + rem_; i++) {
      int delta = i - mid;
      palette[i] = mid - correction * delta;
      if (palette[i] > 256) {
        palette[i] = 256;
      } else if (palette[i] < 0) {
        palette[i] = 0;
      }
    }
    if (det_ > 0) {
      for (int proc = 1; proc < size; proc++) {
        MPI_Recv(&palette[rem_ + det_ * proc], det_,
        MPI_INT, proc, 0, MPI_COMM_WORLD, &status);
      }
    }
  } else if (det_ > 0) {
    for (int i = 0; i < det_; i++) {
      int delta = i + rem_ + det_ * rank - mid;
      tempA[i] = mid - correction * delta;
      if (tempA[i] > 256) {
        tempA[i] = 256;
      }
      if (tempA[i] < 0) {
        tempA[i] = 0;
      }
    }
    MPI_Send(&tempA[0], det_, MPI_INT, 0, 0, MPI_COMM_WORLD);
  }
  MPI_Bcast(&palette[0], 256, MPI_INT, 0, MPI_COMM_WORLD);
  std::vector <int> tempB(det * cols);
  if (rank == 0) {
    MPI_Status status;
    for (int i = 0; i < (det + rem) * cols; i++) {
      smoothedResult[i] = palette[img[i]];
    }
    if (det > 0) {
      for (int proc = 1; proc < size; proc++) {
        MPI_Recv(&smoothedResult[(det * proc + rem) * cols], det *
        cols, MPI_INT, proc, 0, MPI_COMM_WORLD, &status);
      }
    }
  } else if (det > 0) {
    for (int i = 0; i < det * cols; i++) {
      tempB[i] = palette[Data[i]];
    }
    MPI_Send(&tempB[0], det * cols, MPI_INT, 0, 0, MPI_COMM_WORLD);
  }
  return smoothedResult;
}
