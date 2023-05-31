#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>
#include "parsedata.h"

// #include "layer.h"
#include "neuralnetwork.h"


using namespace std;

int main() {
    string filename = "breast-cancer.data";
    
    Parser* parser = new Parser();

    pair<vector<vector<double>>, vector<double>> trainingdata;
    pair<vector<vector<double>>, vector<double>> testingData;
    trainingdata = parser->process_training_data(filename);
    testingData = parser->process_testing_data(filename);

    vector<vector<Layer>> allLayers;

    vector<Layer> layers;
    double lowestError = 99999;
    int hiddenNum = 99999;
    int bestEpoch = 99999;
    for (int hidden = 1; hidden < 51; hidden++) {
        vector<Layer> tempLayer;
        tempLayer.push_back(Layer(51,0));
        tempLayer.push_back(Layer(hidden, 51)); //hidden layer 1
        tempLayer.push_back(Layer(hidden, hidden)); //hidden layer 2
        // tempLayer.push_back(Layer(hidden, hidden)); //hidden layer 3
        // tempLayer.push_back(Layer(hidden, hidden)); //hidden layer 4
        // tempLayer.push_back(Layer(hidden, hidden)); //hidden layer 5
        tempLayer.push_back(Layer(1,hidden));

        allLayers.push_back(tempLayer);
    }

    int curious = 0;

    for (int hidden = 0; hidden < 50; hidden++) {
        double initialLearningRate = 0.04;
        double finalLearningRate = 0.01; // Or whatever lower bound 
        vector<Layer> currLayer = allLayers.at(hidden);
        int epochs = 10000;
        double learningRateDecay = (initialLearningRate - finalLearningRate) / epochs;
        double learningRate = initialLearningRate;
        
        NeuralNetwork* neuralNetwork = new NeuralNetwork(currLayer, trainingdata, learningRate, epochs);
        double error = neuralNetwork->train();
        double currBestEpoch = neuralNetwork->getBestEpoch();

        curious++;

        if (error < lowestError ) {
            lowestError = error;
            hiddenNum = hidden;
            bestEpoch = currBestEpoch;
        }

        // cout << "lowest error:" << lowestError << endl << "hidden layers nodes:" << hiddenNum+1 << endl;
    }

    cout << "lowest possible error:" << lowestError << endl << "ideal number of hidden layers nodes:" << hiddenNum << endl;
    cout << "optimal number of epochs: " << bestEpoch << endl;
    cout << "Number of runs over neural network: " << curious << endl;
    /* driver code!!! */
        // layers.push_back(Layer(51,0)); //input layer, 51 nodes with no weights
        // layers.push_back(Layer(34,51)); //hidden layer 1, 34 nodes each connecting to all 51 nodes so each have 51 weights
        // layers.push_back(Layer(34,34)); //hidden layer 2
        // // layers.push_back(Layer(34,34)); //hidden layer 3
        // layers.push_back(Layer(1,34)); //output layer, 1 node connecting to all 34 nodes in the hidden layer so 34 weights

        // double initialLearningRate = 0.04;
        // double finalLearningRate = 0.01; // Or whatever lower bound 
        // int epochs = 8829;
        // double learningRateDecay = (initialLearningRate - finalLearningRate) / epochs;
        // double learningRate = initialLearningRate;
        
        // NeuralNetwork* neuralNetwork = new NeuralNetwork(layers, trainingdata, learningRate, epochs);
        // double finalError = neuralNetwork->train();
        // cout << "Final error: " << finalError << endl;
        // //add training data feedforward function and output...

// for (int i = 0; i < epochs; i++) {
//     neuralNetwork->train();
//     // learningRate -= learningRateDecay;
//     // neuralNetwork->setLearningRate(learningRate);
// }


    return 0;
}

    // for (int i = 0; i < layers.size(); i++) {
    //     cout << "Layer " << i << ":" << endl;
    //     for (int j = 0; j < layers[i].neurons.size(); j++) {
    //         cout << "Neuron no." << j << ":" << endl;
    //         for (int k = 0; k < layers[i].neurons[j].weights.size(); k++) {
    //             cout << "Neuron weight." << k << ": " << layers[i].neurons[j].weights[k] << endl;
    //         } 
    //     }
    // } //weights initialization works