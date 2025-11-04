#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <unordered_set>
#include <sstream>
#include "similarity.hpp"

using namespace std;

// STAGE 5: LEVENSHTEIN EDIT DISTANCE

int levenshteinDistance(const string& s1, const string& s2) {
    size_t n = s1.size();
    size_t m = s2.size();
    
    // If one string is empty, distance is the length of the other
    if (n == 0) return m;
    if (m == 0) return n;
    
    // Use space-optimized version: only need previous and current row
    vector<int> prev(m + 1);
    vector<int> curr(m + 1);
    
    // Initialize first row
    for (size_t j = 0; j <= m; j++) {
        prev[j] = j;
    }
    
    // Fill the DP table row by row
    for (size_t i = 1; i <= n; i++) {
        curr[0] = i;
        
        for (size_t j = 1; j <= m; j++) {
            if (s1[i - 1] == s2[j - 1]) {
                curr[j] = prev[j - 1]; // No operation needed
            } else {
                curr[j] = 1 + min({
                    prev[j],      // Deletion
                    curr[j - 1],  // Insertion
                    prev[j - 1]   // Substitution
                });
            }
        }
        
        swap(prev, curr);
    }
    
    return prev[m];
}

void levenshteinComparison(const string& text1, const string& text2, 
const string& name1, const string& name2) {
    cout << "\n========================================\n";
    cout << "STAGE 5A: LEVENSHTEIN EDIT DISTANCE\n";
    cout << "========================================\n";
    cout << name1 << ": " << text1.size() << " chars\n";
    cout << name2 << ": " << text2.size() << " chars\n\n";
    
    size_t n1 = text1.size();
    size_t n2 = text2.size();
    
    // Levenshtein has O(N*M) complexity
    // For large texts, we need to sample
    const size_t maxSampleSize = 5000;
    
    int distance = 0;
    double similarity = 0.0;
    
    auto start = chrono::high_resolution_clock::now();
    
    if (n1 <= maxSampleSize && n2 <= maxSampleSize) {
        cout << "Computing Levenshtein distance on full texts...\n";
        distance = levenshteinDistance(text1, text2);
        
        size_t maxLen = max(n1, n2);
        similarity = (1.0 - (double)distance / maxLen) * 100.0;
        
    } else {
        cout << "Texts too large. Using multiple sample comparisons.\n";
        cout << "Sample size: " << maxSampleSize << " characters\n\n";
        
        // Sample from beginning, middle, end of both texts
        vector<pair<size_t, size_t>> samplePositions = {
            {0, 0},                              // Beginning
            {n1 / 4, n2 / 4},                    // First quarter
            {n1 / 2, n2 / 2},                    // Middle
            {3 * n1 / 4, 3 * n2 / 4},           // Third quarter
            {n1 > maxSampleSize ? n1 - maxSampleSize : 0, 
             n2 > maxSampleSize ? n2 - maxSampleSize : 0}  // End
        };
        
        double totalSimilarity = 0.0;
        int numSamples = 0;
        
        for (const auto& pos : samplePositions) {
            size_t len1 = min(maxSampleSize, n1 - pos.first);
            size_t len2 = min(maxSampleSize, n2 - pos.second);
            
            string sample1 = text1.substr(pos.first, len1);
            string sample2 = text2.substr(pos.second, len2);
            
            int dist = levenshteinDistance(sample1, sample2);
            size_t maxLen = max(len1, len2);
            double sim = (1.0 - (double)dist / maxLen) * 100.0;
            
            totalSimilarity += sim;
            numSamples++;
        }
        
        similarity = totalSimilarity / numSamples;
        distance = -1; // Indicate we used sampling
        
        cout << "Analyzed " << numSamples << " sample pairs.\n";
    }
    
    auto end = chrono::high_resolution_clock::now();
    double elapsed_ms = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    
    cout << "\nRESULTS:\n";
    if (distance >= 0) {
        cout << "Edit distance: " << distance << " operations\n";
    } else {
        cout << "Edit distance: Computed from samples\n";
    }
    cout << "Similarity percentage: " << fixed << setprecision(4)
         << similarity << "%\n";
    cout << "Runtime: " << elapsed_ms << " ms\n\n";
    cout << "========================================\n\n";
}

