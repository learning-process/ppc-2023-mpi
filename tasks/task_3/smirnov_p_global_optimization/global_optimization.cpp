// Copyright 2023 Smirnov Pavel
#include "task_3/smirnov_p_global_optimization/global_optimization.h"

// global min of ONE argument function.
// fixed variable is need for dimensional reduction method,
// its destination will be clear in getGlobalMinimumOnPlane() function
Point2D getGlobalMinimumOnSegment(double fixedVariable, double leftBorder,
double rightBorder, double(*func)(double x, double y), int maxIterationsCount,
double r, double accuracy) {
    std::set<Point2D, Point2DComparator> trials;  // calculated points

    double M, maxM, m, R, maxR;
    int countOfTrials = 0;

    // handle borders
    trials.insert(Point2D(leftBorder, func(fixedVariable, leftBorder)));
    countOfTrials++;
    trials.insert(Point2D(rightBorder, func(fixedVariable, rightBorder)));
    countOfTrials++;

    while (countOfTrials < maxIterationsCount) {
        maxM = -999;
        auto iteration = trials.begin();
        iteration++;
        auto previousIteration = trials.begin();

        // calculate max M, which actually is max derivative on intervals
        while (iteration != trials.end()) {
            M = std::abs(static_cast<double>(
                (iteration->y - previousIteration->y) /
                (iteration->x - previousIteration->x)));
            if (M > maxM)
                maxM = M;
            iteration++;
            previousIteration++;
        }

        // calculate m depending on M
        if (maxM > 0)
            m = r * maxM;
        else
            m = 1;

        // restore iterators to beginning
        iteration = trials.begin();
        iteration++;
        previousIteration = trials.begin();

        maxR = -999;

        auto iterationOnMaxR = trials.begin();
        auto previousIterationOnMaxR = trials.begin();

        // calculate R
        while (iteration != trials.end()) {
            R = m*(iteration->x - previousIteration->x) + (std::pow(
                (iteration->y - previousIteration->y), 2) /
            (m * (iteration->x - previousIteration->x))) - 2 *
            (iteration->y + previousIteration->y);
            if (R > maxR) {
                maxR = R;
                iterationOnMaxR = iteration;
                previousIterationOnMaxR = previousIteration;
            }
            iteration++;
            previousIteration++;
        }

        countOfTrials++;

        // calculating X of new point for trial
        double newPointForTrial = (0.5)*(iterationOnMaxR->x +
        previousIterationOnMaxR->x) - ((iterationOnMaxR->y -
        previousIterationOnMaxR->y) / (2 * m));

        // save new counted value of function in new point
        trials.insert(Point2D(newPointForTrial, func(fixedVariable,
        newPointForTrial)));

        // finish our work when interval is less than required accuracy
        if (iterationOnMaxR->x - previousIterationOnMaxR->x <= accuracy)
            break;
    }

    // find and return calculated min value from set of trials

    auto iterationOnGlobalMin = trials.begin();

    for (auto it=trials.begin(); it != trials.end(); ++it) {
        // std::cout << fixedVariable << ' ' << (it->x) << ' ' <<
        // (it->y) << std::endl;
        if (it->y < iterationOnGlobalMin->y) {
            iterationOnGlobalMin = it;
        }
    }

    Point2D globalMin(iterationOnGlobalMin->x, iterationOnGlobalMin->y);
    // std::cout << fixedVariable << ' ' << (iterationOnGlobalMin->x) <<
    // ' ' << (iterationOnGlobalMin->y) << std::endl;

    return globalMin;
}

