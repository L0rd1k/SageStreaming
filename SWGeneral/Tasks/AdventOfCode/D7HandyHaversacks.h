#pragma once

#include "FileHandler.h"

class D7HandyHaversacks : public FileHandler {
public:
	D7HandyHaversacks();
	~D7HandyHaversacks();
private:
	int parseDataFile(std::string filePath) override;
};

