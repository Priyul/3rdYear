#include "neuralnetwork.h"
NeuralNetwork::NeuralNetwork(vector<Layer> layers, pair<vector<vector<double>>, vector<double>> trainingdata, double learningRate, int epochs) : learningRate(learningRate), epochs(epochs) {
    for (const Layer &layer : layers) {
        this->layers.push_back(layer);
    }

    this->input = trainingdata.first;
    this->expectedOutput = trainingdata.second;
    bestEpoch = 0;

    curious = 0;

    this->correctCount = 0;
    this->wrongCount = 0;
}


double NeuralNetwork::binaryCrossEntropy(double expected, double output) {
    const double epsilon = 1e-10;
    return -expected * log(output + epsilon) - (1 - expected) * log(1 - output + epsilon);
}


double NeuralNetwork::max(double x, double y) {
    if (x > y) {
        return x;
    } else {
        return y;
    }
    return 0;
}

void NeuralNetwork::setLearningRate(double rate) {
    this->learningRate = rate;
}

double NeuralNetwork::ReLU(double x) {
   return max(0,x); 
}

double NeuralNetwork::leakyReLU(double x) {
    double alpha = 0.01;
    return x >= 0 ? x : alpha * x;
}

double NeuralNetwork::ReLU_derivative(double x) {
    return x > 0 ? 1 : 0.000001;
}

double NeuralNetwork::sigmoid_derivative(double x) {
    double sigmoid_x = sigmoid(x);
    return sigmoid_x * (1 - sigmoid_x);
}


double NeuralNetwork::sigmoid(double x) {
    double result = 1.0 / (1.0 + exp(-x));
    double min_val = 0.00001;
    double max_val = 0.99999;
    if (result < min_val) {
        return min_val;
    } else if (result > max_val) {
        return max_val;
    } else {
        return result;
    }
}



double NeuralNetwork::train() {
    double totalError = 0;
    double minError = std::numeric_limits<double>::max(); // Initialize with the maximum value
    int epochsWithoutImprovement = 0;

    for (int epoch = 0; epoch < epochs; epoch++) {
        totalError = 0;
        for (int i = 0; i < input.size(); i++) {
            feedforward(input[i]);

            for (Neuron& neuron : layers.back().neurons) {
                totalError += binaryCrossEntropy(expectedOutput[i], neuron.output);
                //loss function
            }

            backpropagate(expectedOutput[i]);
        }

        // If the error reduces, update the minimum error and best epoch
        if (totalError < minError) {
            minError = totalError;
            bestEpoch = epoch;
        }
        std::cout << "Error at the end of epoch " << epoch << " = " << totalError << std::endl;
    }
    return minError;
}

// void NeuralNetwork::feedforward(vector<double>& instance) {
//     for (int j = 0; j < layers[0].neurons.size(); j++) {
//         layers[0].neurons[j].output = instance[j];  //for input layer
//     }

//     for (int layer_index = 1; layer_index < layers.size(); layer_index++) {
//         Layer& current_layer = layers[layer_index];
//         Layer& previous_layer = layers[layer_index - 1];
//         for (Neuron& neuron : current_layer.neurons) {
//             double n = neuron.bias;
//             for (int input_index = 0; input_index < previous_layer.neurons.size(); input_index++) {
//                 n += previous_layer.neurons[input_index].output * neuron.weights[input_index];
//             }
//             if (layer_index < layers.size() - 1) { // hidden layers
//                 neuron.output = leakyReLU(n);
//             } else { // output layer
//                 neuron.output = sigmoid(n);
//             }
//         }
//     }

// }

// void NeuralNetwork::backpropagate(double expectedOutput) {
//     // calculate error information term for each node in the output layer
//     Layer& outputLayer = layers.back();
//     for (Neuron& neuron : outputLayer.neurons) {
//         double output = neuron.output;
//         neuron.error = (expectedOutput - output) ;
//     }

//     // calculate the sum of delta inputs for each node in the hidden layer
//     // calculate the error information term for each node in the hidden layer
//     for (int i = layers.size()-2; i > 0; i--) { 
//         Layer& currentLayer = layers[i];
//         Layer& nextLayer = layers[i+1];
//         for (int j = 0; j < currentLayer.neurons.size(); j++) {
//             double output = currentLayer.neurons[j].output;
//             double relu_derivative = output > 0 ? 1 : 0.01;
//             double sumDelta = 0;

//             for (int k = 0; k < nextLayer.neurons.size(); k++) {
//                 // cout << "layer no:" << i << endl << "node number:" << k << endl << endl;
//                 // cout << "actual node error: " << nextLayer.neurons[k].weights[j] * nextLayer.neurons[k].error << endl;
//                 sumDelta += nextLayer.neurons[k].weights[j] * nextLayer.neurons[k].error;
//             }
//             currentLayer.neurons[j].error = sumDelta * relu_derivative;
//         }
//     }


//     // calculate the weight correction term for each node in the output layer
//     // calculate the weight correction term for each node in the hidden layer
//     for (int i = 1; i < layers.size(); i++) {
//         Layer& currentLayer = layers[i];
//         Layer& prevLayer = layers[i-1];
//         for (int j = 0; j < currentLayer.neurons.size(); j++) {
//             for (int k = 0; k < currentLayer.neurons[j].weights.size(); k++) {
//                 currentLayer.neurons[j].weights[k] -= learningRate * currentLayer.neurons[j].error * prevLayer.neurons[k].output;
//             }
//             // cout << currentLayer.neurons[j].bias << endl;
//             // currentLayer.neurons[j].bias += learningRate * currentLayer.neurons[j].error; //add or -=
//         }
//     }
// }

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
            // cout << "Sum at layer " << i << " on epoch " << epochs << ": " << sum << endl; 
            if (i != layers.size() - 1) {  // if not output layer
                layers[i].neurons[j].output = ReLU(sum);
            } else {
                layers[i].neurons[j].output = sigmoid(sum);
    
            }
        }
    }
}

