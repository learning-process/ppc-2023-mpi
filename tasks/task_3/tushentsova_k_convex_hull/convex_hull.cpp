// Copyright 2023 Tushentsova Karina
#include <mpi.h>
#include <iomanip>
#include <vector>
#include <list>
#include <queue>
#include <set>
#include <ctime>
#include <iostream>

#include "task_3/tushentsova_k_convex_hull/convex_hull.h"

std::vector<int> getRandomVector() {
    const int rows = 5, colls = 5;
    int matr[rows][colls];
    int c = 2;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < colls; j++) {
            matr[i][j] = (i + j) % 2;
        }
    }
    std::vector<int> vec(rows * colls);
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < colls; j++) {
            vec[i * colls + j] = matr[i][j];
        }
    vec.push_back(rows);
    vec.push_back(colls);
    return vec;
}

std::vector<int> buildingConvexHull(
        const std::vector<int>& vecInput, int rows, int colls) {
    std::vector<int> vec = vecInput;
    int mark = 1;

    for (int i = 0; i < rows * colls; i++) {
        if (i % colls != 0 && vec[i - 1] != 0 && vec[i] == 1) {
            vec[i] = vec[i - 1];
        } else if (i - colls > 0 && vec[i - colls] != 0 && vec[i] == 1) {
            vec[i] = vec[i - colls];
          } else if (vec[i] == 1) {
                vec[i] = mark;
                mark++;
            }
    }

    std::vector<std::vector<int>> pairs;

    for (int i = 0; i < rows * colls; i++) {
        if (i - colls > 0 && vec[i - colls] != 0 && vec[i] != 0 &&
            vec[i] != vec[i - colls])
            pairs.push_back({ vec[i], vec[i - colls] });
    }

    std::vector<std::set<int>> totalM;

    for (int i = 0; i < pairs.size(); i++) {
        int coinc = -1;

        for (int j = 0; j < totalM.size(); j++) {
            if (totalM[j].count(pairs[i][0]) >= 1 ||
                totalM[j].count(pairs[i][1]) >= 1) {
                if (coinc != -1) {
                    totalM[j].insert(totalM[coinc].begin(),
                        totalM[coinc].end());
                    totalM[coinc].erase(totalM[coinc].begin(),
                        totalM[coinc].end());
                    coinc = j;
                }

                totalM[j].insert({ pairs[i][0], pairs[i][1] });
                coinc = j;
            }
        }

        if (coinc == -1) {
            totalM.push_back({ pairs[i][0], pairs[i][1] });
        }
    }

    std::vector<int> vec2 = vec;

    for (int i = 0; i < totalM.size(); i++) {
        for (int j = 0; j < rows * colls; j++) {
            if (totalM[i].count(vec[j])) {
                vec2[j] = *totalM[i].begin();
            }
        }
    }

    std::set<int> elements;
    for (int i = 0; i < rows * colls; i++) {
        elements.insert(vec2[i]);
    }
    if (elements.count(0) >= 1) elements.erase(elements.begin());

    std::set<int>::iterator iter = elements.begin();
    for (int i = 1; i < elements.size() + 1; i++) {
        for (int j = 0; j < rows * colls; j++) {
            if (*iter == vec2[j]) vec2[j] = i;
        }
        ++iter;
    }

    return vec2;
}

