// Copyright 2023 Veselov Mihail
#include <mpi.h>
#include <stdexcept>
#include <set>
#include <random>
#include "task_3/veselov_m_glob_opt_by_divid_area/glob_opt_by_divid_area.h"

double f1(double x, double y) {
    return std::pow(x, 2) + std::pow(y - 1, 2);  // 0 0 0
}

double f2(double x, double y) {
    return 4 + std::pow(std::pow(std::pow(x, 2) + std::pow(y, 2), 2), 1.0/3);  // 0 0 4
}

double f3(double x, double y) {  // 1 1/2 4
    return std::pow(x, 3) + 8*std::pow(y, 3) - 6*x*y + 5;
}
double f4(double x, double y) {  // no min
    return y*sqrt(x) - 2*std::pow(y, 2) - x + 14*y;
}
double f5(double x, double y) {
    return x + 4*y - 6 - 2*log(x*y) - 3*log(y);  // 2 1.25 z = -2*ln(5/2)-3*ln(5/4)+1 = -1.5
}

double f6(double x, double y) {
    return 2*sin(x) + cos(y);  // 3*pi/2 pi -3
}

resultTwoVar solveTwoVar(const double& _a1, const double& _b1, const double& _a2, const double& _b2,
double(*func)(double x, double y), const double& _eps, const int& _Nmax, const double& _epsOneVar,
const int& _NmaxOneVar, const double& _r_par) {
    if (_a1 > _b1)
        throw "A1 > B1";
    if (_a2 > _b2)
        throw "A2 > B2";
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (size < 2)
        return solveTwoVarSequential(_a1, _b1, _a2, _b2, func);
    MPI_Status status;
    resultTwoVar finalRes = {0, 0, 0};
    resultTwoVar res;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        std::set<setElemTwoVar> set;
        int k;
        if (std::abs(_a1 - _b1) <= 0.0001 || std::abs(_a2 - _b2) <= 0.0001) {
            k = size + 1;
        } else {
            k = size;
        }
        double segmentLen = (_b1 - _a1) / (k - 1);
        for (int i = 0; i < size - 1; ++i) {
            double Xf = _a1 + i * segmentLen;
            MPI_Send(&Xf, 1, MPI_DOUBLE, i+1, 1, MPI_COMM_WORLD);
        }
        res = solveOneVar(_a2, _b2, _b1, func, _epsOneVar);
        set.insert(setElemTwoVar(res.x, res.y, res.z));
        finalRes = res;
        if (k != size) {
            res = solveOneVar(_a2, _b2, _a1 + segmentLen * size, func, _epsOneVar);
            set.insert(setElemTwoVar(res.x, res.y, res.z));
            if (res.z < finalRes.z)
                finalRes = res;
        }
        for (int i = 0; i < size - 1; ++i) {
            MPI_Recv(&res, 3, MPI_DOUBLE, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);
            if (res.z < finalRes.z)
                finalRes = res;
            set.insert(setElemTwoVar(res.x, res.y, res.z));
        }
        std::set<setElemR> setR;
        double M, currM, m, currR, newX;
        bool terminate = false;
        while (!terminate && k < _Nmax) {
            setR.clear();
            M = -1;
            auto iter = set.begin();
            iter++;
            auto prevIter = set.begin();
            while (iter != set.end()) {
                currM = std::abs(static_cast<double>((iter->z - prevIter->z) / (iter->x - prevIter->x)));
                if (currM > M)
                    M = currM;
                iter++;
                prevIter++;
            }
            if (M > 0)
                m = _r_par * M;
            else
                m = 1;
            iter = set.begin();
            iter++;
            prevIter = set.begin();
            while (iter != set.end()) {
                currR = m*(iter->x - prevIter->x) + (std::pow((iter->z - prevIter->z), 2) /
                (m * (iter->x - prevIter->x))) - 2 * (iter->z - prevIter->z);
                setR.insert(setElemR(currR, iter->x, iter->z, prevIter->x, prevIter->z));
                iter++;
                prevIter++;
            }
            auto itR = setR.begin();
            for (int i = 0; i < size - 1; ++i) {
                k++;
                newX = (0.5)*(itR->x + itR->xPrev) - ((itR->z - itR->zPrev) / (2 * m));
                MPI_Send(&newX, 1, MPI_DOUBLE, i+1, 1, MPI_COMM_WORLD);
                if (itR->x - itR->xPrev <= _eps) {
                    terminate = true;
                }
                itR++;
            }
            for (int i = 0; i < size - 1; ++i) {
                MPI_Recv(&res, 3, MPI_DOUBLE, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);
                if (res.z < finalRes.z)
                    finalRes = res;
                set.insert(setElemTwoVar(res.x, res.y, res.z));
            }
        }
        for (int i = 0; i < size - 1; ++i) {
            double terminate = _eps*0.001;
            MPI_Send(&terminate, 1, MPI_DOUBLE, i+1, 1, MPI_COMM_WORLD);
        }
    } else {
        bool terminate = false;
        while (!terminate) {
            double mes;
            MPI_Recv(&mes, 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, &status);
            if (mes == _eps*0.001) {
                terminate = true;
            } else {
                res = solveOneVar(_a2, _b2, mes, func, _epsOneVar, _NmaxOneVar);
                MPI_Send(&res, 3, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
            }
        }
    }
    return finalRes;
}

