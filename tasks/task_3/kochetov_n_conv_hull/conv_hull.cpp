// Copyright 2023 Kochetov Nikolay
#include <mpi.h>
#include <cmath>
#include <random>
#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include "task_3/kochetov_n_conv_hull/conv_hull.h"

int *vec_to_arr(std::vector<point> pnts_v, int prev_size = 0) {
    int *res = new int[((pnts_v.size() > prev_size) ?
        pnts_v.size() : prev_size) * 2];
    for (int i = 0; i < pnts_v.size(); i++) {
        res[i * 2] = pnts_v[i].x;
        res[(i * 2) + 1] = pnts_v[i].y;
    }
    if (pnts_v.size() < prev_size) {
        for (int i = pnts_v.size(); i < prev_size; i++) {
            res[i * 2] = -1;
            res[(i * 2) + 1] = -1;
        }
    }
    return res;
}

std::vector<point> arr_to_vec(int *pnts_arr, int size) {
    std::vector<point> res;
    for (int i = 0; i < size; i += 2) {
        if (pnts_arr[i] == -1) break;
        res.push_back(point{pnts_arr[i], pnts_arr[i + 1]});
    }
    return res;
}

bool ccw(point start, point a, point b) {
    int x1 = a.x - start.x;
    int y1 = a.y - start.y;
    int x2 = b.x - start.x;
    int y2 = b.y - start.y;
    if ((x1 * y2 - x2 * y1) > 0 || (x2 == 0 && y2 == 0)) {
        return true;
    } else {
        return false;
    }
}

double angle(point start, point a) {
    double x = a.x - start.x;
    double y = a.y - start.y;
    if (y == 0 && x == 0)
        return -2;
    return std::atan2(y, x);
}

int distance(point start, point a) {
    double x = a.x - start.x;
    double y = a.y - start.y;
    return abs(x) + abs(y);
}

bool longer(point start, point a, point b) {
    if (distance(start, a) > distance(start, b)) {
        return true;
    } else {
        return false;
    }
}

bool collinear(point start, point a, point b) {
    int x1 = a.x - start.x;
    int y1 = a.y - start.y;
    int x2 = b.x - start.x;
    int y2 = b.y - start.y;
    if (x1 * y2 - x2 * y1 == 0) {
        return true;
    } else {
        return false;
    }
}

bool pnt_greater(point start, point a, point b) {
    return (ccw(start, a, b) ||
        (collinear(start, a, b) && longer(start, a, b)));
}

bool pnt_comp(point start, point a, point b) {
    double angle_a = angle(start, a);
    double angle_b = angle(start, b);
    if (angle_a != angle_b) {
        return angle_a < angle_b;
    }
    return distance(start, a) < distance(start, b);
}

bool pnt_less_by_x(point a, point b) {
    if (a.x != b.x) {
        return a.x < b.x;
    }
    return a.y < b.y;
}

std::vector<point> graham_scan(std::vector<point> pnts) {
    std::vector<point> answer;

    if (pnts.size() != 0) {
        point min_pnt = *std::min_element(
            pnts.begin(), pnts.end(), pnt_less_by_x);

        std::sort(pnts.begin(), pnts.end(),
            [&](const point& a, const point& b) {
                return pnt_comp(min_pnt, a, b);
            });

        answer.push_back(min_pnt);

        for (int i = 1; i < pnts.size(); i++) {
            while (answer.size() > 1 && (!ccw(answer[answer.size() - 2],
                answer[answer.size() - 1], pnts[i])))
                answer.pop_back();
            answer.push_back(pnts[i]);
        }
    }

    return answer;
}

point find_tangent(point pnt, std::vector<point> hull) {
    if (hull.size() == 1) {
        return hull[0];
    }

    int a = 0;
    int b = hull.size();
    int curr = (a + b) / 2;
    int size = hull.size();

    while (!((pnt_greater(pnt, hull[curr % size], hull[(curr + 1) % size]) &&
        pnt_greater(pnt, hull[curr % size], hull[(curr - 1) % size])) ||
            ((b - a) == 1))) {
        //  a + 1 > a
        if (ccw(pnt, hull[(a + 1) % size], hull[a % size])) {
            //  c > c + 1
            if (ccw(pnt, hull[curr % size], hull[(curr + 1) % size])) {
                b = curr;
            //  c + 1 > c
            } else {
                //  c > a
                if (ccw(pnt, hull[curr % size], hull[a % size])) {
                    a = curr;
                //  a > c
                } else {
                    b = curr;
                }
            }
        //  a > a + 1
        } else {
            //  c + 1 > c
            if (ccw(pnt, hull[(curr + 1) % size], hull[curr % size])) {
                a = curr;
            //  c > c + 1
            } else {
                //  c > a
                if (ccw(pnt, hull[curr % size], hull[a % size])) {
                    b = curr;
                //  a > c
                } else {
                    a = curr;
                }
            }
        }
        curr = (a + b) / 2;
    }

    if ((b - a) == 1) {
        if (pnt_greater(pnt, hull[a % size], hull[b % size])) {
            return hull[a % size];
        } else {
            return hull[b % size];
        }
    }
    return hull[curr % size];
}

