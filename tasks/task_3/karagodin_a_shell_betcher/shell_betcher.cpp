// Copyright 2023 Karagodin Andrey

#include <task_3/karagodin_a_shell_betcher/shell_betcher.h>

std::vector<int> MergeArray(const std::vector<int>& arr1,
 const std::vector<int>& arr2) {
    std::vector<int> mergeArray(arr1.size() + arr2.size());
    for (int i = 0; i < arr1.size(); i++) {
        mergeArray[i] = arr1[i];
    }
    for (int i = arr1.size(), j = 0; i < mergeArray.size(); i++, j++) {
        mergeArray[i] = arr2[j];
    }
    return mergeArray;
}

void MergeBatcher(std::vector<int>* arr, int high,
 int str, int r) {
    int step = r * 2;
    if (step < (high - str)) {
        MergeBatcher(arr, high, str, step);
        MergeBatcher(arr, high, str + r, step);
        for (int i = str + r; i < high - r; i += step) {
            if ((*arr)[i] > (*arr)[i + r]) std::swap((*arr)[i], (*arr)[i + r]);
    }
    } else {
        if ((*arr)[str] > (*arr)[str + r]) std::swap((*arr)[str],
         (*arr)[str + r]);
    }
}

std::vector<int> ShellSortSequential(std::vector<int> arr) {
    int step = arr.size() / 2;
    while (step > 0) {
        int j;
        for (int i = step; i < arr.size(); i++) {
            int value = arr[i];
            for (j = i - step; (j >= 0) && (arr[j] > value);
             j -= step) arr[j + step] = arr[j];
            arr[j + step] = value;
        }
        step /= 2;
    }
    return arr;
}

std::vector<int> ShellSortParallel(const std::vector<int>& arr,
 int arrSize) {
    std::vector<int> global_array;
    int size = 0, rank = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int delta = arrSize / size;
    std::vector<int> local_array(delta);
    MPI_Scatter(arr.data(), delta, MPI_INT, local_array.data(),
     delta, MPI_INT, 0, MPI_COMM_WORLD);
    local_array = ShellSortSequential(local_array);
    if (rank == 0) {
        std::vector<int> res_array;
        MPI_Status status;
        global_array = MergeArray(global_array, local_array);
        for (int proc = 1; proc < size; proc++) {
            MPI_Recv(local_array.data(), delta, MPI_INT, proc, 0,
             MPI_COMM_WORLD, &status);
            global_array = MergeArray(global_array, local_array);
        }
        if (size != 1) {
            for (int i = 0, j = 1; i < size / 2; i++, j++) {
                if (size - 1 == 1) {
                    MergeBatcher(&global_array, global_array.size(), 0, 1);
                } else {
                    res_array = std::vector<int>(global_array.begin() +
                     delta * size / 2 * i, global_array.begin() +
                      delta * size / 2 * j);
                    MergeBatcher(&res_array, res_array.size(), 0, 1);
                    global_array.erase(global_array.begin() +
                     delta * size / 2 * i, global_array.begin() +
                      delta * size / 2 * j);
                    global_array.insert(global_array.begin() +
                     delta * size / 2 * i, res_array.begin(),
                      res_array.end());
                }
            }
        }
        MergeBatcher(&global_array, global_array.size(), 0, 1);
    } else {
        MPI_Send(local_array.data(), delta, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    return global_array;
}

std::vector<int> GenerateArrayRandom(int arrSize) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector<int> array(arrSize);
    for (int i = 0; i < arrSize; i++) {
      array[i] = static_cast<int>(gen() % 100);
    }
    return array;
}
