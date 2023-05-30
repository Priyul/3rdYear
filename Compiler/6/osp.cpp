#include <iostream>
#include <cstring>
#include <iomanip>
#include <cstdlib>
#include <string>
#include <cctype>
using namespace std;

// global
string s = "ACACACBAACBAB";

string matchPattern(string& s, int start = 0);

string matchPattern(string& s, int start) {
    if (start == s.size())
        return "";

    if (s.substr(start, 4) == "ACBA") {
        string remaining = matchPattern(s, start+4);
        if (remaining != "mapping not possible") {
            return "X" + remaining;
        }
    }

    if (s.substr(start, 3) == "BAB") {
        string remaining = matchPattern(s, start+3);
        if (remaining != "mapping not possible") {
            return "Y" + remaining;
        }
    }

    if (s.substr(start, 2) == "AC") {
        string remaining = matchPattern(s, start+2);
        if (remaining != "mapping not possible") {
            return "Z" + remaining;
        }
    }

    return "mapping not possible";
}

int main()
{
    if (s == "") {
        cout << "mapping not possible";
        return 0;
    }

    s = matchPattern(s);
    cout << s << endl;

    return 0;
}