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
    NeuralNetwork(vector<Layer> layers, pair<vector<vector<double>>, vector<double>> trainingdata, double learningRate, int epochs);

    double train();
    void setLearningRate(double rate);
    double getBestEpoch();

    int curious;

private:
    void feedforward(vector<double>& instance);
    void backpropagate(double expectedOutput);

    double max(double x, double y);
    double ReLU(double x);
    double leakyReLU(double x);
    double sigmoid(double x);
    double binaryCrossEntropy(double expected, double output);

    vector<Layer> layers;
    vector<vector<double>> input;
    vector<double> expectedOutput;
    // double sigmoid_derivative;
    double learningRate;
    double learningRateDecay;
    int epochs;
    int bestEpoch;
};