/*
Given integer array nums, return the third maximum number in this array. If the third maximum does not exist, return the maximum number.
Example 1:
Input: nums = [3,2,1]
Output: 1
Explanation: The third maximum is 1.
Example 2:
Input: nums = [1,2]
Output: 2
Explanation: The third maximum does not exist, so the maximum (2) is returned instead.
Example 3:
Input: nums = [2,2,3,1]
Output: 1
Explanation: Note that the third maximum here means the third maximum distinct number.
Both numbers with value 2 are both considered as second maximum.
Constraints:
1 <= nums.length <= 104
-231 <= nums[i] <= 231 - 1
Follow up: Can you find an O(n) solution?
*/

class Solution {
public:
    int thirdMax(vector<int>& nums) {
        long max = LONG_MIN;
        long secondmax = LONG_MIN;
        long thirdmax = LONG_MIN;
        
        for (int n : nums) {
            if (n == max || n == secondmax || n == thirdmax) continue;
            if (n > max) {
                std::cout << "n > max : " << n << " " << max << std::endl; 
                thirdmax = secondmax;
                secondmax = max;
                max = n;
            } else if (n > secondmax) {
                std::cout << "n > secondmax : " << n << " " << secondmax << std::endl; 
                thirdmax = secondmax;
                secondmax = n;
            } else if (n > thirdmax) {
                std::cout << "n > thirdmax : " << n << " " << thirdmax << std::endl; 
                thirdmax = n;
            }
        }
        return (thirdmax == LONG_MIN ? max : thirdmax);
    }

    int thirdMaxSecondSolution(vector<int>& nums) {
        std::set<int> numbers;
        for(auto elem : nums) {
            numbers.insert(elem);
        }
        if(numbers.size() < 3) {
            return *(--numbers.end());
        }
        return *std::next(numbers.begin(), numbers.size() - 3);
    }

};



int main() {
    Solution solution;
    std::cout << solution.thirdMax({2,2,3,1}) << std::endl;
}