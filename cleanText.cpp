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

    //remove all after "END OFF"
    if (end != string::npos) {
        cleaned = cleaned.substr(0, end);
    }
    return cleaned;
}

// Remove punctuation and special chars, normalize spaces
string normalize(const string& input) {
    string lower;
    lower.reserve(input.size());

    //Leaves only words and spaces
    for (char c : input) {
        if (isalpha(static_cast<unsigned char>(c)) ||
            isspace(static_cast<unsigned char>(c))) {
            lower.push_back(tolower(static_cast<unsigned char>(c)));
        }
    }

    // Corrects double space with single space
    string normalized;
    bool prevSpace = false;
    for (char c : lower) {
        if (isspace(static_cast<unsigned char>(c))) {
            if (!prevSpace) {
                normalized.push_back(' ');
                prevSpace = true;
            }
        } else {
            normalized.push_back(c);
            prevSpace = false;
        }
    }

    return normalized;
}

// Stopword removal
string stopwords(const string& text) {
    static const unordered_set<string> textStopwords = {
        "the","a","an","and","or","but","in","on","at","to","for","of","with","by","from","as","is","was","were","be","been","being","have","has","had","do","does","did","will","would","should","could","may","might","must","can","shall","it","its","this","that","these","those","i","you","he","she","we","they","him","her","us","them","his","hers","our","their","what","which","who","when","where","why","how","if","then","than","so","said"
    };

    istringstream iss(text);
    ostringstream oss;
    string word;

    while (iss >> word) {
        if (!textStopwords.count(word))
            oss << word << " ";
    }
    return oss.str();
}




// 
string cleanText(const string& input) {
    string text=removePG_header(input);
    text=normalize(text);
    text=stopwords(text);

    return text;
}
