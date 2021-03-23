#pragma once

#include "FileHandler.h"

class D6CustomCustoms : public FileHandler {
public:
	D6CustomCustoms();
	~D6CustomCustoms();
private:
	int parseDataFile(std::string filePath) override;
};

