// Copyright 2023 Kruglov Alexey

#include "task_3/kruglov_a_linear_hyst_stretch/linear_hyst_stretch.h"

std::vector<uint8_t> getRandomImage(size_t count_pix, uint8_t min,
    uint8_t max) {
    if (count_pix == 0) return std::vector<uint8_t>();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(min, max);

    std::vector<uint8_t> arr(count_pix);
    for (auto& elem : arr) elem = dist(gen);

    return arr;
}

void seqStretchContrast(std::vector<uint8_t>* image, uint8_t old_min,
    uint8_t old_max, uint8_t new_min, uint8_t new_max) {
    if (old_min >= old_max || new_min > new_max) return;
    for (uint8_t& pix : *image) {
        pix = (pix - old_min) * (new_max - new_min) / (old_max - old_min) + new_min;
    }
}

void parStretchContrast(std::vector<uint8_t>* image, size_t count_pix,
    uint8_t new_min, uint8_t new_max) {
    if (new_min > new_max) return;

    int rank = 0;
    int size = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int chunk = static_cast<int>(count_pix / size);
    const int tail = static_cast<int>(count_pix % size);

    std::vector<int> send_counts(size, chunk);
    std::vector<int> displs(size, 0);

    for (int i = 0; i < size; ++i) {
        if (i < tail) ++send_counts[i];
        displs[i] = i == 0 ? 0 : displs[i - 1] + send_counts[i - 1];
    }

    uint8_t global_min = 255;
    uint8_t global_max = 0;

    std::vector<uint8_t> local_img(send_counts[rank]);

    MPI_Scatterv(image->data(), send_counts.data(), displs.data(), MPI_UINT8_T,
        local_img.data(), send_counts[rank], MPI_UINT8_T, 0, MPI_COMM_WORLD);

    uint8_t local_min = 255;
    uint8_t local_max = 0;

    if (rank < count_pix) {
        local_min = *std::min_element(local_img.begin(), local_img.end());
        local_max = *std::max_element(local_img.begin(), local_img.end());
    }

    MPI_Allreduce(&local_min, &global_min, 1, MPI_UINT8_T, MPI_MIN, MPI_COMM_WORLD);
    MPI_Allreduce(&local_max, &global_max, 1, MPI_UINT8_T, MPI_MAX, MPI_COMM_WORLD);

    seqStretchContrast(&local_img, global_min, global_max, new_min, new_max);

    MPI_Gatherv(local_img.data(), send_counts[rank], MPI_UINT8_T, image->data(),
        send_counts.data(), displs.data(), MPI_UINT8_T, 0, MPI_COMM_WORLD);
}
