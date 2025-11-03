#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include "lcs.hpp"

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
    cout << name1 << ": " << text1.size() << " chars\n";
    cout << name2 << ": " << text2.size() << " chars\n\n";

    const size_t maxText_size = 15000; // Si ambos son menores, lo corremos sin muestreo
    size_t sampleSize = 8000;              // Tamaño de la ventana de comparación
    size_t overlap = 2000;                 // Solapamiento entre ventanas

    size_t n1 = text1.size();
    size_t n2 = text2.size();
    string bestSubstring;
    size_t bestLen = 0;

    auto start = chrono::high_resolution_clock::now();

    // execute lcs if texts are not too large)
    if (n1 <= maxText_size && n2 <= maxText_size) {
        cout << "Texts are small enough, running full O(N*M) algorithm.\n";
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
    cout << "LCS Fragment:\n";
    if (bestSubstring.size() > 300)
        cout << bestSubstring.substr(0, 300) << "...\n";
    else
        cout << bestSubstring << "\n";
}