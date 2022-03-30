/*
A sentence is a list of words that are separated by a single space with no leading or trailing spaces. Each word consists of lowercase and uppercase English letters.
A sentence can be shuffled by appending the 1-indexed word position to each word then rearranging the words in the sentence.
For example, the sentence "This is a sentence" can be shuffled as "sentence4 a3 is2 This1" or "is2 sentence4 This1 a3".
Given a shuffled sentence s containing no more than 9 words, reconstruct and return the original sentence.

Example 1:
Input: s = "is2 sentence4 This1 a3"
Output: "This is a sentence"
Explanation: Sort the words in s to their original positions "This1 is2 a3 sentence4", then remove the numbers.
Example 2:
Input: s = "Myself2 Me1 I4 and3"
Output: "Me Myself and I"
Explanation: Sort the words in s to their original positions "Me1 Myself2 and3 I4", then remove the numbers.

Constraints:
2 <= s.length <= 200
s consists of lowercase and uppercase English letters, spaces, and digits from 1 to 9.
The number of words in s is between 1 and 9.
The words in s are separated by a single space.
s contains no leading or trailing spaces.
*/

class Solution {
public:
    string sortSentence(string s) {
        int start = 0;
        int end = s.find(" ");
        std::map<int, string> map_test;
        while(end != std::string::npos) {
            std::string name_s = s.substr(start, end - start);
            map_test.insert({(int)name_s[name_s.length()-1] - 48,name_s.substr(0, name_s.length() - 1)});
            start = end + 1;
            end = s.find(" ", start);
        }
        std::string t_name = s.substr(start, end - start);
        map_test.insert({(int)t_name[t_name.length()-1] - 48,t_name.substr(0, t_name.length() - 1)});
        std::string result_str;
        
        for (int i = 1; i <= map_test.size() - 1; i++) {
            result_str += map_test[i] + " ";
        } 
        result_str += map_test[map_test.size()];
        
        return result_str;
    }
};

int main() {
    Solution solution;
    std::cout << solution.sortSentence("is2 sentence4 This1 a3") << std::endl;
}