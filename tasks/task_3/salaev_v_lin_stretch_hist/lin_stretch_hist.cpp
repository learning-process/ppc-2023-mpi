// Copyright 2023 Salaev Vladislav
#include "task_3/salaev_v_lin_stretch_hist/lin_stretch_hist.h"

std::vector<uint8_t> generateImage(
size_t width, size_t height, uint8_t minVal, uint8_t maxVal) {
    if (width * height == 0) return std::vector<uint8_t>();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(minVal, maxVal);

    std::vector<uint8_t> image(width * height);
    for (auto& pixel : image) {
        pixel = static_cast<uint8_t>(dist(gen));
    }

    return image;
}

void increaseContrast(
std::vector<uint8_t>* img,
uint8_t oldMin, uint8_t oldMax, uint8_t newMin, uint8_t newMax) {
    if (img->empty() || oldMin == oldMax) return;

    for (auto& pixel : *img) {
        int newPixel = static_cast<int>(
            (
                pixel - oldMin) * (newMax - newMin) / static_cast<float>(
                    oldMax - oldMin) + newMin);
        pixel = static_cast<uint8_t>(std::min(std::max(newPixel, 0), 255));
    }
}

void parallelIncreaseContrast(
std::vector<uint8_t>* img,
size_t width, size_t height, uint8_t newMin, uint8_t newMax, MPI_Comm comm) {
    int rank, size;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    size_t totalPixels = width * height;
    if (totalPixels == 0 || img->size() != totalPixels) return;

    int pixelsPerProc = totalPixels / size;
    int remainingPixels = totalPixels % size;

    std::vector<int> counts(size, pixelsPerProc);
    std::vector<int> displacements(size, 0);
    for (int i = 0; i < size; ++i) {
        if (i < remainingPixels) ++counts[i];
        displacements[i] = (i == 0) ? 0 : displacements[i - 1] + counts[i - 1];
    }

    std::vector<uint8_t> localImg(counts[rank]);
    MPI_Scatterv(
        img->data(),
        counts.data(),
        displacements.data(),
        MPI_UINT8_T, localImg.data(), counts[rank], MPI_UINT8_T, 0, comm);

    uint8_t localMin = *std::min_element(localImg.begin(), localImg.end());
    uint8_t localMax = *std::max_element(localImg.begin(), localImg.end());
    uint8_t globalMin, globalMax;

    MPI_Allreduce(&localMin, &globalMin, 1, MPI_UINT8_T, MPI_MIN, comm);
    MPI_Allreduce(&localMax, &globalMax, 1, MPI_UINT8_T, MPI_MAX, comm);

    increaseContrast(&localImg, globalMin, globalMax, newMin, newMax);
    MPI_Gatherv(
        localImg.data(),
        counts[rank],
        MPI_UINT8_T,
        img->data(),
        counts.data(),
        displacements.data(), MPI_UINT8_T, 0, comm);
}
