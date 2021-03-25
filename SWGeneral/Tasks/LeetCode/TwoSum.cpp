#include "TwoSum.h"

TwoSum::TwoSum() {
    std::vector<int> test_vec {2,7,11,15};
    bruteForce(test_vec, 18);
    hashTable(test_vec, 18);
}

TwoSum::~TwoSum() {
}

std::vector<int> TwoSum::bruteForce(std::vector<int>& nums, int target) {
    for(int i = 0; i < nums.size() - 1; i++) {
        for(int j = i+1; j < nums.size(); j++) {
            if(nums.at(i) + nums.at(j) == target) {
                std::cout << i << " " << j << "\n";
                return std::vector<int> { i , j };
            }
        }
    }
    throw std::invalid_argument( "No two sum solution");
}

std::vector<int> TwoSum::hashTable(std::vector<int>& nums, int target) {
    std::unordered_map<int, int> test_map;
    for(int i = 0; i < nums.size(); i++) {
        int difference = target - nums[i];
        std::cout << test_map[difference] << std::endl;
        if(test_map.count(difference)) {
            return std::vector<int> {i, test_map[difference]};
        } 
        test_map.insert(std::pair<int,int>(nums[i], i));
    }
    throw std::invalid_argument( "No two sum solution"); 
}