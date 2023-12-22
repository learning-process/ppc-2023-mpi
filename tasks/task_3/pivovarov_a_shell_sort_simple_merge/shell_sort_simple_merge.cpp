// Copyright 2023 Pivovarov Alexey

#include "task_3/pivovarov_a_shell_sort_simple_merge/shell_sort_simple_merge.h"

std::vector<int> generate_random_array(int length, int minValue, int maxValue) {
    std::vector<int> randomArray(length);

    for (int i = 0; i < length; ++i) {
        randomArray[i] = std::rand() % (maxValue - minValue + 1) + minValue;
    }

    return randomArray;
}

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

void shell_sort_seq(std::vector<int> arr) {
    int n = arr.size();
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            int temp = arr[i];
            int j;
            for (j = i; j >= gap && arr[j - gap] > temp; j -= gap)
                arr[j] = arr[j - gap];
            arr[j] = temp;
        }
    }
}

void shell_sort_par(std::vector<int> arr, const boost::mpi::communicator& world) {
    int rank = world.rank();
    int size = world.size();

    int n = arr.size();
    int local_size = n / size;
    std::vector<int> local_arr(local_size);

    MPI_Scatter(arr.data(), local_size, MPI_INT, local_arr.data(), local_size, MPI_INT, 0, MPI_COMM_WORLD);

    shell_sort_seq(local_arr);

    MPI_Gather(local_arr.data(), local_size, MPI_INT, arr.data(), local_size, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        for (int gap = n / 2; gap > 0; gap /= 2) {
            for (int i = 0; i + gap < n; i += 2 * gap) {
                int low = i;
                int mid = i + gap - 1;
                int high = std::min(i + 2 * gap - 1, n - 1);
                merge(arr, low, mid, high);
            }
        }
    }

    world.barrier();
}
