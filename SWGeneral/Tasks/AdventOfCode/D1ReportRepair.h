#pragma once

#include "FileHandler.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <unordered_set>
#include <algorithm>


class D1ReportRepair : public FileHandler {
public:
    D1ReportRepair();
    ~D1ReportRepair();
private:
    void bruteForceSolution(); // BruteForce Approach O(n^2)
    void hashSetSolution();
    void pointerSolution(); // Two Pointer Approach O(nlogn) 
    int parseDataFile(std::string filePath) override;
private:
    std::vector<int> vec_input;
    std::unordered_set<int> complements;
    std::ifstream file;
};