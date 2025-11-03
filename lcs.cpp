#include "lcs.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <sstream>
#include <iomanip>

using namespace std;

// Longest Common Substring
string lcSubString(const string &s1, const string &s2) {
    size_t n = s1.size();
    size_t m = s2.size();

    vector<int> dp((m + 1), 0);
    int maxLen = 0;
    int endPos = 0;

    for (size_t i = 1; i <= n; i++) {
        for (size_t j = m; j > 0; j--) {
            if (s1[i - 1] == s2[j - 1]) {
                dp[j] = dp[j - 1] + 1;
                if (dp[j] > maxLen) {
                    maxLen = dp[j];
                    endPos = i;
                }
            } else {
                dp[j] = 0;
            }
        }
    }

    if (maxLen == 0) return "";
    return s1.substr(endPos - maxLen, maxLen);
}


void lcsComparision(const string& text1, const string& text2,
const string& name1, const string& name2) {
    cout << "Finding longest common substring\n";

    const size_t maxText_size = 15000; //Numero de caracteres tolerado sin overlap
    size_t sampleSize = 8000;              // Tamaño de chrts de la ventana de comparación
    size_t overlap = 2000;                 // Solapamiento entre ventanas

    size_t n1 = text1.size();
    size_t n2 = text2.size();
    string bestSubstring;
    size_t bestLen = 0;

    auto start = chrono::high_resolution_clock::now();

    // execute lcs if texts are not too large)
    if (n1 <= maxText_size && n2 <= maxText_size) {
        cout << "Texts are small enough, run lcs\n";
        bestSubstring = lcSubString(text1, text2);
        bestLen = bestSubstring.size();
        
    } else {
        cout << "Runnin overlapping: sampleSize: "
             << sampleSize << ", overlap: " << overlap << ").\n";

        // Iterar sobre el texto 1
        for (size_t i = 0; i < n1; i += (sampleSize - overlap)) {
            string block1 = text1.substr(i, sampleSize);

            for (size_t j = 0; j < n2; j += (sampleSize - overlap)) {
                string block2 = text2.substr(j, sampleSize);

                // Correr el algoritmo O(N*M) en los bloques
                string current = lcSubString(block1, block2);
                
                if (current.size() > bestLen) {
                    bestLen = current.size();
                    bestSubstring = current;
                }
            }
            // Si el texto restante es menor que sampleSize, paramos
            if (i + sampleSize >= n1 && i < n1) break;
        }

        auto end = chrono::high_resolution_clock::now();
        double elapsed_ms = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        
        cout << "Runtime of sampling: " << elapsed_ms << " ms\n\n";

    }
    
    // Cálculos y Reporte
    // Usamos el tamaño completo del texto para la métrica de similitud
    double avgLen = (static_cast<double>(n1) + static_cast<double>(n2)) / 2.0;
    double similarity = (bestLen / avgLen) * 100.0;

    cout << "Length of longest common substring (from samples): " << bestLen << "\n";
    cout << "Similarity percentage: " << fixed << setprecision(8)
         << similarity << "%\n";
         
    if (n1 > maxText_size || n2 > maxText_size) {
    } else {
         // Si se corrió completo, medimos el tiempo aqu
         auto end = chrono::high_resolution_clock::now();
         double elapsed_ms = chrono::duration_cast<chrono::milliseconds>(end - start).count();
         cout << "Runtime: " << elapsed_ms << " ms\n\n";
    }

    // Imprimir el fragmento
    cout <<"LCS Fragment:\n";
    if (bestSubstring.size() > 300)
        cout << bestSubstring.substr(0, 300) << "...\n";
    else
        cout << bestSubstring << "\n";
}


// STAGE 4: LONGEST COMMON SUBSEQUENCE (WORD-LEVEL)

// Split text into words
vector<string> splitIntoWords(const string& text) {
    vector<string> words;
    stringstream ss(text);
    string word;
    
    while (ss >> word) {
        words.push_back(word);
    }
    
    return words;
}

// Word-level LCS - returns length
int longestCommonSubsequenceWords(const vector<string>& words1, const vector<string>& words2) {
    int m = words1.size();
    int n = words2.size();
    
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
    
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (words1[i - 1] == words2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }
    
    return dp[m][n];
}

