// Copyright 2023 Videneva Ekaterina
#include "task_3/videneva_e_search_space/search_space.h"

double GetGlobalOptimization(const double i, const double j,
    std::function<double(double*)> func, const int part, const double e) {
    std::vector<double> answ;
    int MaxNum = 1;
    double l = 2, L;

    answ.push_back(i);
    answ.push_back(j);
    double M = std::abs(func(&answ.at(1)) - func(&answ.at(0))) / (answ.at(1) - answ.at(0)), m;
    m = (M > 0) ? l * M : 1;

    answ.push_back((answ.at(1) + answ.at(0)) / 2 - (func(&answ.at(1)) - func(&answ.at(0))) / (2 * m));
    int num = answ.size() - 1;

    while (num < part) {
        sort(answ.begin(), answ.begin() + answ.size());

        M = std::abs(func(&answ.at(1)) - func(&answ.at(0))) / (answ.at(1) - answ.at(0));
        for (int i = 2; i <= num; i++) {
            M = std::max(M, std::abs(func(&answ.at(i)) - func(&answ.at(i - 1))) / (answ.at(i) - answ.at(i - 1)));
        }

        m = (M > 0) ? l * M : 1;

        L = m * (answ.at(1) - answ.at(0)) +
            pow((func(&answ.at(1)) - func(&answ.at(0))), 2) / (m * (answ.at(1) - answ.at(0))) -
            2 * (func(&answ.at(1)) + func(&answ.at(0)));
        MaxNum = 1;
        for (int i = 2; i <= num; ++i) {
            double Max =
                m * (answ.at(i) - answ.at(i - 1)) +
                std::pow((func(&answ.at(i)) - func(&answ.at(i - 1))), 2) / (m * (answ.at(i) - answ.at(i - 1))) -
                2 * (func(&answ.at(i)) + func(&answ.at(i - 1)));
            if (L < Max) {
                L = Max;
                MaxNum = i;
            }
        }

        answ.push_back((answ.at(MaxNum) + answ.at(MaxNum - 1)) / 2 -
            (func(&answ.at(MaxNum)) - func(&answ.at(MaxNum - 1))) / (2 * m));
        num++;
        if (answ.at(MaxNum) - answ.at(MaxNum - 1) < e) {
            break;
        }
    }
    return answ.at(MaxNum);
}

double GetGlobalOptimizationParallel(const double i, const double j, std::function<double(double*)> func,
    const int part, const double e) {
    int Size;
    int Rank;

    MPI_Comm_size(MPI_COMM_WORLD, &Size);
    MPI_Comm_rank(MPI_COMM_WORLD, &Rank);

    if (Size == 1) {
        return GetGlobalOptimization(i, j, func, part, e);
    }

    std::vector<double> answ(Size);
    double f = i + ((j - i) / Size) * Rank;
    double s = f + (j - i) / Size;

    double LocalOpt = GetGlobalOptimization(f, s, func, part, e);
    MPI_Gather(&LocalOpt, 1, MPI_DOUBLE, &answ.at(0), 1, MPI_DOUBLE, 0,
        MPI_COMM_WORLD);

    if (Rank == 0) {
        double tmp = func(&answ.at(0));
        for (int i = 1; i < Size; ++i) {
            if (func(&answ.at(0) + i) < tmp) {
                std::swap(answ.at(i), answ.at(0));
                tmp = func(&answ.at(0) + i);
            }
        }
    }
    return answ.at(0);
}