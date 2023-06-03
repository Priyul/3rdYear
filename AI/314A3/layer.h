#pragma once
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>
#include "neuron.h"

using namespace std;

class Layer {
public:
    Layer(int numNeurons, int numWeights, string name);
    vector<Neuron> neurons;
    string name;
    string getName();
};