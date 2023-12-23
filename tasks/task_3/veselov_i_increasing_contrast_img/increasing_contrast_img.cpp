// Copyright 2023 Veselov Ilya
#include "task_3/veselov_i_increasing_contrast_img/increasing_contrast_img.h"

std::vector<uint8_t> generateImg(size_t width, size_t height, uint8_t min_, uint8_t max_) {
  if (width * height == 0)
    return std::vector<uint8_t>();
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dist(min_, max_);
  size_t s = width * height;
  std::vector<uint8_t> img(s);
  for (auto& pixel : img)
    pixel = dist(gen);
  return img;
}

void incContrastImg(std::vector<uint8_t>* img, uint8_t oldMin,
                     uint8_t oldMax, uint8_t newMin, uint8_t newMax) {
  if (oldMin == oldMax || img->empty())
    return;
  for (auto& pixel : *img) {
    pixel = (pixel - oldMin) * (newMax - newMin) / (oldMax - oldMin) + newMin;
    if (pixel < 0) {
        pixel = 0;
    } else if (pixel > 255) {
        pixel = 255;
    }
  }
}

void parIncContrastImg(std::vector<uint8_t>* img, size_t width, size_t height,
                       uint8_t newMin, uint8_t newMax, MPI_Comm comm) {
  int rank;
  int size;
  size_t pixelNumber = img->size();
  if (pixelNumber < width * height || pixelNumber == 0)
    return;
  MPI_Comm_rank(comm, &rank);
  MPI_Comm_size(comm, &size);
  const int elemsPerProc = static_cast<int>(pixelNumber / size);
  const int remainderElems = static_cast<int>(pixelNumber % size);
  std::vector<int> elemsCounts(size, elemsPerProc);
  std::vector<int> displs(size, 0);
  for (int i = 0; i < size; ++i) {
    if (i < remainderElems)
      ++elemsCounts[i];
    displs[i] = i == 0 ? 0 : displs[i - 1] + elemsCounts[i - 1];
  }
  uint8_t globalMin = 255, globalMax = 0;
  uint8_t locMin = 255, locMax = 0;
  std::vector<uint8_t> locImg(elemsCounts[rank]);
  MPI_Scatterv(img->data(), elemsCounts.data(), displs.data(), MPI_UINT8_T,
               locImg.data(), elemsCounts[rank], MPI_UINT8_T, 0, comm);
  if (!locImg.empty()) {
    locMin = *std::min_element(locImg.begin(), locImg.end());
    locMax = *std::max_element(locImg.begin(), locImg.end());
  }
  MPI_Allreduce(&locMin, &globalMin, 1, MPI_UINT8_T, MPI_MIN, comm);
  MPI_Allreduce(&locMax, &globalMax, 1, MPI_UINT8_T, MPI_MAX, comm);
  incContrastImg(&locImg, globalMin, globalMax, newMin, newMax);
  MPI_Gatherv(locImg.data(), elemsCounts[rank], MPI_UINT8_T, img->data(),
               elemsCounts.data(), displs.data(), MPI_UINT8_T, 0, comm);
}
