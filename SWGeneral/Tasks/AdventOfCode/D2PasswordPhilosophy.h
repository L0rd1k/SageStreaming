#pragma once

#include "FileHandler.h"

#include <iostream>

class D2PasswordPhilosophy : public FileHandler {
public:
	D2PasswordPhilosophy();
	~D2PasswordPhilosophy();
private:
	int parseDataFile(std::string filePath) override;
	void solutionPart1();
	int solutionPart2();
	void getTokens();
    std::string token_lowest, token_highest, token_letter;
    const char* char_value;
	std::ifstream file;
	std::string value_numbers, value_letter, value_password;
	int valid_password = 0;
};

