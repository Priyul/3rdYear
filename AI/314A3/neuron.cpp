#include "neuron.h"

Neuron::Neuron(int numWeights) {
    for (int i = 0; i < numWeights; i++) {
         weights.push_back((double)rand() / RAND_MAX); //random weights between 0 and 1
    }
    bias = (double)rand() / RAND_MAX; //random bias between 0 and 1
    output = 0;
}