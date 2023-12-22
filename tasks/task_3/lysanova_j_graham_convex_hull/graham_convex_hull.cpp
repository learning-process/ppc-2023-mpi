// Copyright 2023 Lysanova Julia
#include <mpi.h>

#include <random>
#include <vector>
#include <algorithm>

#include "task_3/lysanova_j_graham_convex_hull/graham_convex_hull.h"


bool cmp(Point a, Point b) { return a.x < b.x || a.x == b.x && a.y < b.y; }

bool clockwise(Point a, Point b, Point c) {
    return a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y) < 0;
}

bool counter_clockwise(Point a, Point b, Point c) {
    return a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y) > 0;
}

std::vector<Point> random(const std::vector<int>::size_type Size) {
    std::mt19937 gen(10);
    std::vector<Point> VertexVector(Size);
    for (std::vector<int>::size_type i = 0; i < Size; i++) {
        VertexVector[i] = {(int) gen() % 10, (int) gen() % 20};
    }
    return VertexVector;
}

void StructPoint(MPI_Datatype* structPoint) {
    int block[] = {1, 1};
    MPI_Aint displacements[] = {offsetof(Point, x), offsetof(Point, y)};
    MPI_Datatype Datatype[] = {MPI_INT, MPI_INT};
    MPI_Datatype type;
    MPI_Aint lb, extent;

    MPI_Type_create_struct(2, block, displacements, Datatype, &type);
    MPI_Type_get_extent(type, &lb, &extent);
    MPI_Type_create_resized(type, lb, extent, structPoint);
    MPI_Type_commit(structPoint);
}

std::vector<Point> GrahamMethod(std::vector<Point> VertexVector) {
    if (VertexVector.size() == 1) return VertexVector;
    sort(VertexVector.begin(), VertexVector.end(), &cmp);
    Point p1 = VertexVector[0], p2 = VertexVector.back();

    std::vector<Point> up, down;
    up.push_back(p1);
    down.push_back(p1);

    for (size_t i = 1; i < VertexVector.size(); ++i) {
        if (i == VertexVector.size() - 1 || clockwise(p1, VertexVector[i], p2)) {
            while (up.size() >= 2 &&
                   !clockwise(up[up.size() - 2], up[up.size() - 1], VertexVector[i]))
                up.pop_back();
            up.push_back(VertexVector[i]);
        }
        if (i == VertexVector.size() - 1 || counter_clockwise(p1, VertexVector[i], p2)) {
            while (
                down.size() >= 2 &&
                !counter_clockwise(down[down.size() - 2], down[down.size() - 1], VertexVector[i]))
                down.pop_back();
            down.push_back(VertexVector[i]);
        }
    }
    std::vector<Point> Vertex;
    for (size_t i = 0; i < up.size(); ++i) Vertex.push_back(up[i]);
    for (size_t i = down.size() - 2; i > 0; --i) Vertex.push_back(down[i]);
    return Vertex;
}

std::vector<Point> parallelGrahamMethod(std::vector<Point> VertexVector,
                                        std::vector<int>::size_type vectorSize) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Datatype structPoint;
    StructPoint(&structPoint);

    if (rank == 0) {
        Point x = VertexVector[vectorSize - 1];
        if (vectorSize < size) {
            for (int i = vectorSize; i < size; i++) {
                VertexVector.push_back(x);
                vectorSize++;
            }
        }
    }

    int delta = vectorSize / size;
    if (delta == 0) delta = 1;

    std::vector<Point> localVectorOfVertex(delta);
    std::vector<Point> localGrahamMethod, globalGrahamMethod;

    MPI_Scatter(VertexVector.data(), delta, structPoint,
                localVectorOfVertex.data(), delta, structPoint, 0,
                MPI_COMM_WORLD);
    localGrahamMethod = GrahamMethod(localVectorOfVertex);

    if (rank) {
        MPI_Send(localGrahamMethod.data(), localGrahamMethod.size(), structPoint, 0,
                 0, MPI_COMM_WORLD);
    } else {
        for (int i = 1; i < size; i++) {
            MPI_Status status;
            int sendElements = 0;
            MPI_Probe(i, 0, MPI_COMM_WORLD, &status);
            MPI_Get_count(&status, structPoint, &sendElements);

            std::vector<int>::size_type oldSize = localGrahamMethod.size();
            localGrahamMethod.resize(oldSize + sendElements);
            MPI_Recv(localGrahamMethod.data() + oldSize, sendElements, structPoint, i,
                     0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        if (size != 1) {
            int tail = vectorSize - size * delta;
            if (tail) {
                localVectorOfVertex = std::vector<Point>(
                    VertexVector.begin() + (vectorSize - tail), VertexVector.end());
                globalGrahamMethod = GrahamMethod(localVectorOfVertex);

                std::vector<int>::size_type oldSize = localGrahamMethod.size();
                localGrahamMethod.resize(oldSize + globalGrahamMethod.size());
                for (std::vector<int>::size_type i = oldSize, j = 0;
                     i < localGrahamMethod.size(); i++, j++) {
                    localGrahamMethod[i] = globalGrahamMethod[j];
                }
            }
        }
        globalGrahamMethod = GrahamMethod(localGrahamMethod);
    }

    return globalGrahamMethod;
}
