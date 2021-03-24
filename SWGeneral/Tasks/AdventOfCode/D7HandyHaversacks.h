#pragma once

#include "FileHandler.h"
#include <iostream>
#include <regex>
#include <cassert>
#include <iterator>

class D7HandyHaversacks : public FileHandler {
public:
	D7HandyHaversacks();
	~D7HandyHaversacks();
private:
	int parseDataFile(std::string filePath) override;
private:
	std::ifstream file;
};