// global min of TWO argument function
// suppose oX is left-right and oY is bottom-top. oZ is the target function
// we are going to fix oX coordinate, then find minimum on oY segment,
// then choose min of mins
Point3D getGlobalMinimumOnPlane(double xLeftBorder, double xRightBorder,
double yBottomBorder, double yTopBorder, double(*func)(double x, double y),
int maxIterationsCount, double r, double accuracy) {
    std::set<Point3D, Point3DComparator> trials;  // calculated points

    double M, maxM, m, R, maxR;
    int countOfTrials = 0;

    // handle borders
    // the second param after xLeftBorder will return point2D,
    // and overloaded constructor for  Point3D will be used
    trials.insert(Point3D(xLeftBorder, getGlobalMinimumOnSegment(xLeftBorder,
    yBottomBorder, yTopBorder, func, maxIterationsCount, r, accuracy)));
    countOfTrials++;
    trials.insert(Point3D(xRightBorder, getGlobalMinimumOnSegment(xRightBorder,
    yBottomBorder, yTopBorder, func, maxIterationsCount, r, accuracy)));
    countOfTrials++;

    while (countOfTrials < maxIterationsCount) {
        maxM = -999;
        auto iteration = trials.begin();
        iteration++;
        auto previousIteration = trials.begin();

        // calculate max M, which actually is max derivative on intervals
        while (iteration != trials.end()) {
            M = std::abs(static_cast<double>(
                (iteration->z - previousIteration->z) /
                (iteration->x - previousIteration->x)));
            if (M > maxM)
                maxM = M;
            iteration++;
            previousIteration++;
        }

        // calculate m depending on M
        if (maxM > 0)
            m = r * maxM;
        else
            m = 1;

        // restore iterators to beginning
        iteration = trials.begin();
        iteration++;
        previousIteration = trials.begin();

        maxR = -999;

        auto iterationOnMaxR = trials.begin();
        auto previousIterationOnMaxR = trials.begin();

        // calculate R
        while (iteration != trials.end()) {
            R = m*(iteration->x - previousIteration->x) +
            (std::pow((iteration->z - previousIteration->z), 2) /
            (m * (iteration->x - previousIteration->x))) - 2 *
            (iteration->z + previousIteration->z);
            if (R > maxR) {
                maxR = R;
                iterationOnMaxR = iteration;
                previousIterationOnMaxR = previousIteration;
            }
            iteration++;
            previousIteration++;
        }

        countOfTrials++;

        // calculating X of new point for trial
        double newPointForTrial = (0.5)*(iterationOnMaxR->x +
        previousIterationOnMaxR->x) - ((iterationOnMaxR->z -
        previousIterationOnMaxR->z) / (2 * m));

        // save new counted value of function in new point
        trials.insert(Point3D(newPointForTrial,
        getGlobalMinimumOnSegment(newPointForTrial, yBottomBorder,
        yTopBorder, func, maxIterationsCount, r, accuracy)));

        // finish our work when interval is less than required accuracy
        if (iterationOnMaxR->x - previousIterationOnMaxR->x <= accuracy &&
            iterationOnMaxR->y - previousIterationOnMaxR->y <= accuracy)
            break;
    }

    // find and return calculated min value from set of trials

    auto iterationOnGlobalMin = trials.begin();

    for (auto it=trials.begin(); it != trials.end(); ++it) {
        // std::cout << (it->x) << ' ' << (it->y)
        // << ' ' << (it->z) << std::endl;
        if (it->z < iterationOnGlobalMin->z) {
            iterationOnGlobalMin = it;
        }
    }

    Point3D globalMin(iterationOnGlobalMin->x, iterationOnGlobalMin->y,
    iterationOnGlobalMin->z);
    // std::cout << (iterationOnGlobalMin->x) << ' ' <<
    // (iterationOnGlobalMin->y) << ' ' << (iterationOnGlobalMin->z)
    // << std::endl;

    return globalMin;
}

