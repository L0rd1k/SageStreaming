#pragma once

#include "FileHandler.h"

class D2PasswordPhilosophy : public FileHandler {
public:
	D2PasswordPhilosophy();
	~D2PasswordPhilosophy();
private:
	int parseDataFile(std::string filePath) override;
};

