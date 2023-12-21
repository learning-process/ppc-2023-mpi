// Copyright 2023 Sokolova Daria
#include <mpi.h>
#include <stddef.h>
#include <random>
#include <algorithm>
#include <numeric>
#include <set>
#include "../../../tasks/task_3/sokolova_d_jarvis_algorithm/jarvis_algorithm.h"

int rotate(const Point& vertex1, const Point& vertex2, const Point& vertex3) {
    return (vertex2.x - vertex1.x) * (vertex3.y - vertex2.y)
        - (vertex2.y - vertex1.y) * (vertex3.x - vertex2.x);
}
void initializeStructPoint2d(MPI_Datatype* structPoint) {
    int blocklengthsArray[] = { 1, 1 };
    MPI_Datatype typesArray[] = { MPI_INT, MPI_INT };
    MPI_Aint shiftsArray[] = { offsetof(Point, x), offsetof(Point, y) };
    MPI_Datatype tmp_type;
    MPI_Aint lb, large;
    MPI_Type_create_struct(2, blocklengthsArray,
        shiftsArray, typesArray, &tmp_type);
    MPI_Type_get_extent(tmp_type, &lb, &large);
    MPI_Type_create_resized(tmp_type, lb, large, structPoint);
    MPI_Type_commit(structPoint);
}

vector<Point> randomVector(const vector<int>::size_type size) {
    std::mt19937 gen(100);
    vector<Point> vectorVertex(size);
    for (vector<int>::size_type i = 0; i < size; i++) {
        int randomX = gen() % 10000;
        int randomY = gen() % 10000;
        Point randomPoint(randomX, randomY);
        vectorVertex[i] = randomPoint;
    }
    return vectorVertex;
}

vector<Point> consistentJarvisMarch(const vector<Point>& vertexVector) {
    if (vertexVector.empty()) {
        return {};
    }
    vector<int> indexVector(vertexVector.size());
    std::iota(indexVector.begin(), indexVector.end(), 0);
    for (vector<int>::size_type i = 1; i <
        vertexVector.size(); ++i) {
        if (vertexVector[indexVector[i]].x <
            vertexVector[indexVector[0]].x) {
            std::swap(indexVector[i], indexVector[0]);
        }
    }
    vector<int> curve = { indexVector[0] };
    indexVector.erase(indexVector.begin());
    indexVector.push_back(curve[0]);
    int right = 0;
    do {
        for (vector<int>::size_type i = 1; i <
            indexVector.size(); i++) {
            if (rotate(vertexVector[curve[curve.size() - 1]],
                vertexVector[indexVector[right]],
                vertexVector[indexVector[i]]) < 0) {
                right = i;
            }
        }
        if (indexVector[right] != curve[0]) {
            curve.push_back(indexVector[right]);
            indexVector.erase(indexVector.begin() + right);
        }
    } while (indexVector[right] != curve[0]);
    vector<Point> curve_vertex(curve.size());
    for (vector<int>::size_type i = 0; i < curve.size(); i++) {
        curve_vertex[i] = vertexVector[curve[i]];
    }
    return curve_vertex;
}
vector<Point> parallelJarvisMarch(const vector<Point>&
    vector_vertex, vector<int>::size_type vector_size) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Datatype customPoint2d;
    initializeStructPoint2d(&customPoint2d);

    int chunkSize = vector_size / size;

    vector<Point> localPoints(chunkSize);
    vector<Point> localJarvis, globalJarvis;
    MPI_Scatter(vector_vertex.data(), chunkSize,
        customPoint2d, localPoints.data(), chunkSize,
        customPoint2d, 0, MPI_COMM_WORLD);
    if (localPoints.empty()) {
        return {};
    }

    localJarvis = consistentJarvisMarch(localPoints);

    if (rank != 0) {
        MPI_Send(localJarvis.data(), localJarvis.size(),
            customPoint2d, 0, 0, MPI_COMM_WORLD);
    } else {
        for (int i = 1; i < size; i++) {
            MPI_Status status;
            int receivedElements = 0;
            MPI_Probe(i, 0, MPI_COMM_WORLD, &status);
            MPI_Get_count(&status, customPoint2d, &receivedElements);
            vector<int>::size_type previousSize = localJarvis.size();
            localJarvis.resize(previousSize + receivedElements);
            MPI_Recv(localJarvis.data() + previousSize,
                receivedElements, customPoint2d, i, 0,
                MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        if (size != 1) {
            int tail = vector_size - size * chunkSize;
            if (tail) {
                localPoints = vector<Point>(vector_vertex.begin() +
                    (vector_size - tail), vector_vertex.end());
                globalJarvis = consistentJarvisMarch(localPoints);
                vector<int>::size_type oldSize = localJarvis.size();
                localJarvis.resize(oldSize + globalJarvis.size());
                for (vector<int>::size_type i = oldSize, j = 0;
                    i < localJarvis.size(); i++, j++) {
                    localJarvis[i] = globalJarvis[j];
                }
            }
        }
        globalJarvis = consistentJarvisMarch(localJarvis);
    }

    return globalJarvis;
}
