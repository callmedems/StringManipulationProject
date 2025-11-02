#include "cleanText.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <regex>
#include <cctype>
#include <unordered_set>
#include <sstream>

using namespace std;

// Read full file
string readFile(const string& filename) {
    ifstream in(filename);
    if (!in.is_open()) throw runtime_error("Error opening file: " + filename);
    ostringstream ss;
    ss << in.rdbuf();
    return ss.str();
}

// Remove project Gutenberg header and footer
string removePG_header(const string& text) {
    string cleaned = text;
    size_t start= cleaned.find("*** START OF");
    size_t end= cleaned.find("*** END OF");

    //remove all before "START OFF"

    if (start != string::npos){
        size_t newline = cleaned.find('\n', start);
        if (newline != string::npos)
            cleaned = cleaned.substr(newline + 1);
        else
            cleaned = cleaned.substr(start); 
    }

    //remove all befafter "END OFF"

    if (end != string::npos) {
        cleaned = cleaned.substr(0, end);
    }
    return cleaned;
}



// 
string cleanText(const string& input) {
    string text=removePG_header(input);
    cerr << "[DEBUG] Cleaned text length: " << text.size() << endl;
    return text;
}
