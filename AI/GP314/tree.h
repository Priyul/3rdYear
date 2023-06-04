#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>
#include <cstdlib>
#include <filesystem>
#include <chrono>
#include <stdexcept>

#include "node.h"

using namespace std;

class Tree {
public:
    Node* root;
    Tree(int depth, int max_features);
    Tree(const Tree& other);
    Node* copy_tree(const Node* node);
    
    Node* create_node(int depth, int max_features);

    double evaluate_fitness(const vector<vector<double>>& features, const vector<double>& labels);


};