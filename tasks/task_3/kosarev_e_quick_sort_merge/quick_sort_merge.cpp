// Copyright 2023 Kosarev Egor
#include "task_3/kosarev_e_quick_sort_merge/quick_sort_merge.h"


std::pair<int, int> split(int* array, int n) {
    int val = array[n / 2];
    int left = 0, right = n - 1;
    while (left<= right) {
        while (array[left] < val) ++left;
        while (array[right] > val) --right;
        if (left<= right) {
            std::swap(array[left], array[right]);
            left++;
            right--;
        }
    }
    return { left, right };
}

void recursive_quick_sort(int* array, int n) {
    if (n < 2) return;
    std::pair<int, int> pos = split(array, n);
    if (pos.second > 0) recursive_quick_sort(array, pos.second + 1);
    if (pos.first < n) recursive_quick_sort(&array[pos.first], n - pos.first);
}

void Merge(int* array1, int size1, int size2) {
    int* array2 = &array1[size1];
    int com_size = size1 + size2;
    int* array = new int[com_size];
    int left, right;
    left = right = 0;
    int i = 0;
    while (left< size1 && right < size2) {
        if (array1[left] < array2[right]) {
            array[i] = array1[left];
            i++;
            left++;
        } else {
            array[i] = array2[right];
            i++;
            right++;
        }
    }
    while (left< size1) array[i++] = array1[left++];
    while (right < size2) array[i++] = array2[right++];

    for (i = 0; i < com_size; ++i) {
        array1[i] = array[i];
    }
    delete[] array;
}

void parallelQuickSort(int* array, size_t n) {
    int rank = 0;
    int ProcNum = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        MPI_Status status;
        int my_count = (n % ProcNum);
        int other_count = (n - my_count) / ProcNum;
        my_count += other_count;

        for (int i = 1; i < ProcNum; ++i) {
            MPI_Send(&array[my_count + other_count * (i - 1)], other_count, MPI_INT, i,
                123, MPI_COMM_WORLD);
        }
        recursive_quick_sort(array, my_count);

        for (int i = 1; i < ProcNum; ++i) {
            MPI_Recv(&array[my_count + other_count * (i - 1)], other_count, MPI_INT, i,
                456, MPI_COMM_WORLD, &status);
        }

        for (int i = 1; i < ProcNum; ++i) {
            Merge(array, my_count + other_count * (i - 1), other_count);
        }
    } else {
        MPI_Status status;
        int* array;
        int count;
        MPI_Probe(0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_INT, &count);
        array = new int[count];
        MPI_Recv(array, count, MPI_INT, 0, 123, MPI_COMM_WORLD, &status);

        recursive_quick_sort(array, count);

        MPI_Send(array, count, MPI_INT, 0, 456, MPI_COMM_WORLD);

        delete[] array;
    }
}

bool checking(int* array, int n) {
    for (int i = 0; i < n - 1; ++i) {
        if (array[i] > array[i + 1]) return false;
    }
    return true;
}
