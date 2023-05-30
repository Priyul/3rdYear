#pragma once
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>
#include <cmath>

#include "layer.h"

using namespace std;

class NeuralNetwork {
public:    
    NeuralNetwork(vector<Layer> layers, pair<vector<vector<double>>, vector<int>> trainingdata, double learningRate, int epochs);
    void train();
private:
    void feedforward(vector<double>& instance);
    void backpropagate(int expectedOutput);

    double max(double x, double y);
    double ReLU(double x);
    double sigmoid(double x);

    vector<Layer> layers;
    vector<vector<double>> input;
    vector<int> expectedOutput;
    // double sigmoid_derivative;
    double learningRate;
    int epochs;
};