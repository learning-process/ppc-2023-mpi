// Copyright 2023 Kasimtcev Roman
#include "task_3/kasimtcev_block_linear_filtering/block_partit.h"
#include <mpi.h>
#include <ctime>
#include <iostream>
#include <random>

std::vector <double> randomImage(int row, int col) {
  std::mt19937 gen;
  gen.seed(static_cast<unsigned int>(time(0)));
  std::vector<double> image(col * row);
  for (int i = 0; i < col * row; i++) {
    int color = gen() % 256;
    image[i] = color / 255.0;
  }
  return image;
}

std::vector<double> consistentOparation(std::vector<double> image, int row,
                                            int col) {
  std::vector<double> res(row * col);

  for (int i = 0; i < row; i++)
    for (int j = 0; j < col; j++) {
      double color = 0;
      for (int k = -r; k < r; k++)
        for (int h = -r; h < r; h++) {
          int i1 = i + h, j1 = j + k;
          if (i1 < 0 || j1 > col - 1 || j1 < 0 ||
              j1 > row - 1) {
            color += 0;
          } else {
            color += image[j1 * col + i1] *
                     gaussianKernel[(k + 1) * sizeG + (h + 1)];
          }
        }
      res[i * col + j] = std::max(0.0, std::min(color / norma, 1.0));
    }
  return res;
}

std::vector<double> parllOp(std::vector<double> global_image,
                                          int row, int col) {
  int size, rank;

  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int error_code = 0;
  if (rank == 0 && global_image.size() == 0) {
    error_code = 1;
  } else if (rank == 0 &&
             static_cast<int>(global_image.size()) != row * col) {
    error_code = 2;
  }
  MPI_Bcast(&error_code, 1, MPI_INT, 0, MPI_COMM_WORLD);
  if (error_code == 1) {
    throw std::runtime_error("Image is empty");
  } else if (error_code == 2) {
    throw std::runtime_error("Invalid parameters");
  }

  if (size == 1) {
    return consistentOparation(global_image, row, col);
  }

  const int del = (row / size) * col;
  const int eps = (row % size) * col;

  int sizeOfData;
  if (rank == 0) {
    sizeOfData = del + eps + col;
  } else if (rank == size - 1) {
    sizeOfData = del + col;
  } else {
    sizeOfData = del + 2 * col;
  }

  std::vector<double> localimage(sizeOfData);

  if (rank == 0) {
    std::memcpy(localimage.data(), global_image.data(),
                (del + eps + col) * sizeof(double));
    for (int proc = 1; proc < size - 1; proc++) {
      MPI_Send(global_image.data() + eps + proc * del - col,
               del + 2 * col, MPI_DOUBLE, proc, 0, MPI_COMM_WORLD);
    }
    MPI_Send(global_image.data() + eps + (size - 1) * del - col,
             del + col, MPI_DOUBLE, size - 1, 0, MPI_COMM_WORLD);
  } else if (rank == size - 1) {
    MPI_Status status;
    MPI_Recv(localimage.data(), del + col, MPI_DOUBLE, 0, 0,
             MPI_COMM_WORLD, &status);
  } else {
    MPI_Status status;
    MPI_Recv(localimage.data(), del + 2 * col, MPI_DOUBLE, 0, 0,
             MPI_COMM_WORLD, &status);
  }

  std::vector<double> local_res = consistentOparation(
      localimage, rank == 0 ? (del + eps) / col : del / col + 1,
      col);
  std::vector<double> res(row * col);

  if (rank == 0) {
    std::memcpy(res.data(), local_res.data(),
                (del + eps) * sizeof(double));
    for (int proc = 1; proc < size; proc++) {
      MPI_Status status;
      MPI_Recv(res.data() + proc * del + eps, del, MPI_DOUBLE, proc,
               0, MPI_COMM_WORLD, &status);
    }
  } else {
    MPI_Send(local_res.data() + col, del, MPI_DOUBLE, 0, 0,
             MPI_COMM_WORLD);
  }

  return res;
}
