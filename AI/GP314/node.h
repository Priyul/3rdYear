#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>
#include <cstdlib>
#include <filesystem>
#include <chrono>
#include <stdexcept>

using namespace std;
class Node {
public:
    int feature_index;
    double threshold;
    double output;
    double evaluate(const vector<double>& features);

    Node* left_child;
    Node* right_child;


    Node();
};