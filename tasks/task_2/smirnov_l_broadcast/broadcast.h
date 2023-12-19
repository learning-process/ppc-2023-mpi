// Copyright 2023 Smirnov Leonid
#pragma once

#include <mpi.h>
#include <cmath>
#include <iostream>
#include <vector>

void test();

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
