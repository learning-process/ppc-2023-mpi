// Copyright 2023 Martynov Aleksandr
#include <algorithm>
#include <random>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>
#include "../tasks/task_3/martynov_a_horizontal_linear_filtering/horizontal_linear_filtering.h"


unsigned char InputAnotherPixel(const std::vector<unsigned char>& image,
  int x, int y, int rows, int cols) {
  int sum = 0;
  unsigned char gauss[3][3]{ {1, 2, 1}, {2, 4, 2}, {1, 2, 1} };

  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++) {
      int tX = x + i - 1,
        tY = y + j - 1;

      if (tX < 0 || tX > rows - 1) tX = x;
      if (tY < 0 || tY > cols - 1) tY = y;
      if (tX * cols + tY >= cols * rows) {
        tX = x;
        tY = y;
      }
      sum += static_cast<int>(image[tX * cols + tY] * (gauss[i][j]));
    }

  return sum / 16;
}

std::vector<unsigned char> GetImage(int rows, int cols) {
  std::vector<unsigned char> temporaryIm(rows * cols);
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<>
    dist(0, std::numeric_limits<unsigned char>::max());

  for (int i = 0; i < rows; i++)
    for (int j = 0; j < cols; j++)
      temporaryIm[i * cols + j] = dist(gen);

  return temporaryIm;
}

std::vector<unsigned char> toFiltA(const std::vector<unsigned char>& image,
  int rows, int cols) {
  MPI_Status status;
  int count_of_proc, myrank;
  MPI_Comm_size(MPI_COMM_WORLD, &count_of_proc);
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  std::vector<unsigned char> result(rows * cols);

  if (rows <= count_of_proc) return toFiltB(image, rows, cols);
  int block_on_proc = rows / count_of_proc,
    remainder = rows % count_of_proc;
  std::vector<unsigned char> temporaryImage(block_on_proc * cols + 2 * cols);
  std::vector<unsigned char>Result(block_on_proc * cols);

  if (myrank == 0) {
    if (count_of_proc != 1) {
      temporaryImage.resize((block_on_proc + remainder + 1) * cols);
      for (int i = 0; i < (block_on_proc + remainder + 1) * cols; i++)
        temporaryImage[i] = image[i];
    }
    for (int i = 1; i < count_of_proc; i++) {
      int count = block_on_proc * cols + cols;
      if (i != count_of_proc - 1) count += cols;

      MPI_Send(image.data() + (i * block_on_proc * cols) +(remainder - 1) *cols,
        count, MPI_UNSIGNED_CHAR, i, 0, MPI_COMM_WORLD);
    }
  } else {
    if (myrank != count_of_proc - 1) {
      MPI_Recv(&temporaryImage[0], (block_on_proc + 2) * cols + 2,
        MPI_UNSIGNED_CHAR, 0, 0, MPI_COMM_WORLD, &status);
    } else {
      temporaryImage.resize((block_on_proc + 1) * cols);
      MPI_Recv(temporaryImage.data(),
        (block_on_proc + 1) * cols, MPI_UNSIGNED_CHAR,
        0, 0, MPI_COMM_WORLD, &status);
    }
  }

  if (myrank == count_of_proc - 1 && count_of_proc != 1) {
    for (int i = 0; i < block_on_proc; i++)
      for (int j = 0; j < cols; j++)
        Result[i * cols + j] =
        InputAnotherPixel(temporaryImage, i + 1, j, block_on_proc + 1, cols);
  } else if (myrank != 0) {
    for (int i = 1; i < block_on_proc + 1; i++)
      for (int j = 0; j < cols; j++)
        Result[(i - 1) * cols + j] =
        InputAnotherPixel(temporaryImage, i, j, block_on_proc + 2, cols);
  } else {
    if (count_of_proc != 1) {
      for (int i = 0; i < block_on_proc + remainder; i++)
        for (int j = 0; j < cols; j++)
          result[i * cols + j] =
          InputAnotherPixel(temporaryImage, i, j,
            block_on_proc + remainder + 1, cols);
    } else {
      for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
          result[i * cols + j] = InputAnotherPixel(image, i, j, rows, cols);
    }
  }

  if (myrank != 0) {
    MPI_Send(Result.data(), block_on_proc * cols, MPI_UNSIGNED_CHAR, 0,
      0, MPI_COMM_WORLD);
  } else {
    for (int i = 1; i < count_of_proc; i++)
      MPI_Recv(result.data() +
        ((block_on_proc + remainder) * cols) + ((i - 1) * block_on_proc * cols),
        block_on_proc * cols, MPI_UNSIGNED_CHAR, i, 0, MPI_COMM_WORLD, &status);
  }

  return result;
}

std::vector<unsigned char> toFiltB(const std::vector<unsigned char>& image,
  int rows, int cols) {
  std::vector<unsigned char> final_image(rows * cols);

  for (int i = 0; i < rows; i++)
    for (int j = 0; j < cols; j++)
      final_image[i * cols + j] = InputAnotherPixel(image, i, j, rows, cols);

  return final_image;
}
