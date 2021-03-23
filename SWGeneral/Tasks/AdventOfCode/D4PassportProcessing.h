#pragma once

#include "FileHandler.h"

class D4PassportProcessing : public FileHandler {
public:
	D4PassportProcessing();
	~D4PassportProcessing();
private:
	int parseDataFile(std::string filePath) override;
};

