#ifndef SIMILARITY_HPP
#define SIMILARITY_HPP
#include <string>

using namespace std;

// Stage 5: Levenshtein Edit Distance
int levenshteinDistance(const string& s1, const string& s2);

void levenshteinComparison(const string& text1, const string& text2, 
const string& name1, const string& name2);

// Stage 5: Jaccard Similarity using N-grams
double jaccardSimilarity(const string& text1, const string& text2, int n);

void jaccardComparison(const string& text1, const string& text2, 
const string& name1, const string& name2);

#endif