Point3D getGlobalMinimumOnPlaneParallelly(double xLeftBorder,
double xRightBorder, double yBottomBorder, double yTopBorder,
double(*func)(double x, double y), int maxIterationsCount, double r,
double accuracy) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (!(size > 1))
        return getGlobalMinimumOnPlane(xLeftBorder, xRightBorder,
        yBottomBorder, yTopBorder, func, maxIterationsCount, r, accuracy);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Status status;


    Point3D localMin(-9999, -9999, -9999);
    Point3D globalMin(-9999, -9999, -9999);
    // printf("Before %d\n", rank);

    if (rank == 0) {
        // printf("Hello from 0\n");
        std::set<Point3D, Point3DComparator> trials;
        double M, maxM, m, R;
        int countOfTrials = 0;
        // printf("rank 0 after var init\n");

        double partPerProcess = (xRightBorder - xLeftBorder) / (size - 1);
        for (int proc = 1; proc < size; ++proc) {
            double fixedVariable = xLeftBorder + proc * partPerProcess;
            MPI_Send(&fixedVariable, 1, MPI_DOUBLE, proc, 1, MPI_COMM_WORLD);
        }

        // printf("rank 0 after send\n");

        trials.insert(Point3D(xLeftBorder, getGlobalMinimumOnSegment(
        xLeftBorder,
        yBottomBorder, yTopBorder, func, maxIterationsCount, r, accuracy)));
        countOfTrials++;
        trials.insert(Point3D(xRightBorder, getGlobalMinimumOnSegment(
            xRightBorder, yBottomBorder, yTopBorder, func,
            maxIterationsCount, r, accuracy)));
        countOfTrials++;

        // printf("rank 0 before recv\n");
        double tmpLocalMin[3];
        for (int proc = 1; proc < size; ++proc) {
            MPI_Recv(tmpLocalMin, 3, MPI_DOUBLE, proc, 1,
            MPI_COMM_WORLD, &status);
            trials.insert(Point3D(tmpLocalMin[0], tmpLocalMin[1],
            tmpLocalMin[2]));
        }

        // printf("rank 0 after recv\n");

        std::set<characteristicR> characteristics;

        bool accuracyAchieved = false;
        while (!accuracyAchieved && countOfTrials < maxIterationsCount) {
            // printf("rank 0 in while\n");
            characteristics.clear();
            maxM = -999;
            auto iteration = trials.begin();
            iteration++;
            auto previousIteration = trials.begin();

            // calculate max M, which actually is max derivative on intervals
            while (iteration != trials.end()) {
                // printf("rank 0 in inner while\n");
                M = std::abs(static_cast<double>(
                    (iteration->z - previousIteration->z) /
                    (iteration->x - previousIteration->x)));
                if (M > maxM)
                    maxM = M;
                iteration++;
                previousIteration++;
            }

            // printf("rank 0 after inner while\n");

            // calculate m depending on M
            if (maxM > 0)
                m = r * maxM;
            else
                m = 1;

            // restore iterators to beginning
            iteration = trials.begin();
            iteration++;
            previousIteration = trials.begin();

            // calculate R
            while (iteration != trials.end()) {
                // printf("rank 0 in R inner while\n");
                R = m*(iteration->x - previousIteration->x) +
                (std::pow((iteration->z - previousIteration->z), 2) /
                (m * (iteration->x - previousIteration->x))) - 2 *
                (iteration->z + previousIteration->z);

                characteristics.insert(
                    characteristicR(R, iteration->x, iteration->z,
                    previousIteration->x, previousIteration->z));

                iteration++;
                previousIteration++;
            }

            // printf("rank 0 after R inner while\n");

            auto iteratorR = characteristics.begin();

            for (int proc = 1; proc < size; ++proc) {
                countOfTrials++;
                double newPointForTrial = (0.5)*
                (iteratorR->x + iteratorR->xPrevious) -
                ((iteratorR->z - iteratorR->zPrevious) / (2 * m));
                MPI_Send(&newPointForTrial, 1, MPI_DOUBLE, proc, 1,
                MPI_COMM_WORLD);
                if (iteratorR->x - iteratorR->xPrevious <= accuracy) {
                    accuracyAchieved = true;
                }
                iteratorR++;
            }
            // printf("rank 0 after send newPointForTrial\n");

            if (accuracyAchieved == true) {
                break;
            }

            double tmpLocalMin_2[3];
            for (int proc = 1; proc < size; ++proc) {
                MPI_Recv(tmpLocalMin_2, 3, MPI_DOUBLE, proc,
                1, MPI_COMM_WORLD, &status);
                trials.insert(Point3D(tmpLocalMin_2[0],
                tmpLocalMin_2[1], tmpLocalMin_2[2]));
            }
            // printf("rank 0 after recv localMin\n");
        }

        // printf("rank 0 after outer while\n");

        for (int proc = 1; proc < size; ++proc) {
            double terminate = accuracy * 0.001;
            MPI_Send(&terminate, 1, MPI_DOUBLE, proc, 1, MPI_COMM_WORLD);
        }

        // MPI_Barrier(MPI_COMM_WORLD);

        // find and return calculated min value from set of trials

        auto iterationOnGlobalMin = trials.begin();

        for (auto it=trials.begin(); it != trials.end(); ++it) {
            // std::cout << (it->x) << ' '
            // << (it->y) << ' ' << (it->z) << std::endl;
            if (it->z < iterationOnGlobalMin->z) {
                iterationOnGlobalMin = it;
            }
        }

        globalMin = Point3D(iterationOnGlobalMin->x,
        iterationOnGlobalMin->y, iterationOnGlobalMin->z);

    } else {
        // printf("Hello from %d", rank);
        bool terminate = false;
        while (!terminate) {
            double message;
            MPI_Recv(&message, 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, &status);
            if (message == accuracy * 0.001) {
                terminate = true;
            } else {
                localMin = Point3D(message,
                getGlobalMinimumOnSegment(message, yBottomBorder, yTopBorder,
                func, maxIterationsCount, r, accuracy));
                double tmpLocalMin_3[3] = {localMin.x, localMin.y, localMin.z };
                MPI_Send(&tmpLocalMin_3[0], 3, MPI_DOUBLE, 0, 1,
                MPI_COMM_WORLD);
            }
        }
    }
    // printf("after: %d", rank);
    return globalMin;
}

double function_1(double x, double y) {
    return x*x + y*y;
}

double function_2(double x, double y) {
    return 0.5*(x-5)*(x-5) + y - 3;
}

double function_3(double x, double y) {
    return (x-5)*(x-5) + (y-3)*(y-3);
}

bool operator<(const Point2D& firstPoint,
    const Point2D& secondPoint) {
        return firstPoint.x < secondPoint.x;
    }

bool operator<(const Point3D& firstPoint,
    const Point3D& secondPoint) {
        return firstPoint.x < secondPoint.x;
    }
