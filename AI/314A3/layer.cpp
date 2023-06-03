#include "layer.h"

Layer::Layer(int numNeurons, int numWeights, string name) {
    for (int i = 0; i < numNeurons; i++) {
        Neuron neuron(numWeights);
        neurons.push_back(neuron);
    }
    this->name = name;
}

string Layer::getName() {
    return name;
}