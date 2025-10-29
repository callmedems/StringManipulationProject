// inspo for the main.cpp file
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

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

// Longest Common Subsequence
string lcs(const string &s1, const string &s2) {
    size_t n = s1.size();
    size_t m = s2.size();
    
    //this is for sampling large texts
        string sample1 = s1;
        string sample2 = s2;
    
    // tf the texts are too large, take meaningful samples from different parts
    if (n > 10000 || m > 10000) {
        size_t sampleSize = 5000;
        sample1 = s1.substr(0, sampleSize) + s1.substr(n/2, sampleSize) + s1.substr(n - sampleSize);
        sample2 = s2.substr(0, sampleSize) + s2.substr(m/2, sampleSize) + s2.substr(m - sampleSize);
        n = sample1.size();
        m = sample2.size();
    }
    
    // a dp table to store lengths of longest common subsequence
    vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));
    
    // filling the DP table
    for (size_t i = 1; i <= n; i++) {
        for (size_t j = 1; j <= m; j++) {
            if (sample1[i - 1] == sample2[j - 1])
                dp[i][j] = dp[i - 1][j - 1] + 1;
            else
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
        }
    }
    
    // reconstructing the LCS
    string result;
    size_t i = n, j = m;
    while (i > 0 && j > 0) {
        if (sample1[i - 1] == sample2[j - 1]) {
            result = sample1[i - 1] + result;
            i--;
            j--;
        } else if (dp[i - 1][j] > dp[i][j - 1]) {
            i--;
        } else {
            j--;
        }
    }
    
    return result;
}

// process text to remove unnecessary characters and normalize case
string preprocessText(const string &text) {
    string result;
    result.reserve(text.size());
    
    for (char c : text) {
        // Keep only letters, spaces, and basic punctuation
        if (isalnum(c) || c == ' ' || c == '.' || c == ',' || c == '!' || c == '?') {
            result.push_back(tolower(c));
        }
    }
    return result;
}


// reads the file content into a string, up to 50,000 chars
string readFile(const string &filename, size_t limit = 50000) {
    ifstream in(filename, ios::in | ios::binary);
    if (!in) {
        throw runtime_error("Error opening file: " + filename);
    }
    string contents;
    contents.resize(limit);
    in.read(&contents[0], limit);
    contents.resize(in.gcount());
    return contents;
}

//main
int main() {
    try {
        string file1 = "Mrs_Dalloway_in_Bond_Street.txt";
        string file2 = "a_modest_proposal.txt";

        string s1 = readFile(file1, 50000);
        string s2 = readFile(file2, 50000);  

        cout << "Loaded " << s1.size() << " chars from " << file1 << "\n";
        cout << "Loaded " << s2.size() << " chars from " << file2 << "\n\n";

        // prepares text to reduce unnecessary characters and improve performance
        cout << "Preprocessing text...\n";
        s1 = preprocessText(s1);
        s2 = preprocessText(s2);
        cout << "After preprocessing: " << s1.size() << " and " << s2.size() << " characters\n\n";

        // Longest Common Substring
        string substring = lcSubString(s1, s2);
        cout << "\nLongest Common Substring (length " << substring.size() << "):\n";
        cout << substring << "\n\n";

        // Longest Common Subsequence
        string subsequence = lcs(s1, s2);
        cout << "\nLongest Common Subsequence (length " << subsequence.size() << "):\n";
        cout << subsequence << "\n";

    } catch (exception &e) {
        cerr << e.what() << "\n";
        return 1;
    }
    return 0;
}
