// Copyright 2023 Trubin Artem

#include "task_3/trubin_a_bin_img/bin_img.h"

void labeling(image* img) {
  int rank, size, label, res = 0, label_count = 0, string_count, new_string_count, rest;
  bool flag = 0;
  int* global_img_arr, *local_img_arr;
  image* local_img;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Status status;
  string_count = img->m / size;
  rest = img->m % size;

  if (rank == 0) {
    new_string_count = string_count + rest;
  } else {
    new_string_count = string_count;
  }

  label = rank * 100 + 2;

  // alloc mem for recv buf & local_img & res_buf
  global_img_arr = new int[img->m * img->n];
  local_img_arr = new int[new_string_count * img->n];
  local_img = new image(new_string_count, img->n);

  // clone img matrix values into arr & send parts to other ranks
  if (rank == 0) {
    for (int i = 0; i < img->m; i++) {
      for (int j = 0; j < img->n; j++) {
        global_img_arr[i * img->n + j] = img->data[i][j];
      }
    }

    // put first part of img arr in 0-rank's local img
    for (int i = 0; i < (new_string_count) * img->n; i++) {
      local_img_arr[i] = global_img_arr[i];
    }

    // send other parts of img arr to other ranks
    for (int i = 1; i < size; i++) {
      MPI_Send(&global_img_arr[i * img->n * string_count + rest * img->n], string_count * img->n,
        MPI_INT, i, 0, MPI_COMM_WORLD);
    }
  } else {
    MPI_Recv(&local_img_arr[0], string_count * img->n, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
  }

  // clone part of img arr into cur rank's img
  for (int i = 0; i < new_string_count; i++) {
    for (int j = 0; j < img->n; j++) {
      local_img->data[i][j] = local_img_arr[i * img->n + j];
    }
  }

  // img labeling
  for (int i = 0; i < local_img->m; i++) {
    for (int j = 0; j < img->n; j++) {
      if (local_img->data[i][j] == 1) {
        flag = 1;
        local_img->data[i][j] = label;
        if (i + 1 < new_string_count && local_img->data[i + 1][j] == 1) {
          int k = i + 1;
          while (k < new_string_count && local_img->data[k][j] == 1) {
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
        if (flag) {
          flag = 0;
          label++;
          label_count++;
        }
      }
    }
  }

  // now clone cur rank's img back to img arr
  for (int i = 0; i < new_string_count; i++) {
    for (int j = 0; j < img->n; j++) {
      local_img_arr[i * img->n + j] = local_img->data[i][j];
    }
  }

  // put 0-ranks's local arr into global img arr
  if (rank == 0) {
    for (int i = 0; i < new_string_count * img->n; i++) {
      global_img_arr[i] = local_img_arr[i];
    }
  }

  // then send local img arrs to 0-rank
  if (rank != 0) {
      MPI_Send(&local_img_arr[0], new_string_count * img->n,
        MPI_INT, 0, 0, MPI_COMM_WORLD);
  } else {
    for (int i = 1; i < size; i++) {
      MPI_Recv(&global_img_arr[i * img->n * string_count + rest * img->n], new_string_count * img->n,
        MPI_INT, i, 0, MPI_COMM_WORLD, &status);
    }
  }

  MPI_Reduce(&label_count, &res, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  // now build new img
  if (rank == 0) {
    for (int i = 0; i < img->m; i++) {
      for (int j = 0; j < img->n; j++) {
        img->data[i][j] = global_img_arr[i * img->n + j];
      }
    }

    // borders
    flag = 0;
    for (int i = new_string_count - 1; i < img->m - string_count; i += string_count) {
      for (int j = 0; j < img->n; j++) {
        if (img->data[i][j] != 0 && img->data[i + 1][j] != 0) {
          flag = 1;
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
          if (flag) {
            flag = 0;
            res--;
          }
        }
      }
    }
  }
  img->count = res;
}