std::vector<point> find_hull(std::vector<std::vector<point>> cmpnts) {
    int ProcRank, ProcNum;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    std::vector<point> ans;

    //  parallel part ------------------------------------

    int compnts_amount = 0;

    if (ProcRank == 0) {
        compnts_amount = cmpnts.size();
        MPI_Bcast(&compnts_amount, 1, MPI_INT, 0, MPI_COMM_WORLD);
    } else {
        MPI_Bcast(&compnts_amount, 1, MPI_INT, 0, MPI_COMM_WORLD);
    }

    int *ProcSizes = new int[ProcNum]();
    int *ProcDisp = new int[ProcNum]();
    for (int i = 0; i < ProcNum; i++) {
        ProcSizes[i] = compnts_amount / ProcNum +
            (i < compnts_amount % ProcNum ? 1 : 0);
        ProcDisp[i] = i * (compnts_amount / ProcNum) +
            std::min<int>(i, compnts_amount % ProcNum);
    }

    if (compnts_amount == 0)
        return ans;

    int *compnts_sizes = new int[compnts_amount]();

    if (ProcRank == 0) {
        for (int i = 0; i < cmpnts.size(); i++) {
            compnts_sizes[i] = cmpnts[i].size() * 2;
        }
        MPI_Bcast(compnts_sizes, compnts_amount, MPI_INT, 0, MPI_COMM_WORLD);
    } else {
        MPI_Bcast(compnts_sizes, compnts_amount, MPI_INT, 0, MPI_COMM_WORLD);
    }

    int* ProcElemSizes = new int[ProcNum]();
    int* ProcElemDisp = new int[ProcNum]();
    int disp = 0;
    for (int i = 0; i < ProcNum; i++) {
        ProcElemDisp[i] = disp;
        int size = 0;
        for (int j = 0; j < ProcSizes[i]; j++) {
            size += compnts_sizes[ProcDisp[i] + j];
        }
        disp += size;
        ProcElemSizes[i] = size;
    }

    int *components = new int[ProcElemSizes[ProcRank]]();

    //  sending components to processors
    int* all_compnt_arr = nullptr;
    int ints_amount = 0;
    if (ProcRank == 0) {
        std::vector<point> all_compnt;
        for (int i = 0; i < cmpnts.size(); i++) {
            all_compnt.insert(all_compnt.end(),
                cmpnts[i].begin(), cmpnts[i].end());
        }
        ints_amount = all_compnt.size() * 2;

        int* all_compnt_arr = vec_to_arr(all_compnt);


        MPI_Scatterv(all_compnt_arr, ProcElemSizes, ProcElemDisp, MPI_INT,
            components, ProcElemSizes[ProcRank], MPI_INT, 0, MPI_COMM_WORLD);
        delete[] all_compnt_arr;
    } else {
        MPI_Scatterv(nullptr, nullptr, nullptr, MPI_INT,
            components, ProcElemSizes[ProcRank],
            MPI_INT, 0, MPI_COMM_WORLD);
    }

    std::vector<std::vector<point>> local_comp_vec;

    std::vector<point> tmp_vec;
    int* tmp_compnt = nullptr;
    disp = 0;
    for (int i = 0; i < ProcSizes[ProcRank]; i++) {
        tmp_compnt = new int[compnts_sizes[ProcDisp[ProcRank] + i]]();
        for (int j = 0; j < compnts_sizes[ProcDisp[ProcRank] + i]; j++) {
            tmp_compnt[j] = components[disp + j];
        }
        disp += compnts_sizes[ProcDisp[ProcRank] + i];
        local_comp_vec.push_back(arr_to_vec(tmp_compnt,
            compnts_sizes[ProcDisp[ProcRank] + i]));
        delete[] tmp_compnt;
    }

    //  find hulls for each component
    std::vector<std::vector<point>> local_sub_hulls;
    for (int i = 0; i < local_comp_vec.size(); i++) {
        local_sub_hulls.push_back(graham_scan(local_comp_vec[i]));
    }

    //  collecting data on root processor
    int* tmp_arr_pnts = nullptr;
    disp = 0;
    for (int i = 0; i < local_sub_hulls.size(); i++) {
        tmp_arr_pnts = vec_to_arr(local_sub_hulls[i],
            compnts_sizes[ProcDisp[ProcRank] + i]);

        for (int j = disp; j < disp +
            compnts_sizes[ProcDisp[ProcRank] + i]; j++) {
            components[j] = tmp_arr_pnts[j - disp];
        }
        disp += compnts_sizes[ProcDisp[ProcRank] + i];
        delete[] tmp_arr_pnts;
    }

    std::vector<std::vector<point>> sub_hulls;
    if (ProcRank == 0) {
        all_compnt_arr = new int[ints_amount]();

        MPI_Gatherv(components, ProcElemSizes[ProcRank],
            MPI_INT, all_compnt_arr,
            ProcElemSizes, ProcElemDisp, MPI_INT, 0, MPI_COMM_WORLD);

        disp = 0;
        int* tmp_arr_pnts = nullptr;
        for (int i = 0; i < compnts_amount; i++) {
            tmp_arr_pnts = new int[compnts_sizes[i]]();
            for (int j = disp; j < compnts_sizes[i] + disp; j++) {
                tmp_arr_pnts[j - disp] = all_compnt_arr[j];
            }
            disp += compnts_sizes[i];
            sub_hulls.push_back(arr_to_vec(tmp_arr_pnts, compnts_sizes[i]));
        }
    } else {
        MPI_Gatherv(components, ProcElemSizes[ProcRank], MPI_INT, nullptr,
            ProcElemSizes, ProcElemDisp, MPI_INT, 0, MPI_COMM_WORLD);
    }

    if (ProcRank != 0) {
        return ans;
    }

    //  end of parallel part ------------------------------------

    //  find min point
    point start_pnt{-1, -1};
    for (int i = 0; i < sub_hulls.size(); i++) {
        point tmp_pnt = *std::min_element(sub_hulls[i].begin(),
            sub_hulls[i].end(), pnt_less_by_x);
        if (start_pnt.x == -1 || pnt_less_by_x(tmp_pnt, start_pnt)) {
            start_pnt = tmp_pnt;
        }
    }
    ans.push_back(start_pnt);

    //  connect sub-hulls
    point new_pnt{-1, -1};
    while (true) {
        new_pnt = {-1, -1};
        for (int i = 0; i < sub_hulls.size(); i++) {
            point tmp_pnt = find_tangent(ans.back(), sub_hulls[i]);
            if (new_pnt.x == -1 || pnt_greater(ans.back(), tmp_pnt, new_pnt)) {
                new_pnt = tmp_pnt;
            }
        }
        if (new_pnt == ans[0]) {
            return ans;
        }
        if (ans.size() > 1 && (collinear(ans[ans.size() - 2],
            new_pnt, ans[ans.size() - 1]) &&
            longer(ans[ans.size() - 2], new_pnt, ans[ans.size() - 1]))) {
            ans[ans.size() - 1] = new_pnt;
        } else {
            ans.push_back(new_pnt);
        }
    }
}

