// Copyright 2023 Tushentsova Karina
#include <mpi.h>
#include <vector>
#include <random>
#include <algorithm>
#include <utility>
#include "task_2/tushentsova_k_odd_even_sort/odd_even_sort.h"

std::vector<int> getRandomVector(int size) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector<int> arr(size);
    for (int i = 0; i < size; i++) {
        arr[i] = gen() % 100;
    }
    return arr;
}

std::vector<int> bubbleSortOddEven(std::vector<int> arr) {
    int n = arr.size();
    for (int i = 0; i < n; i++) {
        if (i % 2) {
            for (int j = 1; j < n - 1; j += 2) {
                if (arr[j] > arr[j + 1]) {
                    std::swap(arr[j], arr[j + 1]);
                }
            }
        } else {
            for (int j = 0; j < n - 1; j += 2) {
                if (arr[j] > arr[j + 1]) {
                    std::swap(arr[j], arr[j + 1]);
                }
            }
          }
    }
    return arr;
}

std::vector<int> parallBubbleSortOddEven(std::vector<int> arr) {
    int size, rank;
    MPI_Status status;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int* count = new int[size];
    int* dis = new int[size];
    int n = arr.size();

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < size; i++) {
        count[i] = n / size;
    }
    for (int i = 0; i < n % size; i++) {
        count[i] += 1;
    }
    dis[0] = 0;
    for (int i = 1; i < size; i++) {
        dis[i] = dis[i - 1] + count[i - 1];
    }

    int* part_arr = new int[count[rank]];
    int odd, a;
    MPI_Scatterv(arr.data(), count, dis, MPI_INT,
        part_arr, count[rank], MPI_INT, 0, MPI_COMM_WORLD);
    for (int i = 0; i < n; i++) {
        if (i % 2) {
            odd = !(dis[rank] % 2);
        } else {
            odd = dis[rank] % 2;
          }
        for (int j = odd; j < count[rank]; j += 2) {
            if (j == count[rank] - 1) {
                if (rank != size - 1) {
                    MPI_Recv(&a, 1, MPI_INT, MPI_ANY_SOURCE,
                        MPI_ANY_TAG, MPI_COMM_WORLD, &status);
                    if (part_arr[count[rank] - 1] > a) {
                        std::swap(part_arr[count[rank] - 1], a);
                    }
                    MPI_Send(&a, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
                }
                break;
            }
            if (part_arr[j] > part_arr[j + 1]) {
                std::swap(part_arr[j], part_arr[j + 1]);
            }
        }
        if (odd && rank != 0) {
            MPI_Send(&(part_arr[0]), 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD);
            MPI_Recv(&(part_arr[0]), 1, MPI_INT, MPI_ANY_SOURCE,
                MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }
    MPI_Gatherv(part_arr, count[rank], MPI_INT, arr.data(),
        count, dis, MPI_INT, 0, MPI_COMM_WORLD);
    delete[] count;
    delete[] dis;
    delete[] part_arr;
    return arr;
}
