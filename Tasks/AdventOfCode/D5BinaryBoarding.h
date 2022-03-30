#pragma once

#include "FileHandler.h"
#include <vector>
#include <iostream>
#include <algorithm>
class D5BinaryBoarding : public FileHandler {
public:
	D5BinaryBoarding();
	~D5BinaryBoarding();
private:
	int parseDataFile(std::string filePath) override;
	void executeSolver();
private:
	std::ifstream file;
	int max_value = 0;
	std::vector<int> id_values;
	std::string seat_partitioning;
};

