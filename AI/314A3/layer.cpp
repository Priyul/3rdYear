#include "layer.h"

Layer::Layer(int numNeurons, int numWeights) {
    for (int i = 0; i < numNeurons; i++) {
        Neuron neuron(numWeights);
        neurons.push_back(neuron);
    }
}