// Copyright 2023 Ionova Ekaterina
#include "task_2/ionova_e_smoothing_filter/filter.h"

std::vector<uint8_t> seqFilter(std::vector<uint8_t> img, int rows, int cols,
                               int kernel) {
  if (img.size() <= 1) return img;

  std::vector<uint8_t> resImg(rows * cols);
  std::vector<uint8_t> values;
  values.reserve(kernel * kernel);
  int radius = kernel / 2;

  for (int y = 0; y < rows; ++y) {
    for (int x = 0; x < cols; ++x) {
      values.clear();

      for (int l = -radius; l <= radius; ++l) {
        for (int k = -radius; k <= radius; ++k) {
          int posX = std::clamp(y + l, 0, rows - 1);
          int posY = std::clamp(x + k, 0, cols - 1);

          values.emplace_back(img[posX * cols + posY]);
        }
      }
      std::sort(values.begin(), values.end());
      resImg[y * cols + x] = values[(kernel * kernel) / 2];
    }
  }
  return resImg;
}

std::vector<uint8_t> parFilter(std::vector<uint8_t> img, int rows, int cols,
                               int kernel, MPI_Comm comm) {
  if (img.size() <= 1) return img;

  int rank = 0;
  int sizeWorld = 0;

  MPI_Comm_size(comm, &sizeWorld);
  MPI_Comm_rank(comm, &rank);

  const int chunk = rows / sizeWorld;
  const int tail = rows % sizeWorld;
  std::vector<uint8_t> resImg(rows * cols);

  if (rank == 0) {
    for (int i = 1; i < sizeWorld; ++i) {
      int sends = cols * (i == sizeWorld - 1 ? chunk + tail + 1 : chunk + 2);
      MPI_Send(img.data() + cols * (i * chunk - 1), sends, MPI_UINT8_T, i, 0,
               comm);
    }

    int count = cols * (chunk + (sizeWorld == 1 ? 0 : 1));
    std::vector<uint8_t> locImg(img.begin(), img.begin() + count);
    std::vector<uint8_t> res =
        seqFilter(locImg, chunk + (sizeWorld == 1 ? 0 : 1), cols, kernel);
    std::vector<uint8_t> resLoc(res.begin(),
                                res.end() - (sizeWorld == 1 ? 0 : 1) * cols);

    int ind = 0;
    unionImage(resLoc, &resImg, &ind);

    for (int i = 1; i < sizeWorld; ++i) {
      int count =
          cols * (chunk + (i == sizeWorld - 1 && sizeWorld != 1 ? tail : 0));
      std::vector<uint8_t> resProc(count);

      MPI_Recv(resProc.data(), count, MPI_UINT8_T, i, 0, comm,
               MPI_STATUS_IGNORE);

      unionImage(resProc, &resImg, &ind);
    }
  } else {
    bool flag = rank == sizeWorld - 1;
    int localSize = flag ? chunk + tail + 1 : chunk + 2;
    std::vector<uint8_t> locImg(localSize * cols);

    MPI_Recv(locImg.data(), localSize * cols, MPI_UINT8_T, 0, 0, comm,
             MPI_STATUS_IGNORE);

    std::vector<uint8_t> res = seqFilter(locImg, localSize, cols, kernel);
    std::vector<uint8_t> resLoc(res.begin() + cols,
                                res.end() - (flag ? 0 : cols));

    MPI_Send(resLoc.data(), cols * (flag ? chunk + tail : chunk), MPI_UINT8_T,
             0, 0, comm);
  }
  return resImg;
}

void unionImage(const std::vector<uint8_t>& subImg, std::vector<uint8_t>* img,
                int* ind) {
  int size = subImg.size();
  for (int i = 0; i < size; ++i) (*img)[i + *ind] = subImg[i];
  *ind += size;
}
