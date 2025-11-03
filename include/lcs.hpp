#ifndef LCS_HPP
#define LCS_HPP

#include <string>
#include <vector>

using namespace std;


string lcSubString(const string& s1, const string& s2);

//similarity analysis using longest common substring
void lcsComparision(const string& text1, const string& text2,
                    const string& name1, const string& name2);

//similarity analysis using longest common subsequence
int longestCommonSubsequence(const string& text1, const string& text2);


//Get the longest common subsequence
string getLCSString(const string& text1, const string& text2);

void lcsBlockComparison(const string& text1, const string&  text2, const string& name1, const string& name2);

#endif