std::vector<int> building(const std::vector<int>& vecInput, int locrows,
    int rows,
    int colls, int rank) {
    std::vector<int> vec = vecInput;

    std::vector<std::vector<int>> pairs;

    for (int i = 0; i < rows * colls; i++) {
        if (i - colls > 0 && vec[i - colls] != 0 && vec[i] != 0 &&
            vec[i] != vec[i - colls])
            pairs.push_back({ vec[i], vec[i - colls] });
    }

    std::vector<std::set<int>> totalM;

    for (int i = 0; i < pairs.size(); i++) {
        int coinc = -1;

        for (int j = 0; j < totalM.size(); j++) {
            if (totalM[j].count(pairs[i][0]) >= 1 ||
                totalM[j].count(pairs[i][1]) >= 1) {
                if (coinc != -1) {
                    totalM[j].insert(totalM[coinc].begin(),
                        totalM[coinc].end());
                    totalM[coinc].erase(totalM[coinc].begin(),
                        totalM[coinc].end());
                    coinc = j;
                }
                totalM[j].insert({ pairs[i][0], pairs[i][1] });
                coinc = j;
            }
        }

        if (coinc == -1) totalM.push_back({ pairs[i][0], pairs[i][1] });
    }

    std::vector<int> vec2 = vec;
    for (int i = 0; i < totalM.size(); i++) {
        for (int j = 0; j < rows * colls; j++) {
            if (totalM[i].count(vec[j])) vec2[j] = *totalM[i].begin();
        }
    }

    std::set<int> elements;
    for (int i = 0; i < rows * colls; i++) {
        elements.insert(vec2[i]);
    }
    if (elements.count(0) >= 1) elements.erase(elements.begin());

    std::set<int>::iterator iter = elements.begin();
    for (int i = 1; i < elements.size() + 1; i++) {
        for (int j = 0; j < rows * colls; j++) {
            if (*iter == vec2[j]) vec2[j] = i;
        }
        ++iter;
    }

    return vec2;
}

std::vector<int> parallBuildingConvexHull(
    std::vector<int> vecInput, int rows, int colls) {
    int rank, size;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int localrows = rows / (size - 1);

    if (rank == size - 1 && rows % (size - 1) != 0)
        localrows += rows % (size - 1);

    std::vector<int> localVec;

    int localStart, localEnd;
    if (rank == size - 1 && rows % (size - 1) != 0)
        localStart = (localrows - rows % (size - 1)) * colls * (rank - 1);
    else if (rank != 0)
        localStart = localrows * colls * (rank - 1);

    if (rank != 0) localEnd = localStart + localrows * colls;

    if (rank != 0) {
        for (int i = localStart; i < localEnd; i++) {
            localVec.push_back(vecInput[i]);
        }
    }

    int temp = localrows * colls;
    int temp2 = temp + (rows % (size - 1)) * colls;
    std::vector<int> tempvec;
    MPI_Status status;
    std::vector<int> res(temp);
    std::vector<int> res2(temp2);

    int mark = (rank - 1) * (colls * localrows / 2) + 1;

    if (rank != 0) {
        for (int i = 0; i < localVec.size(); i++) {
            if (i % colls != 0 && localVec[i - 1] != 0 && localVec[i] == 1) {
                localVec[i] = localVec[i - 1];
            } else if (i - colls >= 0 && localVec[i - colls] != 0 &&
                localVec[i] == 1) {
                localVec[i] = localVec[i - colls];
              } else if (localVec[i] == 1) {
                    localVec[i] = mark;
                    mark++;
                }
        }
    }

    if (rank != 0) {
        MPI_Send(&localVec.front(), localVec.size(),
            MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    if (rank == 0) {
        if (rows % (size - 1) == 0) {
            for (int i = 1; i < size; i++) {
                MPI_Recv(&res.front(), temp, MPI_INT,
                    i, 0, MPI_COMM_WORLD, &status);
                tempvec.insert(tempvec.end(), res.begin(), res.end());
            }
        } else if (rows % (size - 1) != 0) {
            for (int i = 1; i < size - 1; i++) {
                MPI_Recv(&res.front(), temp, MPI_INT,
                    i, 0, MPI_COMM_WORLD, &status);
                tempvec.insert(tempvec.end(), res.begin(), res.end());
            }
            temp += (rows % (size - 1)) * colls;
            MPI_Recv(&res2.front(), temp2, MPI_INT, size - 1, 0, MPI_COMM_WORLD,
                &status);
            tempvec.insert(tempvec.end(), res2.begin(), res2.end());
          }
    }

    if (rank == 0) {
        return building(tempvec, localrows, rows, colls, rank);
    }

    return { 0 };
}
