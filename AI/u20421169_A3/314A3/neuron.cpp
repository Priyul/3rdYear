#include <cstdlib>
#include <ctime>
#include "neuron.h"

Neuron::Neuron(int numWeights) {
    static bool seed_set = false;
    if (!seed_set) {
        srand(1234);
        seed_set = true;
    }

    for (int i = 0; i < numWeights; i++) {
        double weight = ((double)rand() / RAND_MAX) / sqrt(numWeights);
        weights.push_back(weight);
    }

    bias =  (double)rand() / RAND_MAX;
    output = 0;
    error = 0;
}
