// Copyright 2023 Zawadowski Jan
#include "task_2/zawadowski_j_broadcast/broadcast.h"

struct Node {
    Node* parent,
        * left,
        * right;
    int key;
    Node() { parent = nullptr; left = nullptr;
             right = nullptr; key = -1; }
    Node(Node* parent, Node* left, Node* right, int key) {
        this->parent = parent;
        this->left = left;
        this->right = right;
        this->key = key;
    }
};

void insertNode(Node* root, int key) {
    Node* node;
    std::vector<Node*> vector;
    if (root != nullptr) vector.push_back(root);
    while (!vector.empty()) {
        node = vector.back();
        vector.pop_back();
        if (node->left != nullptr) {
            vector.push_back(node->left);
        } else {
            node->left = new Node(node, nullptr, nullptr, key);
            return;
        }
        if (node->right != nullptr) {
            vector.push_back(node->right);
        } else {
            node->right = new Node(node, nullptr, nullptr, key);
            return;
        }
    }
}

Node* findNode(Node* root, int key) {
    Node* node;
    std::vector<Node*> vector;
    if (root != nullptr) vector.push_back(root);
    while (!vector.empty()) {
        node = vector.back();
        vector.pop_back();
        if (node->key == key) return node;
        if (node->left != nullptr) vector.push_back(node->left);
        if (node->right != nullptr) vector.push_back(node->right);
    }
    return nullptr;
}

void broadcast(void* buffer, int count, MPI_Datatype datatype,
               int root, MPI_Comm comm) {
    int sizeWorld, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &sizeWorld);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    Node* node = new Node(nullptr, nullptr, nullptr, root);

    for (int i = 0; i < sizeWorld; i++)
        if (i != root) insertNode(node, i);

    if (rank == root) {
        if (node->left != nullptr)
            MPI_Send(buffer, count, datatype, node->left->key, 0, comm);
        if (node->right != nullptr)
            MPI_Send(buffer, count, datatype, node->right->key, 0, comm);
    } else {
        MPI_Status status;
        Node* temp = findNode(node, rank);
        MPI_Recv(buffer, count, datatype, temp->parent->key, 0, comm, &status);
        if (temp->left != nullptr)
            MPI_Send(buffer, count, datatype, temp->left->key, 0, comm);
        if (temp->right != nullptr)
            MPI_Send(buffer, count, datatype, temp->right->key, 0, comm);
    }
}
