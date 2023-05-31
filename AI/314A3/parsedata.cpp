#include "parsedata.h"

Parser:: Parser() {

}

vector<string> Parser::split(const string &s, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);
    while (getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }

    // for (int i = 0; i < tokens.size(); i++) {
    //     cout << tokens[i] << endl;
    // }
    // cout << endl; // data read correctly

    return tokens;
}

pair<vector<vector<double>>, vector<double>> Parser::process_data(const string &filename) {
    ifstream file(filename);
    vector<vector<double>> instances;
    vector<double> labels;

    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            vector<string> tokens = split(line, ',');
            vector<double> instance(51, 0);

            if (tokens[0] == "no-recurrence-events")
                instance[0] = 1;
            else if (tokens[0] == "recurrence-events")
                instance[1] = 1;

            if (tokens[1] == "10-19") 
                instance[2] = 1;
            else if (tokens[1] == "20-29") 
                instance[3] = 1;
            else if (tokens[1] == "30-39") 
                instance[4] = 1;
            else if (tokens[1] == "40-49") 
                instance[5] = 1;
            else if (tokens[1] == "50-59") 
                instance[6] = 1;
            else if (tokens[1] == "60-69") 
                instance[7] = 1;
            else if (tokens[1] == "70-79") 
                instance[8] = 1;
            else if (tokens[1] == "80-89") 
                instance[9] = 1;
            else if (tokens[1] == "90-99") 
                instance[10] = 1;

            if (tokens[2] == "lt40") 
                instance[11] = 1;
            else if (tokens[2] == "ge40") 
                instance[12] = 1;
            else if (tokens[2] == "premeno") 
                instance[13] = 1;

            if (tokens[3] == "0-4") 
                instance[14] = 1;
            else if (tokens[3] == "5-9")
                instance[15] = 1;
            else if (tokens[3] == "10-14")
                instance[16] = 1;
            else if (tokens[3] == "15-19")
                instance[17] = 1;
            else if (tokens[3] == "20-24")
                instance[18] = 1;
            else if (tokens[3] == "25-29")
                instance[19] = 1;
            else if (tokens[3] == "30-34")
                instance[20] = 1;
            else if (tokens[3] == "35-39")
                instance[21] = 1;
            else if (tokens[3] == "40-44")
                instance[22] = 1;
            else if (tokens[3] == "45-49")
                instance[23] = 1;
            else if (tokens[3] == "50-54")
                instance[24] = 1;
            else if (tokens[3] == "55-59")
                instance[25] = 1;    

            if (tokens[4] == "0-2") 
                instance[26] = 1;
            else if (tokens[4] == "3-5") 
                instance[27] = 1;
            else if (tokens[4] == "6-8") 
                instance[28] = 1;
            else if (tokens[4] == "9-11") 
                instance[29] = 1;
            else if (tokens[4] == "12-14") 
                instance[30] = 1;
             else if (tokens[4] == "15-17") 
                instance[31] = 1;
            else if (tokens[4] == "18-20") 
                instance[32] = 1;
            else if (tokens[4] == "21-23") 
                instance[33] = 1;
            else if (tokens[4] == "24-26") 
                instance[34] = 1;
            else if (tokens[4] == "27-29") 
                instance[35] = 1;
            else if (tokens[4] == "30-32") 
                instance[36] = 1;
            else if (tokens[4] == "33-35") 
                instance[37] = 1;
            else if (tokens[4] == "36-39") 
                instance[38] = 1;

            if (tokens[5] == "yes") 
                instance[39] = 1;
            else if (tokens[5] == "no") 
                instance[40] = 1;

            if (tokens[6] == "1") 
                instance[41] = 1;
            else if (tokens[6] == "2") 
                instance[42] = 1;
            else if (tokens[6] == "3") 
                instance[43] = 1;

            if (tokens[7] == "left") 
                instance[44] = 1;
            else if (tokens[7] == "right")
                instance[45] = 1;

            if (tokens[8] == "left-up") 
                instance[46] = 1;
            else if (tokens[8] == "left-low") 
                instance[47] = 1;
            else if (tokens[8] == "right-up") 
                instance[48] = 1;
            else if (tokens[8] == "right-low") 
                instance[49] = 1;
            else if (tokens[8] == "central") 
                instance[50] = 1;

            if (tokens[9] == "yes") {
                labels.push_back(0.99);
            } else if (tokens[9] == "no") {
                labels.push_back(0.01);
            }

            instances.push_back(instance);
            string istring = "";
            for (int i = 0; i < instance.size(); i++) {
                if (instance[i] == 0) {
                    istring += "0";
                } else {
                    istring+="1";
                }
            }
            cout << "Instance " << istring << endl;
        }
        file.close();
    }

    return make_pair(instances, labels);
}