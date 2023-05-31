#include "neuron.h"

Neuron::Neuron(int numWeights) {
    double variance = sqrt(2.0 / (numWeights));
    for (int i = 0; i < numWeights; i++) {
        double weight = ((double)rand() / RAND_MAX) * variance;
        weights.push_back(weight);
    }
    bias = (double)rand() / RAND_MAX; //random bias between 0 and 1
    output = 0;
    error = 0;
}

