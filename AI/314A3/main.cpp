#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>
#include <cstdlib>
#include "parsedata.h"

// #include "layer.h"
#include "neuralnetwork.h"


using namespace std;

int main() {
    string filename = "breast-cancer.data";
    
    Parser* parser = new Parser(filename);
    parser->inputData();
    parser->removeIncompleteData();
    parser->shuffleData();
    parser->parseTrainingData();
    parser->parseTestData();
    pair<vector<vector<double>>, vector<double>> trainingdata = parser->getTrainingData();
    pair<vector<vector<double>>, vector<double>> testingData = parser->getTestingData();
    

    vector<vector<Layer>> allLayers;
    vector<Layer> layers;

    double lowestError = 99999;
    int hiddenNum = 99999;
    int bestEpoch = 99999;
    for (int hidden = 1; hidden < 51; hidden++) {
        vector<Layer> tempLayer;
        tempLayer.push_back(Layer(51,0, "input"));
        tempLayer.push_back(Layer(hidden, 51, "hidden 1")); //hidden layer 1
        tempLayer.push_back(Layer(hidden, hidden, "hidden 2")); //hidden layer 2
        // tempLayer.push_back(Layer(hidden, hidden, "hidden 3")); //hidden layer 3
        // tempLayer.push_back(Layer(hidden, hidden)); //hidden layer 4
        // tempLayer.push_back(Layer(hidden, hidden)); //hidden layer 5
        tempLayer.push_back(Layer(1,hidden, "output"));

        allLayers.push_back(tempLayer);
    }

    int curious = 0;
    double bestLearningRate = 99999;
    int bestCorrectCount = 0;
    int bestWrongCount = 99999;
    double currBestEpoch = 99999;

    //for (double learningRate = 0.1; learningRate < 1.0; learningRate+0.1) {
        // for (int hidden = 0; hidden < 50; hidden++) {
        //     double learningRate = 0.08;
        //     vector<Layer> currLayer = allLayers.at(hidden);
        //     int epochs = 500;
            
        //     NeuralNetwork* neuralNetwork = new NeuralNetwork(currLayer, trainingdata, learningRate, epochs);
        //     double error = neuralNetwork->train();
        //     currBestEpoch = neuralNetwork->getBestEpoch();

        //     curious+=neuralNetwork->curious;

        //     if (error < lowestError ) {
        //         lowestError = error;
        //         hiddenNum = hidden;
        //         bestEpoch = currBestEpoch;
        //         bestLearningRate = learningRate;

        //         // bestWrongCount = neuralNetwork->wrongCount;
        //         // bestCorrectCount = neuralNetwork->correctCount;
        //     }
                //// neuralNetwork->testNetwork(testingData);
        //     //     if (neuralNetwork->wrongCount < bestWrongCount) {
        //     //         bestWrongCount = neuralNetwork->wrongCount;
        //     //         bestCorrectCount = neuralNetwork->correctCount;
        //     //         lowestError = error;
        //     //         hiddenNum = hidden;
        //     //         bestEpoch = currBestEpoch;
        //     //         bestLearningRate = learningRate;
        //     //     }

        //     //cout << "lowest error:" << lowestError << endl << "hidden layers nodes:" << hiddenNum+1 << endl;
        // }
    //}
    cout << endl;
    cout << "lowest possible error:" << lowestError << endl << "ideal number of hidden layers nodes:" << hiddenNum << endl;
    cout << "optimal number of epochs: " << currBestEpoch << endl;
    cout << "best learning rate: " << bestLearningRate << endl;
    cout << "Best correct count:" << bestCorrectCount << endl;
    cout << "Best wrong count:" << bestWrongCount << endl;
    /* driver code!!! */
        layers.push_back(Layer(51,0, "input")); //input layer, 51 nodes with no weights
        layers.push_back(Layer(29,51, "hidden 1")); //hidden layer 1, 29 nodes each connecting to all 51 nodes so each have 51 weights
        layers.push_back(Layer(29,29, "hidden 2")); //hidden layer 2
        // layers.push_back(Layer(23,23, "hidden 3")); //hidden layer 3
        layers.push_back(Layer(1,29, "output")); //output layer, 1 node connecting to all 29 nodes in the hidden layer so 29 weights

        double initialLearningRate = 0.08;
        int epochs = 3000;

        double learningRate = initialLearningRate;
        
        NeuralNetwork* neuralNetwork = new NeuralNetwork(layers, trainingdata, initialLearningRate, epochs);
        double finalError = neuralNetwork->train();
        cout << "Final error: " << finalError << endl;
        cout << endl;

        cout << "Training data feedforward:" << endl;
        neuralNetwork->testNetwork(testingData);


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