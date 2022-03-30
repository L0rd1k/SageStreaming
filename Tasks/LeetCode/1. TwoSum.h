#pragma once

#include <iostream>
#include <vector>
#include <stdexcept>
#include <map>
#include <unordered_map>

class TwoSum {
public:
    TwoSum();
    virtual ~TwoSum();
private:
    std::vector<int> bruteForce(std::vector<int>& nums, int target);
    std::vector<int> hashTable(std::vector<int>& nums, int target);

};