/*
Given an array nums of size n, return the majority element.
The majority element is the element that appears more than ⌊n / 2⌋ times. You may assume that the majority element always exists in the array.
Example 1:
Input: nums = [3,2,3]
Output: 3
Example 2:
Input: nums = [2,2,1,1,1,2,2]
Output: 2
Constraints:
n == nums.length
1 <= n <= 5 * 104
-231 <= nums[i] <= 231 - 1
Follow-up: Could you solve the problem in linear time and in O(1) space?
*/

class Solution {
public:
     int majorityElement(vector<int>& nums) {
//1. HASH MAP - Time complexity : O(n) Space complexity : O(n)
//         std::map<int, int> nums_c;
//         for(auto elem : nums) {
//             if(!nums_c.count(elem)) {
//                 nums_c.insert({elem, 1});
//             } else {
//                 nums_c[elem]++;
//             } 
//         }   
//         auto pr = std::max_element(std::begin(nums_c), std::end(nums_c),[] 
//                 (const std::pair<int,int>& p1, const std::pair<int,int>& p2) {
//                 return p1.second < p2.second;
//             }
//         );
//         return pr->first;
      
//2. Boyer-Moore Voting Algorithm  Time complexity : O(n) Space complexity : O(1) 
//         int count = 0;
//         int candidate = 0;
//         for (int num : nums) {
//             if (count == 0) {
//                 candidate = num;
//             }       
//             std::cout << num << " " << candidate << " " << count << std::endl;
//             if(num == candidate) {
//                 count+=1;
//             } else {
//                 count+=-1;
//             }
//         }
//	return candidate;


//2. SORTING Time complexity : O(n) Space complexity : O(1) or O(n)
        std::sort(nums.begin(), nums.end());
        return nums[nums.size()/2];
        
    }
};

int main() {
    Solution solution;
    std::cout << solution.majorityElement({2,2,1,1,1,2,2}) << std::endl;
}