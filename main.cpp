#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include "cleanText.hpp"

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

int main() {
    try {
        //Stage one 
        string pride = readFile("../texts/prideAndPrejudice.txt");
        string sense = readFile("../texts/senseAndSensibility.txt");

        cout<< "Pride and Prejudice word count: "
                  << countWords(pride) << "\n";
        cout<< "Sense and Sensibility word count: "
                  << countWords(sense) << "\n";
        //Stage two
        pride = cleanText(pride);
        sense = cleanText(sense);

        cout << "Pride and Prejudice word count after cleaning: "
             << countWords(pride) << "\n";

        cout << "Sense and Sensibility word count after cleaning "
             << countWords(sense) << "\n";


        

    } catch (const exception& e) {
        cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
