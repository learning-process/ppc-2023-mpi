// Copyright 2023 Vyunov Danila

#include "../tasks/task_3/vyunov_d_sobel/sobel.h"

#include <mpi.h>

#include <ctime>
#include <random>

int* getRandomImage(int width, int height) {
  std::mt19937 gen(time(0));
  std::uniform_int_distribution<> uid(0, 255);
  int* img = new int[width * height];
  for (int i = 0; i < width * height; i++) {
    img[i] = uid(gen);
  }
  return img;
}

int clamp(int value, int max, int min) {
  if (value > max) return max;
  if (value < min) return min;
  return value;
}

float calcNewPixelColorX(int* img, int x, int y, int width, int height) {
  float returnC = 0;

  for (int i = -1; i <= 1; i++) {
    for (int j = -1; j <= 1; j++) {
      int idx = (i + 1) * 3 + j + 1;
      int pixel = img[x + j + (y + i) * width];
      returnC += pixel * kernelX[idx];
    }
  }
  return returnC;
}

float calcNewPixelColorY(int* img, int x, int y, int width, int height) {
  float returnC = 0;

  for (int i = -1; i <= 1; i++) {
    for (int j = -1; j <= 1; j++) {
      int idx = (i + 1) * 3 + j + 1;
      int pixel = img[x + j + (y + i) * width];
      returnC += pixel * kernelY[idx];
    }
  }
  return returnC;
}

int* getSequentialSobel(int* image, int width, int height) {
  int* new_image = new int[(width - 2) * (height - 2)];

  for (int i = 1; i < height - 1; i++) {
    for (int j = 1; j < width - 1; j++) {
      int pixel_x = calcNewPixelColorX(image, j, i, width, height);
      int pixel_y = calcNewPixelColorX(image, j, i, width, height);
      new_image[j - 1 + (i - 1) * (width - 2)] =
          clamp(sqrt(pixel_x * pixel_x + pixel_y * pixel_y), 255, 0);
    }
  }
  return new_image;
}

int* getParallelSobel(int* image, int width, int height) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int delta = 0;
  int rem = 0;

  delta = (height - 2) / size;
  rem = (height - 2) % size;

  if (delta) {
    int row = rem + delta;

    if (rank == 0) {
      for (int proc = 1; proc < size; proc++) {
        MPI_Send(image + row * width, (delta + 2) * width, MPI_INT,
                 proc, 0, MPI_COMM_WORLD);
        row += delta;
      }
    }

    int* local_matrix;

    if (rank == 0) {
      local_matrix = new int[(delta + rem + 2) * width];
      for (int i = 0; i < (delta + rem + 2) * width; i++) {
        local_matrix[i] = image[i];
      }
      local_matrix = getSequentialSobel(local_matrix, width,
                                        delta + rem + 2);
    } else {
      local_matrix = new int[(delta + 2) * width];
      MPI_Status status;
      MPI_Recv(local_matrix, (delta + 2) * width, MPI_INT, 0, 0,
               MPI_COMM_WORLD, &status);
      local_matrix =
          getSequentialSobel(local_matrix, width, delta + 2);
      MPI_Send(local_matrix, delta * (width - 2), MPI_INT, 0, 0,
               MPI_COMM_WORLD);
    }

    int* global_matrix = nullptr;

    if (rank == 0) {
      global_matrix = new int[(width - 2) * (height - 2)];
      for (int i = 0; i < delta + rem; i++) {
        for (int j = 0; j < width - 2; j++) {
          global_matrix[j + i * (width - 2)] =
              local_matrix[j + i * (width - 2)];
        }
      }
      for (int proc = 1; proc < size; proc++) {
        int* temp = new int[delta * (width - 2)];
        MPI_Status status;
        MPI_Recv(temp, delta * (width - 2), MPI_INT, proc, 0,
                 MPI_COMM_WORLD, &status);
        for (int i = proc * delta + rem; i < proc * delta + rem + delta; i++) {
          for (int j = 0; j < width - 2; j++) {
            global_matrix[j + i * (width - 2)] =
                temp[j + (i - proc * delta - rem) * (width - 2)];
          }
        }
      }
    }

    return global_matrix;
  } else {
    if (rank == 0) {
      return getSequentialSobel(image, width, height);
    } else {
      return nullptr;
    }
  }
}
