#pragma once

#include "FileHandler.h"

class D3TobogganTrajectory : public FileHandler {
public:
	D3TobogganTrajectory();
	~D3TobogganTrajectory();
private:
	int parseDataFile(std::string filePath) override;
};