// STAGE 5: JACCARD SIMILARITY WITH N-GRAMS

// Generate n-grams from text
unordered_set<string> generateNGrams(const string& text, int n) {
    unordered_set<string> ngrams;
    
    if (text.size() < static_cast<size_t>(n)) {
        ngrams.insert(text);
        return ngrams;
    }
    
    for (size_t i = 0; i <= text.size() - n; i++) {
        ngrams.insert(text.substr(i, n));
    }
    
    return ngrams;
}

// Generate word-based n-grams
unordered_set<string> generateWordNGrams(const string& text, int n) {
    unordered_set<string> ngrams;
    istringstream iss(text);
    vector<string> words;
    string word;
    
    while (iss >> word) {
        words.push_back(word);
    }
    
    if (words.size() < static_cast<size_t>(n)) {
        string combined;
        for (const auto& w : words) {
            combined += w + " ";
        }
        ngrams.insert(combined);
        return ngrams;
    }
    
    for (size_t i = 0; i <= words.size() - n; i++) {
        string ngram;
        for (int j = 0; j < n; j++) {
            ngram += words[i + j];
            if (j < n - 1) ngram += " ";
        }
        ngrams.insert(ngram);
    }
    
    return ngrams;
}

// Calculate Jaccard Similarity coefficient
double jaccardSimilarity(const string& text1, const string& text2, int n) {
    auto ngrams1 = generateNGrams(text1, n);
    auto ngrams2 = generateNGrams(text2, n);
    
    // Calculate intersection
    int intersection = 0;
    for (const auto& ng : ngrams1) {
        if (ngrams2.count(ng)) {
            intersection++;
        }
    }
    
    // Calculate union
    int unionSize = ngrams1.size() + ngrams2.size() - intersection;
    
    if (unionSize == 0) return 0.0;
    
    return (double)intersection / unionSize;
}

void jaccardComparison(const string& text1, const string& text2, 
const string& name1, const string& name2) {
    cout << "\n========================================\n";
    cout << "STAGE 5B: JACCARD SIMILARITY (N-GRAMS)\n";
    cout << "========================================\n";
    cout << name1 << ": " << text1.size() << " chars\n";
    cout << name2 << ": " << text2.size() << " chars\n\n";
    
    auto start = chrono::high_resolution_clock::now();
    
    // testing with different n-gram sizes
    vector<int> ngramSizes = {3, 5, 10};
    
    cout << "Computing Jaccard similarity with character n-grams:\n";
    cout << "----------------------------------------------------\n";
    
    for (int n : ngramSizes) {
        double similarity = jaccardSimilarity(text1, text2, n);
        cout << "N-gram size " << n << ": " 
             << fixed << setprecision(4) << (similarity * 100) << "%\n";
    }
    
    // Word-based n-grams
    cout << "\nComputing Jaccard similarity with word n-grams:\n";
    cout << "------------------------------------------------\n";
    
    vector<int> wordNgramSizes = {2, 3, 4};
    
    for (int n : wordNgramSizes) {
        auto ngrams1 = generateWordNGrams(text1, n);
        auto ngrams2 = generateWordNGrams(text2, n);
        
        int intersection = 0;
        for (const auto& ng : ngrams1) {
            if (ngrams2.count(ng)) {
                intersection++;
            }
        }
        
        int unionSize = ngrams1.size() + ngrams2.size() - intersection;
        double similarity = (unionSize > 0) ? (double)intersection / unionSize : 0.0;
        
        cout << "Word " << n << "-grams: " 
             << fixed << setprecision(4) << (similarity * 100) << "%\n";
    }
    
    auto end = chrono::high_resolution_clock::now();
    double elapsed_ms = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    
    // Use character 5-grams as the primary metric
    double primarySimilarity = jaccardSimilarity(text1, text2, 5) * 100;
    
    cout << "\nRESULTS (using 5-character n-grams):\n";
    cout << "Jaccard similarity: " << fixed << setprecision(4)
         << primarySimilarity << "%\n";
    cout << "Runtime: " << elapsed_ms << " ms\n\n";
    cout << "========================================\n\n";
}