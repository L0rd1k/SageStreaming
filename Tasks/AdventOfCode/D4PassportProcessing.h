#pragma once

#include "FileHandler.h"
#include <map>
#include <vector>
#include <algorithm>
#include <iostream>
#include <regex>

class D4PassportProcessing : public FileHandler {
public:
	D4PassportProcessing();
	~D4PassportProcessing();
private:
	int parseDataFile(std::string filePath) override;
	void solutionPart1();
	void solutionPart2();
private:
	std::ifstream file;
	std::string string_data, str;
	int valid_counter = 0;
	std::map<std::string, std::string> passportData;
	std::size_t pos_start, pos_end;
};

