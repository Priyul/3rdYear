#include "tree.h"


Tree::Tree(int depth, int max_features) {
    this->root = create_node(depth, max_features);
}

Tree::Tree(const Tree& other) {
    this->root = copy_tree(other.root);
}

Node* Tree::copy_tree(const Node* node) {
    if (node == NULL) {
        return NULL;
    }
    Node* new_node = new Node();
    new_node->feature_index = node->feature_index;
    new_node->threshold = node->threshold;
    new_node->output = node->output;
    new_node->left_child = copy_tree(node->left_child);
    new_node->right_child = copy_tree(node->right_child);
    return new_node;
}

Node* Tree::create_node(int depth, int max_features) {
    Node* node = new Node();

    if (depth == 0) {
        // This is a leaf node, so generate a random output
        node->output = (double)(rand() % 2); 
    } else {
        // This is an internal node, so randomly choose a feature and threshold
        node->feature_index = rand() % max_features;
        node->threshold = (double)rand() / RAND_MAX; // random double between 0.0 and 1.0

        // Recursively create left and right child nodes
        node->left_child = create_node(depth - 1, max_features);
        node->right_child = create_node(depth - 1, max_features);
    }

    return node;
}

double Tree::evaluate_fitness(const vector<vector<double>>& features, const vector<double>& labels) {
    double fitness = 0;
    for (int i = 0; i < features.size(); i++) {
        if (root->evaluate(features[i]) == labels[i]) {
            fitness++;
        }
    }
    return fitness / features.size();
}