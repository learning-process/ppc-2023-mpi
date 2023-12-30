// Copyright 2023 Bakhtiarov Alexander
#ifndef TASKS_TASK_2_BAKHTIAROV_A_BROADCAST_BROADCAST_H_
#define TASKS_TASK_2_BAKHTIAROV_A_BROADCAST_BROADCAST_H_

#include <mpi.h>
#include <cmath>
#include <iostream>
#include <vector>

struct TreeNode {
    int rank;
    TreeNode* parent;
    TreeNode* left_child;
    TreeNode* right_child;

    explicit TreeNode(int r);
};

class Tree {
 public:
    int size;
    int root;
    std::vector<TreeNode*> nodes;

 public:
    Tree(int size, int rootValue);
    void CreateTree();
    TreeNode* findNode(int value) const;
    ~Tree();
};

void broadcast(void* data, int count,
    MPI_Datatype datatype, int root, MPI_Comm comm);
void improved_broadcast(void* data, int count,
    MPI_Datatype datatype, int root, MPI_Comm comm);

#endif  // TASKS_TASK_2_BAKHTIAROV_A_BROADCAST_BROADCAST_H_
