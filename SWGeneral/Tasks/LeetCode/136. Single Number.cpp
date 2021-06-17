/*
Given a non-empty array of integers nums, every element appears twice except for one. Find that single one.
You must implement a solution with a linear runtime complexity and use only constant extra space.
Example 1:
Input: nums = [2,2,1]
Output: 1
Example 2:
Input: nums = [4,1,2,1,2]
Output: 4
Example 3:
Input: nums = [1]
Output: 1
Constraints:
1 <= nums.length <= 3 * 10^4
-3 * 10^4 <= nums[i] <= 3 * 10^4
Each element in the array appears twice except for one element which appears only once.
Accepted
*/

#include <iostream>
#include <unordered_map>
#include <string>

/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */

class Solution {
public:
// SOLUTION1
    int singleNumber(vector<int>& nums) {
        std::vector<int> temp_vec;
        int sum_of_elem = 0, sum_of_elems_s = 0;
        for(auto elem : nums) {
            if(!(std::find(temp_vec.begin(), temp_vec.end(), elem) != temp_vec.end())) {
                temp_vec.push_back(elem);       
                sum_of_elems_s += elem;
            }
            sum_of_elem += elem;
        }

        return sum_of_elems_s - (sum_of_elem - sum_of_elems_s);
    }

    int singleNumber(vector<int>& nums) {
        std::set<int> temp_vec;
        int sum_of_elem = 0, sum_of_elems_s = 0;
        for(int i = 0; i < nums.size(); i++) {
            if(!(std::find(temp_vec.begin(), temp_vec.end(), nums[i]) != temp_vec.end())) {
                temp_vec.insert(nums[i]);       
            } else {
                temp_vec.erase(nums[i]);
            }
        }
        return *(temp_vec.begin());
    }

    int singleNumber(vector<int>& nums) {
        if(nums.size() == 1) {
            return nums[0];
        }
        
        std::sort(nums.begin(), nums.end());
        
        for(int i = 0; i < nums.size() - 1; i++) {
            if(nums[i] != nums[i+1])
                return nums[i];
            else 
                i++;
        }
        
        return nums[nums.size() - 1];
    }
};

int main() {
    Solution solution;
    [4,1,2,1,2]
    std::cout << solution.singleNumber() << std::endl;
}