// Get the actual word-level LCS
vector<string> getLCSWords(const vector<string>& words1,
                           const vector<string>& words2) {
    int m = words1.size();
    int n = words2.size();
    
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
    
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (words1[i - 1] == words2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }
    
    // Backtrack
    vector<string> lcs;
    int i = m, j = n;
    
    while (i > 0 && j > 0) {
        if (words1[i - 1] == words2[j - 1]) {
            lcs.insert(lcs.begin(), words1[i - 1]);
            i--;
            j--;
        } else if (dp[i - 1][j] > dp[i][j - 1]) {
            i--;
        } else {
            j--;
        }
    }
    
    return lcs;
}

// Block-based word-level LCS comparison
void lcsBlockComparison(const string& text1, const string& text2, const string& name1, const string& name2) {
    
    cout << "Computing Longest Common Subsequence (word-level)...\n";
    cout << "Note: LCS finds words that appear in the same order, even if not consecutive.\n\n";
        
    // Split into words
    vector<string> words1 = splitIntoWords(text1);
    vector<string> words2 = splitIntoWords(text2);
    
    cout << "Text 1 (" << name1 << "): " << words1.size() << " words\n";
    cout << "Text 2 (" << name2 << "): " << words2.size() << " words\n\n";
    
    // For large texts, use sampling
    const size_t MAX_WORDS = 8000;
    const size_t sample_size = 6000;
    const size_t sample_overlap = 4800;
    
    size_t bestLcSub=0;
    vector<string> subseqWords;

    auto startTime = chrono::high_resolution_clock::now();
    if (words1.size() <= MAX_WORDS && words2.size() <= MAX_WORDS) {
        cout << "Both texts are within the word limit. Running full LCS computation.\n";
        bestLcSub= longestCommonSubsequenceWords(words1, words2);
        subseqWords = getLCSWords(words1, words2);
    }else{
        cout << "Running overlapping word sampling" << sample_size 
             << ", Overlap: " << sample_overlap<< ").\n";

        size_t step= sample_size - sample_overlap;

        if (step==0) step=1;
        for (size_t i = 0; i < words1.size(); i += step) {
            // Extraer bloque 1
            size_t end_idx1 = min(i + sample_size, words1.size());
            vector<string> block1(words1.begin() + i, words1.begin() + end_idx1);

            // Iterar sobre el texto 2
            for (size_t j = 0; j < words2.size(); j += step) {

                // Extraer bloque 2
                size_t end_idx2 = min(j + sample_size, words2.size());
                vector<string> block2(words2.begin() + j, words2.begin() + end_idx2);

                //getLCSWords para obtener tanto la longitud como la subsecuencia
                vector<string> currentLCSWords = getLCSWords(block1, block2);
                size_t currentLCS = currentLCSWords.size();
                
                // Actualizar el mejor LCS global
                if (currentLCS > bestLcSub) {
                    bestLcSub = currentLCS;
                    subseqWords = currentLCSWords;
                }
            }
            // Si ya se ha cubierto el final del texto, salimos
            if (end_idx1 == words1.size()) break; 
        }
    

    }
    
    cout << "\nComparing texts...\n";
    
    
    auto endTime = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(endTime - startTime);

    size_t totalWordsAvg = (words1.size() + words2.size()) / 2;
    // Si bestLCS es 0, evitamos la división por cero
    double similarityPercent = (totalWordsAvg > 0) ? (static_cast<double>(bestLcSub) / totalWordsAvg) * 100.0 : 0.0;
    
    // Report results
    cout << "RESULTS:\n";
    cout << "Longest Common Subsequence Length: " << bestLcSub << " words\n";
    cout << "Similarity Percentage: " << fixed << setprecision(2) 
              << similarityPercent << "%\n";
    cout << "Execution Time: " << duration.count() << " ms\n";
    cout << string(60, '-') << "\n\n";
    
    // Display fragment
    if (!subseqWords.empty()) {
        cout << "LCS Fragment Preview (first 150 words):\n";
        cout << "\"";
        
        size_t showWords = min(size_t(150), subseqWords.size());
        for (size_t i = 0; i <showWords; i++) {
            cout << subseqWords[i];
            if (i < showWords -1) cout << " ";
        }
        
        if (subseqWords.size() > 50) {
            cout << " ...";
        }
        cout << "\"\n\n";
        
        cout << "Total LCS words found: " << subseqWords.size() << "\n";
    } else {
        cout << "No common subsequence found.\n";
    }
    
    
}