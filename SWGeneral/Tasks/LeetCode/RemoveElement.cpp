#include "RemoveElement.h"

RemoveElement::RemoveElement() {
    std::vector<int> vecInput = {0,1,2,2,3,0,4,2};
    std::cout << "Vec size: " << solution(vecInput, 2) << std::endl;
}

RemoveElement::~RemoveElement() {

}

int RemoveElement::solution(std::vector<int>& nums, int val) {
    int counter = 0;
    int pastePos = 0;
    for(int i = 0; i < nums.size(); i++) {
        if(nums.at(i) != val) {
            counter++;
            nums[pastePos] = nums[i];
            pastePos++;    
        }
    }

    for(auto itr = 0; itr < counter; itr++) {
        std::cout << nums[itr] << " ";
    }
    std::cout << std::endl;
    return counter;
}