// Copyright 2023 Kulaev Eugene

#include "task_2/kulaev_e_contrast_enhancement/contrast_enha.h"

void Seq_linearStretchingHistogram(std::vector<int>* local_img, int global_min,
                    int global_max, int new_min, int new_max) {
    if (global_min == global_max || local_img->size() == 0) return;
    for (auto& pixel : *local_img) {
        pixel = boost::algorithm::clamp(pixel, global_min, global_max);
        pixel = ((pixel - global_min) * (new_max - new_min)) / (global_max - global_min) + new_min;
    }
}

void Par_linearStretchingHistogram(std::vector<int>* image, int new_min, int new_max, int n, int m) {
    boost::mpi::communicator comm;

    int rankProc = comm.rank();
    int numProc = comm.size();

    int imageSize = image->size();

    if (imageSize == 0 || imageSize < n * m) return;

    int chunk_size  = imageSize / numProc;
    int remainder = imageSize % numProc;

    std::vector<int> recv_counts(numProc, chunk_size);
    std::vector<int> dis_pls(numProc, 0);

     for (int i = 0; i < numProc; i++) {
        recv_counts[i] = chunk_size;
        if (i < remainder) {
            recv_counts[i]++;
        }
        dis_pls[i] = (i > 0) ? (dis_pls[i - 1] + recv_counts[i - 1]) : 0;
    }

    int globalMin = 255;
    int globalMax = 0;

    std::vector<int> local_img(recv_counts[rankProc]);

    boost::mpi::scatterv(comm, image->data(), recv_counts, dis_pls, local_img.data(), recv_counts[rankProc], 0);

    int localMin = 255;
    int localMax = 0;

    if (rankProc < image->size()) {
        localMin = *std::min_element(local_img.begin(), local_img.end());
        localMax = *std::max_element(local_img.begin(), local_img.end());
    }

    boost::mpi::all_reduce(comm, localMin, globalMin, boost::mpi::minimum<uint8_t>());
    boost::mpi::all_reduce(comm, localMax, globalMax, boost::mpi::maximum<uint8_t>());

    Seq_linearStretchingHistogram(&local_img, globalMin, globalMax, new_min, new_max);

    boost::mpi::gatherv(comm, local_img.data(), recv_counts[rankProc], image->data(), recv_counts, dis_pls, 0);
}

int generateRandomNumbers(int min, int max) {
    int randomNumber = min + (std::rand() % (max - min + 1));

    return randomNumber;
}
