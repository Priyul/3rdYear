#include "neuralnetwork.h"
NeuralNetwork::NeuralNetwork(vector<Layer> layers, pair<vector<vector<double>>, vector<int>> trainingdata, double learningRate, int epochs) : learningRate(learningRate), epochs(epochs) {
    for (const Layer &layer : layers) {
        this->layers.push_back(layer);
    }

    this->input = trainingdata.first;
    this->expectedOutput = trainingdata.second;
}

double NeuralNetwork::max(double x, double y) {
    if (x > y) {
        return x;
    } else {
        return y;
    }
    return 0;
}

double NeuralNetwork::ReLU(double x) {
   return max(0,x); 
}

double NeuralNetwork::sigmoid(double x) {
    return 1.0 / (1.0 + exp(-x));
}

void NeuralNetwork::train() {
    //  cout << "train entered" << endl;
    for (int epoch = 0; epoch < epochs; epoch++) {
        double totalError = 0;
        for (int i = 0; i < input.size(); i++) {
            feedforward(input[i]);
            // cout << "Feed forward done" << endl;
            backpropagate(expectedOutput[i]);
            // cout << "backpropagate done" << endl;
            for (Neuron& neuron : layers.back().neurons) {
                totalError += pow((expectedOutput[i] - neuron.output), 2) / 2; //squared error cost function. 
            }
        }
        cout << "Error at the end of epoch " << epoch << " = " << totalError << endl;
    }
}

void NeuralNetwork::feedforward(vector<double>& instance) {
    for (int j = 0; j < layers[0].neurons.size(); j++) {
        //for input layer
        layers[0].neurons[j].output = instance[j]; //instance[i][j] // each neuron in the input layer 
                                                    //gets a feature from the attributes
    }

    for (int i = 1; i < layers.size(); i++) {
        for (int j = 0; j < layers[i].neurons.size(); j++) {
            double sum = 0;
            for (int k = 0; k < layers[i-1].neurons.size(); k++) {
                sum += layers[i-1].neurons[k].output * layers[i].neurons[j].weights[k];
            }
            sum+= layers[i].neurons[j].bias;
            if (i != layers.size() - 1) {  // if not output layer
                layers[i].neurons[j].output = ReLU(sum);
            } else {
                layers[i].neurons[j].output = sigmoid(sum);
            }
        }
    }
}

void NeuralNetwork::backpropagate(int expectedOutput) {
    //error = (expected output - actual output) * derivative of the activation function value
    Layer& outputLayer = layers.back();
    for (int i = 0; i < outputLayer.neurons.size(); i++) {
        double output = outputLayer.neurons[i].output;
        double sigmoid_derivative = output * (1 - output);
        outputLayer.neurons[i].error = (expectedOutput - output)*sigmoid_derivative;
    }

    for (int i = layers.size()-(layers.size()-1); i >= 0; i--) {
        Layer& currentLayer = layers[i];
        Layer& nextLayer = layers[i+1];
        for (int j = 0; j < currentLayer.neurons.size(); j++) {
            double output = currentLayer.neurons[j].output;
            double relu_derivative = output > 0 ? 1 : 0;
            double errorSum = 0;
            for (int k = 0; k < nextLayer.neurons.size(); k++) {
                errorSum += nextLayer.neurons[k].weights[j] * nextLayer.neurons[k].error;
            }
            currentLayer.neurons[j].error = errorSum * relu_derivative;
        }
    }

    for (int i = 1; i < layers.size(); i++) {
        Layer& currentLayer = layers[i];
        Layer& prevLayer = layers[i-1];
        for (int j = 0; j < currentLayer.neurons.size(); j++) {
            for (int k = 0; k < currentLayer.neurons[j].weights.size(); k++) {
                // cout << "old weights: " << currentLayer.neurons[j].weights[k] << endl;
                /*  
                


                WEIGHT UPDATES ARE WRONG :(



                */
                double x = learningRate * currentLayer.neurons[j].error * prevLayer.neurons[k].output;
                cout << "x: " << x << endl;
                currentLayer.neurons[j].weights[k] += learningRate * currentLayer.neurons[j].error * prevLayer.neurons[k].output;
                //cout << "new weights: " << currentLayer.neurons[j].weights[k] << endl;
            }
            currentLayer.neurons[j].bias += learningRate * currentLayer.neurons[j].error;
        }
    }
}