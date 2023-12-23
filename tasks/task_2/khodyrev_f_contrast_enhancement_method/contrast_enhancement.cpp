// Copyright 2023 Khodyrev Fedor
#include "task_2/khodyrev_f_contrast_enhancement_method/contrast_enhancement.h"

#include <mpi.h>
#include <iostream>
#include <ctime>
#include <algorithm>
#include <vector>
#include <cstdlib>


bool compareMatrix(const std::vector<std::vector<int>>& v1,
  const std::vector<std::vector<int>>& v2) {
    if (v1.size() != v2.size()) {
        return false;
    }
    for (size_t i = 0; i < v1.size(); ++i) {
        if (v1[i] != v2[i]) {
            return false;
        }
    }
    return true;
}

std::vector<std::vector<int>> generate_image(int rows, int cols) {
    std::vector<std::vector<int>> image(rows, std::vector<int>(cols));
    std::srand(static_cast<unsigned int>(std::time(0)));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            image[i][j] = std::rand() % 256;
        }
    }
    return image;
}

std::vector<std::vector<int>> seq_linear_contrast_stretching
  (std::vector<std::vector<int>> image, int rows, int cols) {
    int minIntensity = 255, maxIntensity = 0;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            minIntensity = std::min(minIntensity, image[i][j]);
            maxIntensity = std::max(maxIntensity, image[i][j]);
        }
    }
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            image[i][j] = static_cast<int>((image[i][j] - minIntensity)
            * 255.0 / (maxIntensity - minIntensity));
        }
    }
    return image;
}

std::vector<std::vector<int>> par_linear_contrast_stretching
  (std::vector<std::vector<int>> image, int rows, int cols) {
    int minIntensity = 255, maxIntensity = 0;
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
      for (int i = 0; i < rows; ++i) {
          for (int j = 0; j < cols; ++j) {
              minIntensity = std::min(minIntensity, image[i][j]);
              maxIntensity = std::max(maxIntensity, image[i][j]);
          }
      }
    }

    MPI_Bcast(&minIntensity, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&maxIntensity, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int totalElements = rows * cols;
    int elementsPerProcess = totalElements / size;
    int remainder = totalElements % size;

    std::vector<int> imageInVector;

    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
        imageInVector.push_back(image[i][j]);
      }
    }

    std::vector<int> sendCounts(size, elementsPerProcess);
    std::vector<int> displacements(size, 0);

    for (int i = 0; i < remainder; ++i) {
        sendCounts[i]++;
    }

    for (int i = 1; i < size; ++i) {
        displacements[i] = displacements[i - 1] + sendCounts[i - 1];
    }

    std::vector<int> localData(sendCounts[rank]);
    MPI_Scatterv(imageInVector.data(), sendCounts.data(), displacements.data(),
    MPI_INT, localData.data(), sendCounts[rank], MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < localData.size(); ++i) {
        localData[i] = static_cast<int>((localData[i] - minIntensity) * 255 /
         (maxIntensity - minIntensity));
    }

    std::vector<int> gatheredData(totalElements);
    MPI_Gatherv(localData.data(), sendCounts[rank], MPI_INT,
      gatheredData.data(), sendCounts.data(), displacements.data(),
        MPI_INT, 0, MPI_COMM_WORLD);

    std::vector<std::vector<int>> result(rows, std::vector<int>(cols));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            result[i][j] = gatheredData[i * cols + j];
        }
    }

    if (rank == 0) {
      return result;
    } else {
      return std::vector<std::vector<int>>();
    }
}
