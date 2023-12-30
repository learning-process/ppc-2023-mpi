// Copyright 2023 Bakhtiarov Alexander

#include "task_2/bakhtiarov_a_broadcast/broadcast.h"
#include <mpi.h>
#include <cmath>
#include <iostream>
#include <vector>

TreeNode::TreeNode(int r) : rank(r), parent(nullptr),
    left_child(nullptr), right_child(nullptr) {}

Tree::Tree(int size, int rootValue) : size(size), root(rootValue),
    nodes(std::vector<TreeNode*>(size, nullptr)) {}

void Tree::CreateTree() {
    nodes[0] = new TreeNode(root);
    for (size_t i = 1; i < size; i++) {
        nodes[i] = new TreeNode(i);
        int parentValue = (i - 1) / 2;
        nodes[i]->parent = findNode(parentValue);
        if (i % 2 == 1) {
            nodes[i]->parent->left_child = nodes[i];
        } else {
            nodes[i]->parent->right_child = nodes[i];
        }
    }
}

TreeNode* Tree::findNode(int value) const {
    for (TreeNode* node : nodes) {
        if (node->rank == value) {
            return node;
        }
    }
    return nullptr;
}

Tree::~Tree() {
    for (auto node : nodes) {
        delete node;
    }
}


void broadcast(void* data, int count,
    MPI_Datatype datatype, int root, MPI_Comm comm) {
    int rank;
    MPI_Comm_rank(comm, &rank);
    int size;
    // ***
    MPI_Comm_size(comm, &size);
    if (rank == root) {
        for (size_t i = 0; i < size; i++) {
            if (i != rank) {
                MPI_Send(data, count, datatype, i, 0, comm);
            }
        }
    } else {
        MPI_Recv(data, count, datatype, root, 0, comm, MPI_STATUS_IGNORE);
    }
}

void improved_broadcast(void* data, int count,
    MPI_Datatype datatype, int root, MPI_Comm comm) {
    int rank;
    MPI_Comm_rank(comm, &rank);

    int size;
    MPI_Comm_size(comm, &size);

    Tree tree(size, root);
    tree.CreateTree();
    TreeNode* tmp = tree.findNode(rank);

    if (rank == root) {
        if (tmp->left_child != nullptr) {
            MPI_Send(data, count, datatype, tmp->left_child->rank, 0, comm);
        }
        if (tmp->right_child != nullptr) {
            MPI_Send(data, count, datatype, tmp->right_child->rank, 0, comm);
        }
    } else {
        MPI_Recv(data, count, datatype, tmp->parent->rank,
            0, comm, MPI_STATUS_IGNORE);
        if (tmp->left_child != nullptr) {
            MPI_Send(data, count, datatype, tmp->left_child->rank, 0, comm);
        }
        if (tmp->right_child != nullptr) {
            MPI_Send(data, count, datatype, tmp->right_child->rank, 0, comm);
        }
    }
}
