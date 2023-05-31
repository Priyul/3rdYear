#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

class Parser {
public:    
    Parser();
    pair<vector<vector<double>>, vector<double>> process_training_data(const string &filename);
    pair<vector<vector<double>>, vector<double>> process_testing_data(const string &filename);
private:
    vector<string> split(const string &s, char delimiter);
    int counter = 0;
};