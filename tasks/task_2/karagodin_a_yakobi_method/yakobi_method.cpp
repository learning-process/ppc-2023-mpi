// Copyright 2023 Karagodin Andrey
#include "task_2/karagodin_a_yakobi_method/yakobi_method.h"

double normalize(const std::vector<double> &V) {
    double normilization = 0.0;
    for (const double &v : V) {
        normilization = std::max(normilization, std::abs(v));
    }
    return normilization;
}

std::vector<std::vector<double>> MatrixIncrease(
    const std::vector<std::vector<double>> &m, std::size_t shift) {
    std::vector<std::vector<double>> res = m;

    for (std::vector<double> &row : res) {
        row.insert(row.end(), shift, 0.0);
    }
    res.insert(res.end(), shift, std::vector<double>(m.size() + shift));
    for (std::size_t i = m.size(); i < res.size(); i++) {
        res[i][i] = 1.0;
    }

    return res;
}

std::vector<double> operator-(const std::vector<double> &a,
 const std::vector<double> &b) {
    std::vector<double> res = a;
    for (std::size_t i = 0; i < res.size(); i++) {
        res[i] -= b[i];
    }
    return res;
}

std::vector<double> VectorIncrease(const std::vector<double> &v,
 std::size_t shift) {
    std::vector<double> res = v;
    res.insert(res.end(), shift, 0.0);
    return res;
}

std::vector<double> YakobiParallel(std::vector<std::vector<double>> a,
 std::vector<double> b) {
    int rank = 0, size = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const std::size_t dim = a.size();
    std::vector<double> x(dim);
    std::size_t delta = dim / size;
    std::size_t shift = 0;

    if (dim % size != 0) {
        shift = size * (++delta) - dim;
        x = VectorIncrease(x, shift);
        b = VectorIncrease(b, shift);
        a = MatrixIncrease(a, shift);
    }

    std::vector<double> localX(delta);
    std::size_t pos = rank * delta;
    double norm = 0.0;
    int l = 0;
    do {
        l++;
        std::vector<double> localNextX(delta);
        for (std::size_t i = 0; i < delta; i++) {
            const std::vector<double> &row = a[i + pos];
            localNextX[i] = b[i + pos];
            for (std::size_t j = 0; j < i + pos; j++) {
                localNextX[i] -= row[j] * x[j];
            }
            for (std::size_t j = pos + i + 1; j < dim + shift; j++) {
                localNextX[i] -= row[j] * x[j];
            }
            localNextX[i] /= row[i + pos];
        }
        std::vector<double> localDiff = localX - localNextX;
        double localNorm = normalize(localDiff);

        MPI_Allreduce(&localNorm, &norm, 1,
         MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
        localX = localNextX;
        MPI_Allgather(localX.data(), delta, MPI_DOUBLE, x.data(),
         delta, MPI_DOUBLE, MPI_COMM_WORLD);
    } while (norm > 1e-16);

    x.erase(x.end() - shift, x.end());
    return x;
}

std::vector<double> YakobiSequential(const std::vector<std::vector<double>>& a,
 const std::vector<double>& b) {
    std::size_t dim = a.size();
    std::vector<double> x(dim);
    double norm = 0.0;

    do {
        std::vector<double> nextX = b;
        for (std::size_t i = 0; i < dim; i++) {
            const std::vector<double> &row = a[i];
            for (std::size_t j = 0; j < i; j++) {
                nextX[i] -= row[j] * x[j];
            }
            for (std::size_t j = i + 1; j < dim; j++) {
                nextX[i] -= row[j] * x[j];
            }
            nextX[i] /= row[i];
        }
        std::vector<double> diff = x - nextX;
        norm = normalize(diff);
        x = nextX;
    } while (norm > 1e-16);

    return x;
}
