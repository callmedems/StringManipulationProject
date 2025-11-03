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
    
    regex start_pattern(R"(·PRIDE\s+AND\s+PREJUDICE·)", regex_constants::icase);
    smatch start_match;
    size_t start_pos = 0;

    if (regex_search(cleaned, start_match, start_pattern)) {
        start_pos = cleaned.find('\n', start_match.position());
        if (start_pos != string::npos)
            cleaned = cleaned.substr(start_pos + 1);
    }

    //
    regex end_pattern(R"(\*\*\*\s*END OF[^*]+EBOOK[^*]+\*\*\*)", regex_constants::icase);
    smatch end_match;
    if (regex_search(cleaned, end_match, end_pattern)) {
        size_t end_pos = end_match.position();
        if (end_pos != string::npos)
            cleaned = cleaned.substr(0, end_pos);
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
        "the","a","an","and","or","but","in","on","at","to","for","of","with","by","from","as","is","was","were","be","been","being","have","has","had","do","does","did","will","would","should","could","may","might","must","can","shall","it","its","this","that","these","those","i","you","he","she","we","they","him","her","us","them","his","hers","our","their","what","which","who","when","where","why","how","if","then","than","so","said", "illustration", "chapter", "mr", "mrs", "miss", "ms", "there", "too", "not", "me", "no", "my", "your", "all", "any", "some", "such", "only", "own", "same", "other", "more", "most", "many", "much", "very", "just", "even", "also", "oh", "am", "sir", "lady"
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
