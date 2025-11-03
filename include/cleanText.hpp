#ifndef PREPROCESSING_HPP
#define PREPROCESSING_HPP

#include <string>
#include <vector>
#include <unordered_set>

using namespace std;


// Reads content into a string
string readFile(const string& filename);

// Removes project Gutenberg metadata 
string removePG_header(const string& text);

// Performs normalization: lowercase
string normalize(const string& input);

// Removes stopwords
string stopwords(const string& text);

string cleanText(const string& input);

#endif
