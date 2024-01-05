// Copyright 2023 Vetoshnikova Ekaterina

#include "task_2/vetoshnikova_k_bubble_sort/bubble_sort.h"

std::vector<int> array_generation(int mn, int mx, int sz) {
    std::vector<int> arr(sz);
    for (int i = 0; i < sz; i++) { 
        arr[i] = mn + (std::rand() % (mx - mn + 1));
    }

    return arr;
}

void SequentialBubbleSort(std::vector<int>& arr, int count_size_arr) {
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

void ParallelBubbleSort(std::vector<int>& global_arr, int count_size_arr) {
    int ProcNum;
    int ProcRank;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    std::vector<int> counts_element(ProcNum);
    std::vector<int> dis(ProcNum);
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
    std::vector<int> arr_local(counts_element[ProcRank]);
    MPI_Scatterv(global_arr.data(), counts_element.data(), dis.data(), MPI_INT,
        arr_local.data(), counts_element[ProcRank], MPI_INT, 0, MPI_COMM_WORLD);
    SequentialBubbleSort(arr_local, counts_element[ProcRank]);
    MPI_Gatherv(arr_local.data(), counts_element[ProcRank], MPI_INT,
        global_arr.data(), counts_element.data(), dis.data(), MPI_INT, 0, MPI_COMM_WORLD);
    if (ProcRank == 0) {
        SequentialBubbleSort(global_arr, count_size_arr);
    }
}

