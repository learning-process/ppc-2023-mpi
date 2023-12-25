// Copyright 2023 Kulaev Eugene
#include "task_3/kulaev_e_global_search/glob_search.h"

double GetGlobalOpt(const double a, const double b,
    std::function<double(double*)> func, const int part, const double e) {
    std::vector<double> answ;
    int MaxCount = 1;
    double r = 2, R;

    answ.push_back(a);
    answ.push_back(b);
    double M = std::abs(func(&answ.at(1)) - func(&answ.at(0))) / (answ.at(1) - answ.at(0)), m;
    m = (M > 0) ? r * M : 1;

    answ.push_back((answ.at(1) + answ.at(0)) / 2 - (func(&answ.at(1)) - func(&answ.at(0))) / (2 * m));
    int count = answ.size() - 1;

    while (count < part) {
        sort(answ.begin(), answ.begin() + answ.size());

        M = std::abs(func(&answ.at(1)) - func(&answ.at(0))) / (answ.at(1) - answ.at(0));
        for (int i = 2; i <= count; i++) {
            M = std::max(M, std::abs(func(&answ.at(i)) - func(&answ.at(i - 1))) / (answ.at(i) - answ.at(i - 1)));
        }

        m = (M > 0) ? r * M : 1;

        R = m * (answ.at(1) - answ.at(0)) +
            pow((func(&answ.at(1)) - func(&answ.at(0))), 2) / (m * (answ.at(1) - answ.at(0))) -
            2 * (func(&answ.at(1)) + func(&answ.at(0)));
        MaxCount = 1;
        for (int i = 2; i <= count; ++i) {
            double Max =
                m * (answ.at(i) - answ.at(i - 1)) +
                std::pow((func(&answ.at(i)) - func(&answ.at(i - 1))), 2) / (m * (answ.at(i) - answ.at(i - 1))) -
                2 * (func(&answ.at(i)) + func(&answ.at(i - 1)));
            if (R < Max) {
                R = Max;
                MaxCount = i;
            }
        }

        answ.push_back((answ.at(MaxCount) + answ.at(MaxCount - 1)) / 2 -
            (func(&answ.at(MaxCount)) - func(&answ.at(MaxCount - 1))) / (2 * m));
        count++;
        if (answ.at(MaxCount) - answ.at(MaxCount - 1) < e) {
            break;
        }
    }
    return answ.at(MaxCount);
}

double GetGlobalOptParallel(const double a, const double b, std::function<double(double*)> func,
    const int part, const double e) {
    int ProcCount;
    int ProcRank;

    MPI_Comm_size(MPI_COMM_WORLD, &ProcCount);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    if (ProcCount == 1) {
        return GetGlobalOpt(a, b, func, part, e);
    }

    std::vector<double> answ(ProcCount);
    double f = a + ((b - a) / ProcCount) * ProcRank;
    double s = f + (b - a) / ProcCount;

    double LocalOpt = GetGlobalOpt(f, s, func, part, e);
    MPI_Gather(&LocalOpt, 1, MPI_DOUBLE, &answ.at(0), 1, MPI_DOUBLE, 0,
        MPI_COMM_WORLD);

    if (ProcRank == 0) {
        double beg = func(&answ.at(0));
        for (int i = 1; i < ProcCount; ++i) {
            if (func(&answ.at(0) + i) < beg) {
                std::swap(answ.at(i), answ.at(0));
                beg = func(&answ.at(0) + i);
            }
        }
    }
    return answ.at(0);
}
