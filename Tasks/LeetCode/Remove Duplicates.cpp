#include "RemoveDuplicates.h"

RemoveDuplicates::RemoveDuplicates() {
    std::vector<int> testVec{0,0,1,1,1,2,2,3,3,4};
    solution(testVec);
}

void RemoveDuplicates::solution(std::vector<int> &nums) {
    int counter = 0;
    if(nums.size() != 0) {
        for(auto i = nums.begin() + 1; i != nums.end(); i++) {
            if(*i != nums.at(counter)) {
                counter++;
                nums.at(counter) = nums.at(i - nums.begin());
            }
        } 
        std::cout << "Counter: " << counter++ << std::endl; 
        for(auto itr = 0; itr < counter; itr++) {
            std::cout << nums.at(itr) << " "; 
        }
        std::cout << std::endl;
    }    
}