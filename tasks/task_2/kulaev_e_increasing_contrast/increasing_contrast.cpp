// Copyright 2023 Kulaev Eugene

#include "task_2/kulaev_e_increasing_contrast/increasing_contrast.h"

void Stretching(std::vector<uint8_t>* local_img, uint8_t global_min,
                    uint8_t global_max, uint8_t new_min, uint8_t new_max) {
    if (global_min == global_max || local_img->size() == 0) return;
    for (auto& pixel : *local_img) {
        pixel = boost::algorithm::clamp(pixel, global_min, global_max);
        pixel = ((pixel - global_min) * (new_max - new_min)) / (global_max - global_min) + new_min;
    }
}

uint8_t generateRandomNumbers(uint8_t min, uint8_t max) {
    uint8_t randomNumber = min + (std::rand() % (max - min + 1));

    return randomNumber;
}

uint8_t SeqmetionMax(std::vector<uint8_t>* vec) {
    return *std::max_element(vec->begin(), vec->end());
}

uint8_t SeqmetionMin(std::vector<uint8_t>* vec) {
    return *std::min_element(vec->begin(), vec->end());
}

void ParallelStretching(std::vector<uint8_t>* image, uint8_t new_min, uint8_t new_max) {
    boost::mpi::communicator comm;

    int rankProc = comm.rank();
    int numProc = comm.size();

    int imageSize = image->size();

    if (imageSize == 0) return;

    int chunk_size = imageSize / numProc;
    int remainder = imageSize % numProc;

    std::vector<int> recv_counts(numProc);
    std::vector<int> dis_pls(numProc);

    for (int i = 0; i < numProc; i++) {
        recv_counts[i] = chunk_size;
        if (i < remainder) {
            recv_counts[i]++;
        }
        dis_pls[i] = (i > 0) ? (dis_pls[i - 1] + recv_counts[i - 1]) : 0;
    }

    std::vector<uint8_t> local_img(recv_counts[rankProc]);
    boost::mpi::scatterv(comm, image->data(), recv_counts, dis_pls, local_img.data(), recv_counts[rankProc], 0);

    uint8_t localMin = 255;
    uint8_t localMax = 0;

    if (rankProc < image->size()) {
        localMin = SeqmetionMin(&local_img);
        localMax = SeqmetionMax(&local_img);
    }

    uint8_t globalMin = 255;
    uint8_t globalMax = 0;
    boost::mpi::all_reduce(comm, localMin, globalMin, boost::mpi::minimum<uint8_t>());
    boost::mpi::all_reduce(comm, localMax, globalMax, boost::mpi::maximum<uint8_t>());

    Stretching(&local_img, globalMin, globalMax, new_min, new_max);

    boost::mpi::gatherv(comm, local_img.data(), recv_counts[rankProc], image->data(), recv_counts, dis_pls, 0);
}
