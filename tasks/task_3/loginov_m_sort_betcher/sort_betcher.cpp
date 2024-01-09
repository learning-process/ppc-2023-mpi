// Copyright 2023 by Loginov Maxim
#include "task_3/loginov_m_sort_betcher/sort_betcher.h"

std::vector<std::pair<int, int>> proc_allocation;

std::vector<int> vec_gen(int size) {
    std::mt19937 rand_r(time(0));
    std::vector<int> vec(size);
    for (int i = 0; i < size; i++) {
        vec[i] = rand_r() % 1234;
    }
    return vec;
}

int partition(std::vector<int>* vec, int l, int r) {
    int pivot = (*vec)[r];
    int i = (l - 1);
    int tmp = 0;
    for (int j = l; j <= r - 1; j++) {
        if ((*vec)[j] <= pivot) {
            i++;
            tmp = (*vec)[i];
            (*vec)[i] = (*vec)[j];
            (*vec)[j] = tmp;
        }
    }
    std::swap((*vec)[i + 1], (*vec)[r]);
    return (i + 1);
}

void q_sort(std::vector<int>* vec, int l, int r) {
    if (l < r) {
        int pi = partition(vec, l, r);
        q_sort(vec, l, pi - 1);
        q_sort(vec, pi + 1, r);
    }
}

void _allocation(std::vector<int> vec) {
    int size = vec.size();
    if (size <= 1)
        return;
    std::vector<int> l(vec.begin(), vec.begin() + size / 2);
    std::vector<int> r(vec.begin() + size / 2, vec.end());
    _allocation(l);
    _allocation(r);
    odd_even_merge(l, r);
}

void odd_even_merge(std::vector<int> l, std::vector<int> r) {
    int size = l.size() + r.size();
    int i1 = 0;
    std::vector<int> l_odd;
    std::vector<int> l_even;
    std::vector<int> r_odd;
    std::vector<int> r_even;
    std::vector<int> res;
    if (size <= 1) {
        return;
    }
    if (size == 2) {
        proc_allocation.push_back(std::pair<int, int>(l[0], r[0]));
        return;
    }
    for (unsigned int i = 0; i < l.size(); i++) {
        if (i % 2) {
            l_odd.push_back(l[i]);
        } else {
            l_even.push_back(l[i]);
        }
    }
    for (unsigned int i = 0; i < r.size(); i++) {
        if (i % 2) {
            r_odd.push_back(r[i]);
        } else {
            r_even.push_back(r[i]);
        }
    }
    odd_even_merge(l_odd, r_odd);
    odd_even_merge(l_even, r_even);
    for (unsigned int i = 0; i < l.size(); i++) {
        res.push_back(l[i]);
    }
    for (unsigned int i = 0; i < r.size(); i++) {
        res.push_back(r[i]);
    }
    for (int i = 1; i < size - 1; i += 2) {
        i1 = i + 1;
        proc_allocation.push_back(std::pair<int, int>(res[i], res[i1]));
    }
}

void create_allocation(int _proc_num) {
    std::vector<int> vec;
    for (int i = 0; i < _proc_num; i++) {
        vec.push_back(i);
    }
    _allocation(vec);
}

void q_sort_batcher_par(std::vector<int>* vec) {
    int proc_num;
    int num_of_proc;
    int count = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &num_of_proc);
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_num);
    if (static_cast<unsigned int>(num_of_proc) >= vec->size()) {
        if (proc_num == 0) {
            q_sort(vec, 0, vec->size() - 1);
        }
        return;
    }
    create_allocation(num_of_proc);
    if (proc_num == 0) {
        while (true) {
            if (vec->size() % num_of_proc) {
                count++;
                vec->push_back(1337);
            } else {
                break;
            }
        }
    }
    int enlarged_size = vec->size();
    MPI_Bcast(&enlarged_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
    int size_of_part = enlarged_size / num_of_proc;
    std::vector<int> part(size_of_part);
    std::vector<int> tmp(size_of_part);
    std::vector<int>
         near_part(size_of_part);
    MPI_Scatter(&(*vec)[0], size_of_part,
         MPI_INT, &part[0], size_of_part, MPI_INT, 0, MPI_COMM_WORLD);
    q_sort(&part, 0, part.size() - 1);
    for (unsigned int i = 0; i < proc_allocation.size(); i++) {
        if (proc_num == proc_allocation[i].second) {
            MPI_Status status;
            MPI_Recv(&near_part[0], size_of_part,
                 MPI_INT, proc_allocation[i].first, 0, MPI_COMM_WORLD, &status);
            MPI_Send(&part[0], size_of_part,
                 MPI_INT, proc_allocation[i].first, 0, MPI_COMM_WORLD);
            int start = size_of_part - 1;
            int current_pos = start;
            int nearby_pos = start;
            for (int j = start; j >= 0; j--) {
                int current = part[current_pos];
                int nearby = near_part[nearby_pos];
                if (current > nearby) {
                    tmp[j] = current;
                    current_pos--;
                } else {
                    tmp[j] = nearby;
                    nearby_pos--;
                }
            }
            part = tmp;
        } else if (proc_num == proc_allocation[i].first) {
            MPI_Status status;
            MPI_Send(&part[0], size_of_part,
                 MPI_INT, proc_allocation[i].second, 0, MPI_COMM_WORLD);
            MPI_Recv(&near_part[0], size_of_part, MPI_INT,
                 proc_allocation[i].second, 0, MPI_COMM_WORLD, &status);
            int current_pos = 0;
            int nearby_pos = 0;
            for (int j = 0; j < size_of_part; j++) {
                int current = part[current_pos];
                int nearby = near_part[nearby_pos];
                if (current < nearby) {
                    tmp[j] = current;
                    current_pos++;
                } else {
                    tmp[j] = nearby;
                    nearby_pos++;
                }
            }
            part = tmp;
        }
    }
    MPI_Gather(&part[0], size_of_part, MPI_INT,
         &(*vec)[0], size_of_part, MPI_INT, 0, MPI_COMM_WORLD);

    if (proc_num == 0 && count > 0)
        vec->erase(vec->begin() + enlarged_size - count, vec->end());
    return;
}
