#include "D2PasswordPhilosophy.h"

D2PasswordPhilosophy::D2PasswordPhilosophy() {
	if(parseDataFile("/home/ilya/Projects/GitProjects/UltimadeMode/SWGeneral/Data/stage_2.txt")) {
    } else {
		std::cout << "Can't parse data file!" << std::endl;
	}
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

void D2PasswordPhilosophy::solutionPart2() {
	int t_l = std::stoi(token_lowest);
	int t_h = std::stoi(token_highest);
	bool b_1 = (uint64_t(value_password[t_l - 1]) == static_cast<uint64_t>(*char_value));
	bool b_2 = (uint64_t(value_password[t_h - 1]) == static_cast<uint64_t>(*char_value));
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
		return 1;
	} else {
		return 0;
	}
}
