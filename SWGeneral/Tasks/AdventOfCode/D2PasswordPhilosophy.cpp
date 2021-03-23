#include "D2PasswordPhilosophy.h"

D2PasswordPhilosophy::D2PasswordPhilosophy() {

}

D2PasswordPhilosophy::~D2PasswordPhilosophy() {
	file.close();
}

void D2PasswordPhilosophy::getTokens() {
	token_lowest = value_numbers.substr(0, value_numbers.find("-"));
	token_highest = value_numbers.substr(value_numbers.find("-") + 1, value_letter.length());
	token_letter = value_letter.substr(0, value_letter.find(":"));
	char_value = token_letter.c_str();
}

void D2PasswordPhilosophy::solutionPart1() {
	int char_itr = 0;
	for (auto &elem : value_password) {
		if (int(elem) == static_cast<int>(*char_value)) {
			char_itr++;
		}
	}
	if ((std::stoi(token_lowest) <= char_itr) && (char_itr <= std::stoi(token_highest))) {
		valid_password++;
	}
}

int D2PasswordPhilosophy::solutionPart2() {
	bool b_1 = (int(value_password[std::stoi(token_lowest) - 1]) == static_cast<int>(*char_value));
	bool b_2 = (int(value_password[std::stoi(token_highest) - 1]) == static_cast<int>(*char_value));			
	if (b_1 && !b_2) {
		valid_password++;
	} else if (!b_1 && b_2) {
		valid_password++;
	}
}

int D2PasswordPhilosophy::parseDataFile(std::string filePath) {
	file.open(filePath);
	if(file.is_open()) {
		while(!file.eof()) {
			file >> value_numbers >> value_letter >> value_password;
			getTokens();
			solutionPart1();
		}
	std::cout << "Valid passwords number: " << valid_password << "\n";
	} else {
		return -1;
	}
}