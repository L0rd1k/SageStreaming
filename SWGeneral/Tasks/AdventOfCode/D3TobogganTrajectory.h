#pragma once

#include "FileHandler.h"
#include <vector>
#include <iostream>

class D3TobogganTrajectory : public FileHandler {
public:
	D3TobogganTrajectory();
	~D3TobogganTrajectory();
private:
	int parseDataFile(std::string filePath) override;
	void executeSolution();
private:
	std::ifstream file;
	std::string line;
	std::vector<std::vector<char>> input_vec;
	size_t hill_width = 0;
	unsigned int general_counter = 1;
	int arr[5][2] = { {1,1} , {3,1}, {5,1}, {7,1}, {1,2} };
};

