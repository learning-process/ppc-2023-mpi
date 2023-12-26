// Copyright 2023 Saratova Marina
#include "task_3/saratova_m_quick_sort/quick_sort.h"

bool compare_vectors(const std::vector<int>& vec1,
                              const std::vector<int>& vec2) {
    if (vec1.size() != vec2.size()) {
        return false;
    }
    for (size_t i = 0; i < vec1.size(); ++i) {
        if (vec1[i] != vec2[i]) {
            return false;
        }
    }
    return true;
}

std::vector<int> seq_sorting(const std::vector<int>& vec) {
  std::vector<int> seq_sorting_result(vec);
  std::sort(seq_sorting_result.begin(), seq_sorting_result.end());
  return seq_sorting_result;
}

void generate_random_values(std::vector<int> *vec, int min_value,
                                                     int max_value) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::uniform_int_distribution<int> distrib(min_value, max_value);

    for (int i = 0; i < vec->size(); ++i) {
        vec->at(i) = distrib(gen);
    }
}

std::vector<int> task_distribution(const int num_of_proc,
                                       const int num_of_task) {
  std::vector<int> task_per_proc(num_of_proc);
  int remained_rows = num_of_task % num_of_proc;
  for (int proc = 0; proc < num_of_proc; ++proc) {
    task_per_proc.at(proc) = num_of_task / num_of_proc + (remained_rows > 0);
    --remained_rows;
  }
  return task_per_proc;
}

std::vector<int> par_quick_sort(const std::vector<int> &vec) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const int elems_num = vec.size();
  if (elems_num == 0) {
    return std::vector<int>();
  } else if (elems_num == 1) {
    return std::vector<int>({vec.at(0)});
  }

  std::vector<int> elems_per_process = task_distribution(size, elems_num);
  if (elems_per_process.at(rank) == 0) {
    return std::vector<int>{};
  }

  std::vector<int> sorted_vec(elems_per_process[rank]);
  if (rank == 0) {
    int elems_to_ignore = elems_per_process.at(0);
    for (int proc = 1; proc < size; ++proc) {
      if (elems_per_process.at(proc) != 0) {
        MPI_Send(const_cast<int *>(&vec.at(elems_to_ignore)),
        elems_per_process.at(proc), MPI_INT, proc, 0, MPI_COMM_WORLD);
      }
      elems_to_ignore += elems_per_process.at(proc);
    }
  }

  std::vector<int> local_vec(elems_per_process.at(rank));
  if (rank == 0) {
    local_vec = std::vector<int>(vec.begin(),
    vec.begin() + elems_per_process.at(rank));
  } else {
    MPI_Status status;
    MPI_Recv(local_vec.data(), elems_per_process.at(rank), MPI_INT, 0, 0,
    MPI_COMM_WORLD, &status);
  }

  for (int i = 0; i < elems_per_process.at(rank); ++i) {
    std::sort(local_vec.begin(), local_vec.end());
  }

  if (rank == 0) {
    MPI_Status status;
    std::queue<std::shared_ptr<std::vector<int>>> ptr_queue;
    ptr_queue.push(std::make_shared<std::vector<int>>(local_vec));

    for (int proc = 1; proc < size; ++proc) {
      if (elems_per_process.at(proc) != 0) {
        ptr_queue.push(std::make_shared<std::vector<int>>
                                 (elems_per_process.at(proc)));
        MPI_Recv(ptr_queue.back()->data(), ptr_queue.back()->size(), MPI_INT,
                 proc, 0, MPI_COMM_WORLD, &status);
      }
    }

    while (ptr_queue.size() != 1) {
      std::shared_ptr<std::vector<int>> f_v_ptr = ptr_queue.front();
      ptr_queue.pop();
      std::shared_ptr<std::vector<int>> s_v_ptr = ptr_queue.front();
      ptr_queue.pop();
      auto temp =
          std::make_shared<std::vector<int>>(f_v_ptr->size() + s_v_ptr->size());
      std::merge(f_v_ptr->begin(), f_v_ptr->end(), s_v_ptr->begin(),
                 s_v_ptr->end(), temp->begin());
      ptr_queue.push(temp);
    }
    return *ptr_queue.back();
  } else {
    MPI_Send(local_vec.data(), local_vec.size(), MPI_INT, 0, 0, MPI_COMM_WORLD);
  }
  return local_vec;
}
