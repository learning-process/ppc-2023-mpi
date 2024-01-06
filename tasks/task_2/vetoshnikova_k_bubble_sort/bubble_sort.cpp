  // Copyright 2023 Vetoshnikova Ekaterina

#include "task_2/vetoshnikova_k_bubble_sort/bubble_sort.h"

int* array_generation(int mn, int mx, int sz) {
    int* arr = new int[sz];
    for (int i = 0; i < sz; i++) {
        arr[i] = mn + (std::rand() % (mx - mn + 1));
    }

    return arr;
}

void SequentialBubbleSort(int* arr, int count_size_arr) {
    for (int i = 0; i < count_size_arr; i++) {
        int j;
        if (i % 2 == 0) j = 1;
        else
            j = 0;
        for (j; j + 1 < count_size_arr; j += 2) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
            }
        }
    }
}

void ParallelBubbleSort(int* global_arr, int count_size_arr) {
    int ProcNum;
    int ProcRank;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    int* counts_element = new int[ProcNum];
    int* dis = new int[ProcNum];
    int delta     = count_size_arr / ProcNum;
    int remainder = count_size_arr % ProcNum;

    for (int i = 0; i < ProcNum; i++) {
        counts_element[i] = delta;
        if (i < remainder) {
            counts_element[i]++;
        }
        if (i > 0) dis[i] = dis[i - 1] + counts_element[i - 1];
        else
            dis[i] = 0;
    }
    int* arr_local = new int[counts_element[ProcRank]];
    MPI_Scatterv(global_arr, counts_element, dis, MPI_INT,
        arr_local, counts_element[ProcRank], MPI_INT, 0, MPI_COMM_WORLD);
    SequentialBubbleSort(arr_local, counts_element[ProcRank]);
    MPI_Gatherv(arr_local, counts_element[ProcRank], MPI_INT,
        global_arr, counts_element, dis, MPI_INT, 0, MPI_COMM_WORLD);
    if (ProcRank == 0) {
        SequentialBubbleSort(global_arr, count_size_arr);
    }
}

