#pragma once
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

class Neuron {
public: 
    vector<double> weights;
    double bias;
    double output; //serves as input to next node
    double error; //for backpropagation

    Neuron(int numWeights);

};