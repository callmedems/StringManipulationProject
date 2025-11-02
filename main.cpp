#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>

using namespace std;

// Lee todo el archivo en un string
string readFile(const string& filename) {
    ifstream in(filename);
    if (!in.is_open()) throw runtime_error("Error opening file: " + filename);
    ostringstream ss;
    ss << in.rdbuf();
    return ss.str();
}

// Cuenta el número de palabras en un texto
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

int main() {
    try {
        string pride = readFile("../texts/prideAndPrejudice.txt");
        string sense = readFile("../texts/senseAndSensibility.txt");

        cout << "Pride and Prejudice word count: "
                  << countWords(pride) << "\n";
        cout << "Sense and Sensibility word count: "
                  << countWords(sense) << "\n";

    } catch (const exception& e) {
        cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
