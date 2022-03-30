#pragma once

#include "FileHandler.h"

#include <iostream>
#include <bitset>
#include <cstring>

class D6CustomCustoms : public FileHandler {
public:
	D6CustomCustoms();
	~D6CustomCustoms();
private:
	int parseDataFile(std::string filePath) override;
	void solutionPart1();
	void solutionPart2();
private:
	std::ifstream file;
	char string_data[27];
	std::string data;
	int sum = 0;
	std::bitset<26> current_group;
	std::bitset<26> result_group;
};


