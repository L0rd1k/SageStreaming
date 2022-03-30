/*
A pangram is a sentence where every letter of the English alphabet appears at least once.
Given a string sentence containing only lowercase English letters, return true if sentence is a pangram, or false otherwise.

Example 1:
Input: sentence = "thequickbrownfoxjumpsoverthelazydog"
Output: true
Explanation: sentence contains at least one of every letter of the English alphabet.

Example 2:
Input: sentence = "leetcode"
Output: false

Constraints:
1 <= sentence.length <= 1000
sentence consists of lowercase English letters.
*/

#include <iostream>
#include <unordered_map>
#include <string>

class Solution {
public:
    bool checkIfPangram(std::string sentence) {
        std::unordered_map<int, int> mapVec;
        for (int i = 97; i < 123; i++) { 
            mapVec[i]++; 
        } 
        for(auto elem : sentence) {
            if(mapVec.find(elem) != mapVec.end()) {
                mapVec.erase(elem);
            }
        }
        if(mapVec.empty()) 
            return true;
        else
            return false;
    }
};

int main() {
    Solution solution;
    std::cout << solution.checkIfPangram("thequickbrownfoxjumpsoverthelazydog") << std::endl;
}