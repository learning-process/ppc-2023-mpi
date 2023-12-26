// Copyright 2023 Kozlov Mikhail
#include <random>
#include <iostream>
#include <algorithm>
#include "task_3/kozlov_m_shell_sort_simple_merge/shell_sort.h"



void merge(std::vector<int> arr, int low, int mid, int high) {
    int n1 = mid - low + 1;
    int n2 = high - mid;

    std::vector<int> left(n1), right(n2);

    for (int i = 0; i < n1; i++)
        left[i] = arr[low + i];

    for (int j = 0; j < n2; j++)
        right[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = low;

    while (i < n1 && j < n2) {
        if (left[i] <= right[j]) {
            arr[k] = left[i];
            i++;
        } else {
            arr[k] = right[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = left[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = right[j];
        j++;
        k++;
    }
}

void seq_sort(std::vector<int> arr) {
    int n = arr.size();
    for (int offset = n / 2; offset > 0; offset /= 2) {
        for (int i = offset; i < n; i++) {
            int temp = arr[i];
            int j;
            for (j = i; j >= offset && arr[j - offset] > temp; j -= offset)
                arr[j] = arr[j - offset];
            arr[j] = temp;
        }
    }
}

void parallel_sort(std::vector<int> arr) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int n = arr.size();
    int local_size = n / size;
    std::vector<int> local_arr(local_size);
    MPI_Scatter(arr.data(), local_size, MPI_INT, local_arr.data(), local_size, MPI_INT, 0, MPI_COMM_WORLD);

    seq_sort(local_arr);

    MPI_Gather(local_arr.data(), local_size, MPI_INT, arr.data(), local_size, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        for (int offset = n / 2; offset > 0; offset /= 2) {
            for (int i = 0; i + offset < n; i += 2 * offset) {
                int low = i;
                int mid = i + offset - 1;
                int high = std::min(i + 2 * offset - 1, n - 1);
                merge(arr, low, mid, high);
            }
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);
}
