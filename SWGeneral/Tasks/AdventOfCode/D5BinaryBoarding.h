#pragma once

#include "FileHandler.h"

class D5BinaryBoarding : public FileHandler {
public:
	D5BinaryBoarding();
	~D5BinaryBoarding();
private:
	int parseDataFile(std::string filePath) override;
};

