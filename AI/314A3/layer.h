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
    Layer(int numNeurons, int numWeights);
    vector<Neuron> neurons;
};