std::vector<std::vector<point>> marking(int *matrix, int n) {
    int comp_id = 2;
    int zero = 0;
    int *marked_image = new int[n * n]();
    for (int row = 0; row < n; row++) {
        for (int elem = 0; elem < n; elem++) {
            marked_image[row * n + elem] = matrix[row * n + elem];
        }
    }

    //  forward step
    for (int row = 0; row < n; row++) {
        for (int elem = 0; elem < n; elem++) {
            //  for readability
            int *current = &marked_image[(row * n) + elem];
            int *left = (elem == 0 ?
                &zero : &marked_image[(row * n) + (elem - 1)]);
            int *upper = (row == 0 ?
                &zero : &marked_image[((row - 1) * n) + elem]);

            //  empty cell
            if (*current == 0)
                continue;

            if (*left == 0 && *upper == 0) {
                //  new component
                *current = comp_id++;
            } else {
                //  part of existing component
                if (*left > *upper) {
                    *current = *left;
                } else {
                    *current = *upper;
                }
            }
        }
    }

    //  backward step
    for (int row = n - 1; row > -1; row--) {
        for (int elem = n - 1; elem > -1; elem--) {
            //  for readability
            int *current = &marked_image[(row * n) + elem];
            int *right = (elem == n - 1 ?
                &zero : &marked_image[(row * n) + (elem + 1)]);
            int *bottom = (row == n - 1 ?
                &zero : &marked_image[((row + 1) * n) + elem]);

            //  empty cell
            if (*current == 0 || (*right == 0 && *bottom == 0))
                continue;

            //  part of existing component
            if (*right > *bottom) {
                *current = *right;
            } else {
                *current = *bottom;
            }
        }
    }

    std::map<int, int> idxs;
    std::vector<std::vector<point>> ans;
    int components_count = 0;

    for (int row = 0; row < n; row++) {
        for (int elem = 0; elem < n; elem++) {
            if (marked_image[(row * n) + elem] == 0)
                continue;
            if (idxs.find(marked_image[(row * n) + elem]) != idxs.end()) {
                ans[idxs[marked_image[(row * n) +
                    elem]]].push_back(point{elem, row});
            } else {
                idxs.insert({marked_image[(row * n) + elem],
                    components_count++});
                std::vector<point> vec = {point{elem, row}};
                ans.push_back(vec);
            }
        }
    }

    return ans;
}
