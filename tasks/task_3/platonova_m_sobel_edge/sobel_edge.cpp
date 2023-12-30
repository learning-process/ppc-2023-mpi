// Copyright 2023 Platonova Maria
#include "task_3/platonova_m_sobel_edge/sobel_edge.h"
#include <utility>
#include <boost/mpi.hpp>

using ConvolutionKernel = std::vector<std::vector<int>>;

// Internal parallel declarations
void DistributeImage(
        const ImageMatrix& image, ImageMatrix& localImage, // NOLINT
        size_t width, size_t height);

std::pair<size_t, size_t> LocalRowsOffsetAndCountForDistribution(
        size_t width, size_t height,
        size_t processesCount, size_t processRank);


void CollectImage(
        ImageMatrix& totalImage, const ImageMatrix& localImage, // NOLINT
        size_t width, size_t height);

size_t LocalRowsCountForCollection(
        size_t width, size_t height,
        size_t processesCount, size_t processRank);

namespace mpi = boost::mpi;


// Definitions
ImageMatrix SequentialSobelOperator(
        const ImageMatrix& image, size_t width, size_t height
) {
    ConvolutionKernel convolutionByX = {
            {-1, 0, +1},
            {-2, 0, +2},
            {-1, 0, +1}
    };

    ConvolutionKernel convolutionByY = {
            {-1, -2, -1},
            {0, 0, 0},
            {+1, +2, +1}
    };

    ImageMatrix resultMatrix((height - 2) * (width - 2));

    for (size_t y = 1; y < height - 1; ++y) {
        for (size_t x = 1; x < width - 1; ++x) {
            int dx = 0;
            int dy = 0;

            for (int j = -1; j <= 1; ++j) {
                for (int i = -1; i <= 1; ++i) {
                    auto intensity = image[(y + j) * width + x + i];

                    dx += intensity * convolutionByX[j + 1][i + 1];
                    dy += intensity * convolutionByY[j + 1][i + 1];
                }
            }

            auto magnitude = std::min(
                    static_cast<int>(sqrt(dx * dx + dy * dy)),
                    255);

            resultMatrix[(y - 1) * (width - 2) + (x - 1)] = magnitude;
        }
    }

    return std::move(resultMatrix);
}

ImageMatrix ParallelSobelOperator(
        const ImageMatrix& image, size_t width, size_t height
) {
    ImageMatrix localImage, localResult, totalParallelResult;

    DistributeImage(image, localImage, width, height);

    if (!localImage.empty()) {
        localResult = SequentialSobelOperator(
                localImage, width, localImage.size() / width);
    }

    CollectImage(totalParallelResult, localResult, width - 2, height - 2);

    return std::move(totalParallelResult);
}

void DistributeImage(const ImageMatrix& image, ImageMatrix& localImage, // NOLINT
                     size_t width, size_t height) {
    mpi::communicator world;

    auto rank = world.rank();

    auto [_, localImageSize] = LocalRowsOffsetAndCountForDistribution(
            width, height, world.size(), rank);

    localImage.resize(localImageSize, 0);

    if (rank == 0) {
        std::vector<int>
            processesImagesSizes(world.size()),
            processesImagesOffsets(world.size());

        for (int i = 0; i < world.size(); ++i) {
            auto [iProcessRowOffset, iProcessLocalImageSize] =
                    LocalRowsOffsetAndCountForDistribution(width, height,
                                                           world.size(), i);

            processesImagesSizes[i] = iProcessLocalImageSize;
            processesImagesOffsets[i] = iProcessRowOffset;
        }

        mpi::scatterv(
                world, image.data(),
                processesImagesSizes, processesImagesOffsets,
                localImage.data(), processesImagesSizes[0], 0);
    } else {
        mpi::scatterv(
            world, localImage.data(),
            localImageSize, 0);
    }
}

void CollectImage(ImageMatrix& totalImage, // NOLINT
                  const ImageMatrix& localImage, size_t width, size_t height) {
    mpi::communicator world;

    auto rank = world.rank();

    if (rank == 0) {
        totalImage.resize(width * height, 0);

        std::vector<int> processesImageSizes(world.size());

        for (int i = 0; i < world.size(); ++i) {
            auto iProcessLocalImageSize = LocalRowsCountForCollection(
                    width, height, world.size(), i);

            processesImageSizes[i] = iProcessLocalImageSize;
        }

        mpi::gatherv(
                world, localImage, totalImage.data(),
                processesImageSizes, 0);
    } else {
        mpi::gatherv(world, localImage, 0);
    }
}

size_t LocalRowsCountForCollection(size_t width, size_t height,
                                   size_t processesCount, size_t processRank
) {
    auto distributeRowsCount = height;

    auto rowsPerProcess = distributeRowsCount / processesCount;
    auto remainder = distributeRowsCount % processesCount;

    auto rank = processRank;

    auto localRowsCount = rowsPerProcess + (rank < remainder ? 1 : 0);

    if (rank >= distributeRowsCount) {
        return 0;
    }

    return localRowsCount * width;
}

std::pair<size_t, size_t> LocalRowsOffsetAndCountForDistribution(
        size_t width, size_t height, size_t processesCount, size_t processRank
) {
    auto distributeRowsCount = height - 2;

    auto rowsPerProcess = distributeRowsCount / processesCount;
    auto remainder = distributeRowsCount % processesCount;

    auto rank = processRank;

    auto localRowsCount = rowsPerProcess +
            (rank < remainder ? 1 : 0);
    auto absoluteRowOffset = rank * rowsPerProcess +
            (rank < remainder ? rank : remainder);

    if (rank >= distributeRowsCount) {
        return std::make_pair(0, 0);
    }

    return std::make_pair(absoluteRowOffset * width,
                          (localRowsCount + 2) * width);
}
