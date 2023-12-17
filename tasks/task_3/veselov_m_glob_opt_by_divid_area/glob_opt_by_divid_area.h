// Copyright 2023 Veselov Mihail
#ifndef TASKS_TASK_3_VESELOV_M_GLOB_OPT_BY_DIVID_AREA_GLOB_OPT_BY_DIVID_AREA_H_
#define TASKS_TASK_3_VESELOV_M_GLOB_OPT_BY_DIVID_AREA_GLOB_OPT_BY_DIVID_AREA_H_
#include <cmath>

double f1(double x, double y);
double f2(double x, double y);
double f3(double x, double y);
double f4(double x, double y);
double f5(double x, double y);
double f6(double x, double y);

class setElemOneVar {
 public:
    const double x;
    const double y;
    setElemOneVar(double _x, double _y) : x(_x), y(_y) {}
    friend bool operator<(const setElemOneVar& l, const setElemOneVar& r){
        return l.x < r.x;
    }
};

class setElemTwoVar {
 public:
    const double x;
    const double y;
    const double z;
    setElemTwoVar(const double _x, const double _y, const double _z = 0) : x(_x), y(_y), z(_z) {}
    friend bool operator<(const setElemTwoVar& l, const setElemTwoVar& r){
        return l.x < r.x;
    }
};

class setElemR {
 public:
    const double R;
    const double x;
    const double z;
    const double xPrev;
    const double zPrev;
    setElemR(const double _R, const double _x, const double _z, const double _xPrev, const double _zPrev) :
    R(_R), x(_x), z(_z), xPrev(_xPrev), zPrev(_zPrev) {}
    friend bool operator<(const setElemR& l, const setElemR& r) {
        return l.R > r.R;
    }
};

struct resultTwoVar {
    double x;
    double y;
    double z;
};

bool compareResults(const resultTwoVar& a, const resultTwoVar& b, const double& eps = 0.01);

resultTwoVar solveOneVar(const double& _a, const double& _b, const double& Xf, double(*func)(double x, double y),
const double& _eps = 0.1, const int& _N_max = 100, const double& _r_par = 2.0);

resultTwoVar solveTwoVar(const double& _a1, const double& _b1, const double& _a2, const double& _b2,
double(*func)(double x, double y), const double& _eps = 0.1, const int& _Nmax = 100, const double& _epsOneVar = 0.1,
const int& _NmaxOneVar = 100, const double& _r_par = 2.0);

resultTwoVar solveTwoVarSequential(const double& _a1, const double& _b1, const double& _a2, const double& _b2,
double(*func)(double x, double y), const double& _eps = 0.1, const int& _Nmax = 100, const double& _epsOneVar = 0.1,
const int& _NmaxOneVar = 100, const double& _r_par = 2.0);

#endif  // TASKS_TASK_3_VESELOV_M_GLOB_OPT_BY_DIVID_AREA_GLOB_OPT_BY_DIVID_AREA_H_
