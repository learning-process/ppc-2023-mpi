// Copyright 2023 Khramov Ivan
#include "task_3/khramov_i_global_search/global_search.h"

double getMParam(
    const std::vector<double> &x_points,
    std::function<double(double)> target_func) {
    double param_M = MIN_VALUE;
    for (size_t i = 1; i < x_points.size(); i++) {
        double z1 = target_func(x_points[i]);
        double z0 = target_func(x_points[i - 1]);
        double p = std::abs(x_points[i] - x_points[i - 1]);
        param_M = std::max(std::abs((z1 - z0) / p), param_M);
    }
    return param_M;
}
double getLipschitzParam(
    double param_M,
    double param_r) {
    return (param_M > 0) ? param_r * param_M : 1;
}

std::vector<std::pair<int, double>> getIntervalCharactR(
    const std::vector<double> &x_points,
    std::function<double(double)> target_func,
    double param_m) {
    std::vector<std::pair<int, double>> paramR;
    for (size_t i = 1; i < x_points.size(); i++) {
        double z1 = target_func(x_points[i]);
        double z0 = target_func(x_points[i - 1]);
        double p = std::abs(x_points[i] - x_points[i - 1]);
        double curR = param_m * p +
        pow((z1 - z0), 2) / (param_m * p) - 2 * (z1 + z0);
        paramR.push_back(std::make_pair(i, curR));
    }

    return paramR;
}

double getNextPointX(
    double pointX0,
    double pointX1,
    std::function<double(double)> target_func,
    double param_m) {
    double z1 = target_func(pointX1);
    double z0 = target_func(pointX0);
    return pointX0 +
        (pointX1 - pointX0) / 2.0 + (z1 - z0) / (2.0 * param_m);
}

void sortVecR(std::vector<std::pair<int, double>>* charactR) {
    std::sort(
        (*charactR).begin(),
        (*charactR).end(),
        [](const std::pair<int, double> &chR1,
        const std::pair<int, double> &chR2) {
            return chR1.second > chR2.second;
        });
}

double getGlobalSearchSequantial(
    double a, double b,
    std::function<double(double)> target_func,
    double eps) {
    std::vector<double> sorted_x_points = {a, b};

    while (sorted_x_points.size() < MAX_ITERATIONS) {
        double M = getMParam(sorted_x_points, target_func);
        double m = getLipschitzParam(M, r_param);
        auto charactR = getIntervalCharactR(sorted_x_points, target_func, m);
        sortVecR(&charactR);

        int IndexMaxR = charactR[0].first;
        double nextPointX = getNextPointX(
            sorted_x_points[IndexMaxR - 1],
            sorted_x_points[IndexMaxR],
            target_func,
            m);

        if (std::abs(nextPointX - sorted_x_points[IndexMaxR - 1]) < eps)
            return target_func(nextPointX);

        sorted_x_points.insert(
            std::upper_bound(sorted_x_points.begin(),
            sorted_x_points.end(),
            nextPointX),
            nextPointX);
    }
    return MAX_VALUE;
}

double getGlobalSearchParallel(
    double a,
    double b,
    std::function<double(double)> target_func,
    double eps) {
    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    std::vector<double> sorted_x_points = {a, b};

    int iterationCount = 0;

    while (iterationCount++ < MAX_ITERATIONS) {
        std::vector<double> recvBuffer(3);
        int workedProcessCount = world_size;

        if (world_rank == 0) {
            double M = getMParam(sorted_x_points, target_func);
            double m = getLipschitzParam(M, r_param);
            auto charactR = getIntervalCharactR(
                sorted_x_points,
                target_func,
                m);
            sortVecR(&charactR);

            int IndexMaxR = charactR[0].first;
            recvBuffer = {
                sorted_x_points[IndexMaxR - 1],
                sorted_x_points[IndexMaxR],
                m};

            workedProcessCount = std::min(
                static_cast<size_t>(world_size),
                charactR.size());
            for (int i = 1; i < workedProcessCount; i++) {
                std::pair<int, double> partR = charactR[i];
                std::vector<double> sendBuffer = {
                    sorted_x_points[partR.first - 1],
                    sorted_x_points[partR.first],
                    m};
                MPI_Send(
                    sendBuffer.data(),
                    3,
                    MPI_DOUBLE,
                    i,
                    TAG_STATUS::SIMPLE_TAG,
                    MPI_COMM_WORLD);
            }
        }

        MPI_Bcast(&workedProcessCount, 1, MPI_INT, 0, MPI_COMM_WORLD);

        if (world_rank != 0 && world_rank < workedProcessCount) {
            MPI_Status status;
            MPI_Recv(
                recvBuffer.data(),
                3,
                MPI_DOUBLE,
                0,
                TAG_STATUS::SIMPLE_TAG,
                MPI_COMM_WORLD,
                &status);
        }

        bool ResFinded = false;
        double result = 0.0;
        if (world_rank < workedProcessCount) {
            double nextPointX = getNextPointX(
                recvBuffer[0],
                recvBuffer[1],
                target_func,
                recvBuffer[2]);

            int tag = TAG_STATUS::SEARCH_SOLUTION_TAG;
            if (std::abs(nextPointX - recvBuffer[0]) < eps) {
                tag = TAG_STATUS::FOUND_SOLUTION_TAG;
            }

            if (world_rank != 0) {
                MPI_Send(&nextPointX, 1, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD);
            } else {
                for (int i = 1; i < workedProcessCount; i++) {
                    double inputNextPointX;
                    MPI_Status status;
                    MPI_Recv(
                        &inputNextPointX,
                        1,
                        MPI_DOUBLE,
                        i,
                        MPI_ANY_TAG,
                        MPI_COMM_WORLD,
                        &status);

                    if (status.MPI_TAG == TAG_STATUS::FOUND_SOLUTION_TAG) {
                        result = inputNextPointX;
                        ResFinded = true;
                    }

                    auto upperBound = std::upper_bound(
                        sorted_x_points.begin(),
                        sorted_x_points.end(),
                        inputNextPointX);
                    sorted_x_points.insert(upperBound, inputNextPointX);
                }
                if (tag == TAG_STATUS::FOUND_SOLUTION_TAG) {
                    result = nextPointX;
                    ResFinded = true;
                }

                auto upperBound = std::upper_bound(
                    sorted_x_points.begin(),
                    sorted_x_points.end(),
                    nextPointX);
                sorted_x_points.insert(upperBound, nextPointX);
            }
        }

        MPI_Bcast(&ResFinded, 1, MPI_BYTE, 0, MPI_COMM_WORLD);
        if (ResFinded) {
            return target_func(result);
        }
    }
    return MAX_VALUE;
}