// void NeuralNetwork::backpropagate(double expectedOutput) {
//     Layer& outputLayer = layers.back();

//     for (int i = 0; i < outputLayer.neurons.size(); i++) {
//         double output = outputLayer.neurons[i].output;
//        //delta k -> step 1
//         outputLayer.neurons[i].error = (expectedOutput-sigmoid(output))*sigmoid_derivative(output);
//     }

//     for (int i = layers.size()-2; i > 0; i--) { 
//         Layer& currentLayer = layers[i];
//         Layer& nextLayer = layers[i+1];
//         for (int j = 0; j < currentLayer.neurons.size(); j++) {
//             double output = currentLayer.neurons[j].output;

//             double relu_derivative = output > 0 ? 1 : 0;
//             double errorSum = 0;
//             for (int k = 0; k < nextLayer.neurons.size(); k++) {
//                 //step 4
//                 errorSum += nextLayer.neurons[k].weights[j] * nextLayer.neurons[k].error;
//             }
//             //step 5
//             currentLayer.neurons[j].error = errorSum * relu_derivative; //delta i
//         }
//     }

//     for (int i = 1; i < layers.size(); i++) {
//         Layer& currentLayer = layers[i];
//         Layer& prevLayer = layers[i-1];
//         for (int j = 0; j < currentLayer.neurons.size(); j++) {
//             for (int k = 0; k < currentLayer.neurons[j].weights.size(); k++) {
//                 //step 6 
//                 currentLayer.neurons[j].weights[k] = currentLayer.neurons[j].weights[k]-(learningRate * currentLayer.neurons[j].error * prevLayer.neurons[k].output);
//             }
//             //step 7
//             currentLayer.neurons[j].bias -= learningRate * currentLayer.neurons[j].error; 
//         }
//     }
//     //step 2 and 3
//     // Layer& prevLayer = layers[layers.size()-1];
//     // outputLayer.weights[0] = (outputLayer.weights[0]) - (learningRate*outputLayer.neurons[0].error*sigmoid());

// }

void NeuralNetwork::backpropagate(double expectedOutput) {
    Layer& outputLayer = layers.back();
    for (int i = 0; i < outputLayer.neurons.size(); i++) {
        double output = outputLayer.neurons[i].output;
        //output = round(output * 100.0) / 100.0;
        //double sigmoid_derivative = output * (1 - output);
        // outputLayer.neurons[i].error = (output - expectedOutput) / (output * (1 - output));
        // outputLayer.neurons[i].error = output*(1-output)*(expectedOutput-output);
        outputLayer.neurons[i].error = (output - expectedOutput);
        // cout << "output: " << output << endl;
        // cout << "output layer error" << outputLayer.neurons[i].error << endl;

    }

    for (int i = layers.size()-2; i > 0; i--) { 
        Layer& currentLayer = layers[i];
        Layer& nextLayer = layers[i+1];
        for (int j = 0; j < currentLayer.neurons.size(); j++) {
            double output = currentLayer.neurons[j].output;
            // double relu_derivative = output > 0 ? 1 : 0;
            double relu_derivative = output > 0 ? 1 : 0.01;
            double errorSum = 0;
            for (int k = 0; k < nextLayer.neurons.size(); k++) {
                // cout << "layer no:" << i << endl << "node number:" << k << endl << endl;
                // cout << "actual node error: " << nextLayer.neurons[k].weights[j] * nextLayer.neurons[k].error << endl;
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
                currentLayer.neurons[j].weights[k] -= learningRate * currentLayer.neurons[j].error * prevLayer.neurons[k].output;
            }
            // cout << currentLayer.neurons[j].bias << endl;
            // currentLayer.neurons[j].bias += learningRate * currentLayer.neurons[j].error; //add or -=
        }
    }
}

double NeuralNetwork::getBestEpoch() {
    return bestEpoch;
}

void NeuralNetwork::testNetwork(pair<vector<vector<double>>, vector<double>> testingData) {
    this->input = testingData.first;
    this->expectedOutput = testingData.second;
    int c = 0;
    for (int i = 0; i < input.size(); i++) {
        c++;
        feedforwardTestData(input[i], c);
    }
    cout << "Correct count:" << this->correctCount << endl;
    cout << "Wrong count:" << this->wrongCount << endl;
    int totalTestSet = correctCount + wrongCount;
    float errorMargin = (correctCount/totalTestSet)*100;
    cout << "accuracy: " << errorMargin << "%"<< endl;
}
void NeuralNetwork::feedforwardTestData(vector<double>& instance, int c) {
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
            sum += layers[i].neurons[j].bias;
            // cout << "Sum at layer " << i << " on epoch " << epochs << ": " << sum << endl; 

            if (i != layers.size() - 1) {  // if not output layer
                layers[i].neurons[j].output = leakyReLU(sum);
            } else {
                layers[i].neurons[j].output = sigmoid(sum);
                // cout << "sigmoid sum:" << layers[i].neurons[j].output << endl;
            }
        }
    }
    outputLayerData(c);
}

void NeuralNetwork::outputLayerData(int c) {
    for (int i = layers.size()-1; i == layers.size()-1; i++) {
        // cout << "Expected output: " << this->expectedOutput[c] << endl;

        int output = 999;
        if (layers[i].neurons[0].output < 0.5) {
            output = 0;
        } else {
            output = 1;
        }
        // cout << "Output: " << round(layers[i].neurons[0].output) << endl << endl;

        if (output == this->expectedOutput[c]) {
            this->correctCount++;
        } else {
            this->wrongCount++;
        }
    }
}