resultTwoVar solveTwoVarSequential(const double& _a1, const double& _b1, const double& _a2, const double& _b2,
double(*func)(double x, double y), const double& _eps, const int& _Nmax, const double& _epsOneVar,
const int& _NmaxOneVar, const double& _r_par) {
    if (_a1 > _b1)
        throw "A1 > B1";
    if (_a2 > _b2)
        throw "A2 > B2";
    resultTwoVar finalRes = {0, 0, 0};
    resultTwoVar res;
    std::set<setElemTwoVar> set;
    res = solveOneVar(_a2, _b2, _a1, func, _epsOneVar, _NmaxOneVar);
    set.insert(setElemTwoVar(res.x, res.y, res.z));
    finalRes = res;
    res = solveOneVar(_a2, _b2, _b1, func, _epsOneVar, _NmaxOneVar);
    set.insert(setElemTwoVar(res.x, res.y, res.z));
    if (res.z < finalRes.z)
        finalRes = res;
    double M, currM, m, currR, newX;
    int k = 2;
    std::set<setElemR> setR;
    bool terminate = false;
    while (!terminate && k < _Nmax) {
        setR.clear();
        M = -1;
        auto iter = set.begin();
        iter++;
        auto prevIter = set.begin();
        while (iter != set.end()) {
            currM = std::abs(static_cast<double>((iter->z - prevIter->z) / (iter->x - prevIter->x)));
            if (currM > M)
                M = currM;
            iter++;
            prevIter++;
        }
        if (M > 0)
            m = _r_par * M;
        else
            m = 1;
        iter = set.begin();
        iter++;
        prevIter = set.begin();
        while (iter != set.end()) {
            currR = m*(iter->x - prevIter->x) + (std::pow((iter->z - prevIter->z), 2) /
            (m * (iter->x - prevIter->x))) - 2 * (iter->z - prevIter->z);
            setR.insert(setElemR(currR, iter->x, iter->z, prevIter->x, prevIter->z));
            iter++;
            prevIter++;
        }
        k++;
        auto Riter = setR.begin();
        newX = (0.5)*(Riter->x + Riter->xPrev) - ((Riter->z - Riter->zPrev) / (2 * m));
        res = solveOneVar(_a2, _b2, newX, func, _epsOneVar, _NmaxOneVar);
        set.insert(setElemTwoVar(res.x, res.y, res.z));
        if (res.z < finalRes.z)
            finalRes = res;
        if (Riter->x - Riter->xPrev <= _eps)
            terminate = true;
    }
    return finalRes;
}

resultTwoVar solveOneVar(const double& _a, const double& _b, const double& Xf, double(*func)(double x, double y),
const double& _eps, const int& _N_max, const double& _r_par) {
    if (_a > _b)
        throw "A > B";
    bool timeToStop = false;
    std::set<setElemOneVar> set;
    resultTwoVar res;
    res.x = Xf;
    double M, currM, m, R, currR, newX;
    double funcTmp = func(Xf, _a);
    set.insert(setElemOneVar(_a, funcTmp));
    res.y = _a;
    res.z = funcTmp;
    funcTmp = func(Xf, _b);
    set.insert(setElemOneVar(_b, funcTmp));
    if (res.z > funcTmp) {
        res.y = _b;
        res.z = funcTmp;
    }
    int k = 2;
    auto maxRiter = set.begin();
    auto maxPrevRiter = set.begin();
    while (!timeToStop && k < _N_max) {
        M = -1;
        auto iter = set.begin();
        iter++;
        auto prevIter = set.begin();
        while (iter != set.end()) {
            currM = std::abs(static_cast<double>((iter->y - prevIter->y) / (iter->x - prevIter->x)));
            if (currM > M)
                M = currM;
            iter++;
            prevIter++;
        }
        if (M > 0)
            m = _r_par * M;
        else
            m = 1;
        iter = set.begin();
        iter++;
        prevIter = set.begin();
        R = -200000000;
        while (iter != set.end()) {
            currR = m*(iter->x - prevIter->x) + (std::pow((iter->y - prevIter->y), 2) /
            (m * (iter->x - prevIter->x))) - 2 * (iter->y - prevIter->y);
            if (currR > R) {
                R = currR;
                maxRiter = iter;
                maxPrevRiter = prevIter;
            }
            iter++;
            prevIter++;
        }
        k++;
        newX = (0.5)*(maxRiter->x + maxPrevRiter->x) - ((maxRiter->y - maxPrevRiter->y) / (2 * m));
        funcTmp = func(Xf, newX);
        set.insert(setElemOneVar(newX, funcTmp));
        if (res.z > funcTmp) {
            res.y = newX;
            res.z = funcTmp;
        }
        if (maxRiter->x - maxPrevRiter->x <= _eps)
            timeToStop = true;
    }
    return res;
}

bool compareResults(const resultTwoVar& a, const resultTwoVar& b, const double& eps) {
    bool equals = false;
        if (std::abs(static_cast<double>(a.x - b.x)) <= eps)
            if (std::abs(static_cast<double>(a.y - b.y)) <= eps)
                if (std::abs(static_cast<double>(a.z - b.z)) <= eps)
                    equals = true;
    return equals;
}
