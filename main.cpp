#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include "cleanText.hpp"
#include "lcs.hpp"
#include "similarity.hpp"

using namespace std;


//Counts number of words 
size_t countWords(const string& text) {
    size_t wordCount = 0;
    bool inWord = false;

    for (char c : text) {
        if (isspace(static_cast<unsigned char>(c))) {
            if (inWord) {
                wordCount++;
                inWord = false;
            }
        } else {
            inWord = true;
        }
    }
    if (inWord) wordCount++;
    return wordCount;
}

void printSeparator() {
    cout << "\n" << string(70, '=') << "\n\n";
}

void printHeader(const string& title) {
    cout << "\n" << string(70, '=') << "\n";
    cout << "  " << title << "\n";
    cout << string(70, '=') << "\n\n";
}

int main() {
    try {
        printHeader("TEXT SIMILARITY ANALYSIS PROJECT");
        
        // Stage 1: Text Selection and Reading
        printHeader("STAGE 1: TEXT SELECTION AND READING");
        
        string pride = readFile("./texts/prideAndPrejudice.txt");
        string sense = readFile("./texts/senseAndSensibility.txt");

        cout << "Original Text Wordcount:\n";
        cout << "  - Pride and Prejudice:      " << countWords(pride) << " words\n";
        cout << "  - Sense and Sensibility:    " << countWords(sense) << " words\n";
        
        // Stage 2: Text Preprocessing
        printHeader("STAGE 2: TEXT PREPROCESSING");
        
        pride = cleanText(pride);
        sense = cleanText(sense);

        cout << "Cleaned Text Wordcount:\n";
        cout << "  - Pride and Prejudice:      " << countWords(pride) << " words\n";
        cout << "  - Sense and Sensibility:    " << countWords(sense) << " words\n";

        // Stage 3: Longest Common Substring
        printHeader("STAGE 3: LONGEST COMMON SUBSTRING (LCSstr)");
        
        lcsComparision(pride, sense, "Pride and Prejudice", "Sense and Sensibility");

        // Stage 4: Longest Common Subsequence
        printHeader("STAGE 4: LONGEST COMMON SUBSEQUENCE (LCS)");
        
        lcsBlockComparison(pride, sense, "Pride and Prejudice", "Sense and Sensibility");

        printSeparator();
        
        // STAGE 5: Levenshtein Distance
        levenshteinComparison(pride, sense, "Pride and Prejudice", "Sense and Sensibility");
        
        // STAGE 5: Jaccard Similarity
        jaccardComparison(pride, sense, "Pride and Prejudice", "Sense and Sensibility");

        // Final Summary
        cout << "\n================================================\n";
        cout << "  ANALYSIS COMPLETE\n";
        cout << "================================================\n";

    } catch (const exception& e) {
        cerr << "\n[ERROR] " << e.what() << "\n\n";
        return 1;
    }
    return 0;
}