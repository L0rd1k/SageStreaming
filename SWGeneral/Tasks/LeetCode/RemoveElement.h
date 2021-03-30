#pragma once
#include <vector>
#include <iostream>

class RemoveElement {
public:
    RemoveElement();
    virtual ~RemoveElement();
private:
    int solution(std::vector<int>& nums, int val);
};