  // Copyright 2023 Vetoshnikova Ekaterina

#include "task_3/vetoshnikova_k_component_marking/component_marking.h"

void ParallelMarkingComponent(image* img) {
    int ProcNum;
    int ProcRank;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Status status;
    int res = 0;
    int delta = img->m / ProcNum;
    int remainder = img->m % ProcNum;
    int count_string_pr;

    if (ProcRank == 0) {
        count_string_pr = delta + remainder;
    } else {
        count_string_pr = delta;
    }
    int label = ProcRank * 100;
    int* global_image, * local_image;
    global_image = new int[img->m * img->n];
    local_image = new int[count_string_pr * img->n];
    image* local_img;
    local_img = new image(count_string_pr, img->n);

    if (ProcRank == 0) {
        for (int i = 0; i < img->m; i++) {
            for (int j = 0; j < img->n; j++) {
                global_image[i * img->n + j] = img->data[i][j];
            }
        }
        for (int i = 0; i < (count_string_pr)*img->n; i++) {
            local_image[i] = global_image[i];
        }
        for (int i = 1; i < ProcNum; i++) {
            MPI_Send(&global_image[i * img->n * delta + remainder * img->n], delta * img->n,
                MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    } else {
        MPI_Recv(&local_image[0], delta * img->n, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }
    for (int i = 0; i < count_string_pr; i++) {
        for (int j = 0; j < img->n; j++) {
           local_img->data[i][j] = local_image[i * img->n + j];
        }
    }
    int fl = 0;
    int label_count = 0;
    for (int i = 0; i < local_img->m; i++) {
        for (int j = 0; j < img->n; j++) {
            if (local_img->data[i][j] == 1) {
                fl = 1;
                local_img->data[i][j] = label;
                if (i + 1 < count_string_pr && local_img->data[i + 1][j] == 1) {
                    int k = i + 1;
                    while (k < count_string_pr && local_img->data[k][j] == 1) {
                        local_img->data[k][j] = label;
                        if (j - 1 >= 0 && local_img->data[k][j - 1]) {
                            int l = j - 1;
                            while (l >= 0 && local_img->data[k][l]) {
                                local_img->data[k][l] = label;
                                l--;
                            }
                        }
                        if (j + 1 < img->n && local_img->data[k][j + 1] == 1) {
                            int l = j + 1;
                            while (l < img->n && local_img->data[k][l]) {
                                local_img->data[k][l] = label;
                                l++;
                            }
                        }
                        k++;
                    }
                }
            } else {
                if (fl == 1) {
                    fl = 0;
                    label++;
                    label_count++;
                }
            }
        }
    }
    for (int i = 0; i < count_string_pr; i++) {
        for (int j = 0; j < img->n; j++) {
            local_image[i * img->n + j] = local_img->data[i][j];
        }
    }
    if (ProcRank == 0) {
        for (int i = 0; i < count_string_pr * img->n; i++) {
            global_image[i] = local_image[i];
        }
    }
    if (ProcRank != 0) {
        MPI_Send(&local_image[0], count_string_pr * img->n,
            MPI_INT, 0, 0, MPI_COMM_WORLD);
    } else {
        for (int i = 1; i < ProcNum; i++) {
            MPI_Recv(&global_image[i * img->n * delta + remainder * img->n], count_string_pr * img->n,
                MPI_INT, i, 0, MPI_COMM_WORLD, &status);
        }
    }
    MPI_Reduce(&label_count, &res, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    if (ProcRank == 0) {
        for (int i = 0; i < img->m; i++) {
            for (int j = 0; j < img->n; j++) {
                img->data[i][j] = global_image[i * img->n + j];
            }
        }
        fl = 0;
        for (int i = count_string_pr - 1; i < img->m - delta; i += delta) {
            for (int j = 0; j < img->n; j++) {
                if (img->data[i][j] != 0 && img->data[i + 1][j] != 0) {
                    fl = 1;
                    int k = i + 1;
                    while (k < img->m && img->data[k][j] != 0) {
                        img->data[k][j] = img->data[i][j];
                        if (j > 0 && img->data[k][j - 1] != 0) {
                            int l = j - 1;
                            while (l >= 0 && img->data[k][l] != 0) {
                                img->data[k][l] = img->data[i][j];
                                l--;
                            }
                        }
                        if (j < img->n - 1 && img->data[k][j + 1] != 0) {
                            int l = j + 1;
                            while (l < img->n && img->data[k][l] != 0) {
                                img->data[k][l] = img->data[i][j];
                                img->data[k][l] = img->data[i][j];
                                l++;
                            }
                        }
                        k++;
                    }
                } else {
                    if (fl == 1) {
                        fl = 0;
                        res--;
                    }
                }
            }
        }
    }
    img->count = res;
}
