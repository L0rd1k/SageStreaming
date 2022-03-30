/*
Given two strings s and t, return true if t is an anagram of s, and false otherwise.

Example 1:
Input: s = "anagram", t = "nagaram"
Output: true

Example 2:
Input: s = "rat", t = "car"
Output: false
 
Constraints:
1 <= s.length, t.length <= 5 * 104
s and t consist of lowercase English letters.
Follow up: What if the inputs contain Unicode characters? How would you adapt your solution to such a case?
*/

#include <iostream>
#include <unordered_map>
#include <string>

class Solution {
public:
    bool isAnagram(string s, string t) {
        int recognizer_array[26];
        memset(recognizer_array,0,sizeof(int)*26);
        for(int i=0;i<s.size();i++){
            char temp = s[i]-'a';
            recognizer_array[temp]++;
        }
        for(int i=0; i < t.size(); i++){
            char temp = t[i]-'a';
            recognizer_array[temp]--;
        }
        for(int i=0;i<26;i++){
            if(recognizer_array[i]!=0)
                return false;
        }
        return true;
    }
};

int main() {
    Solution solution;
    std::cout << solution.isAnagram("anagram","nagaram") << std::endl;
}