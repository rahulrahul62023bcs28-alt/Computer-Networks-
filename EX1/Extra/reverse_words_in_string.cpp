#include <iostream>
#include <string>
#include <vector>
#include <sstream>

class Solution {
public:
    std::string reverseWords(std::string s) {
        std::vector<std::string> words;
        std::stringstream ss(s);
        std::string word;
        while (ss >> word) {
            words.push_back(word);
        }
        std::string result = "";
        for (int i = static_cast<int>(words.size()) - 1; i >= 0; --i) {
            result += words[i];
            if (i != 0) {
                result += " ";
            }
        }
        return result;
    }
};
