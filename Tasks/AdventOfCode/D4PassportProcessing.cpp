#include "D4PassportProcessing.h"

D4PassportProcessing::D4PassportProcessing() {
	if(parseDataFile("/home/ilya/Projects/GitProjects/UltimadeMode/SWGeneral/Data/stage_4.txt")) {
	} else {
		std::cout << "Can't parse data file!" << std::endl;
	}
}

D4PassportProcessing::~D4PassportProcessing() {
	file.close();
}

void D4PassportProcessing::solutionPart1() {
	std::vector<std::string> fields;
	std::cout << string_data << std::endl;
	while ((pos_end = string_data.find(":", pos_start)) != std::string::npos) {
		std::string field = string_data.substr(pos_end - 3, 3);
		pos_start = pos_end + 1;
		fields.push_back(field);
	}
	bool valid = false;
	if (fields.size() == 8) {
		valid = true;
	} else if (fields.size() >= 7) {
		if (!(std::find(fields.begin(), fields.end(), "cid") != fields.end()))
		valid = true;
	}

	if (valid) valid_counter++;	
	string_data.clear();
}

void D4PassportProcessing::solutionPart2() {
	while ((pos_end = string_data.find(" ", pos_start)) != std::string::npos) {
		std::string field = string_data.substr(pos_start, pos_end - pos_start);
		std::size_t delimeter_pos = field.find(":", 0);
		if (delimeter_pos != std::string::npos) {
			std::string key_value = field.substr(delimeter_pos - 3, 3);
			std::string data_value = field.substr(delimeter_pos + 1, field.length() - 1 - delimeter_pos);
			passportData.insert({ key_value , data_value });
		}
			pos_start = pos_end + 1;
		}

		std::string last_String = string_data.substr(pos_start, std::min(pos_end, string_data.size()) - pos_start + 1);
		std::size_t delimeter_pos = last_String.find(":", 0);
		if (delimeter_pos != std::string::npos) {
			std::string key_value = last_String.substr(delimeter_pos - 3, 3);
			std::string data_value = last_String.substr(delimeter_pos + 1, last_String.length() - 1 - delimeter_pos);
			passportData.insert({ key_value , data_value });
		}

		bool validity_size = false;
		if (passportData.size() == 8) {
			validity_size = true;
			} 
		else if (passportData.size() == 7) {
			for (auto& itr : passportData) {
				if (itr.first == "cid") {
					validity_size = false;
					break;
				}
				validity_size = true;
			}
		}
		string_data.clear();
		if (validity_size) {
			bool passportData_validity = true;
			for (auto value : passportData) {
				std::cout << value.first << ":" << value.second << std::endl;
				if (value.first == "byr") {
					if (std::stoi(value.second) < 1920 || std::stoi(value.second) > 2002) {
						passportData_validity = false;
						break;
					}
				}
				else if (value.first == "iyr") {
					if (std::stoi(value.second) < 2010 || std::stoi(value.second) > 2020) {
						passportData_validity = false;
						break;
					}
				}
				else if (value.first == "eyr") {
					if (std::stoi(value.second) < 2020 || std::stoi(value.second) > 2030) {
						passportData_validity = false;
						break;
					}
				}
				else if (value.first == "hgt") {
					std::string measure_value = value.second.substr(0, value.second.length() - 2);
					std::string measure = value.second.substr(value.second.length() - 2, value.second.length());
					if (measure == "cm") {
						if (std::stoi(measure_value) < 150 || (std::stoi(measure_value) > 193)) {
							passportData_validity = false;
							break;
						}
					}
				else if (measure == "in") {
					if (std::stoi(measure_value) < 59 || (std::stoi(measure_value) > 76)) {
						passportData_validity = false;
						break;
					}
				}
				else {
					passportData_validity = false;
				}
			}
			else if (value.first == "hcl") {
				std::regex regular_exp("[a-z0-9]+");
				std::cout << value.second.substr(1, 7) << std::endl;
				if ((value.second[0] != '#') || (value.second.length() != 7) || !(std::regex_match(value.second.substr(1, 7), regular_exp))) {
					passportData_validity = false;
					break;
				}
			}
			else if (value.first == "ecl") {
				std::vector<std::string> colors{ "amb","blu","brn","gry","grn","hzl","oth" };
				if (!(std::find(colors.begin(), colors.end(), value.second) != colors.end())) {
					passportData_validity = false;
					break;
				}
			}
			else if (value.first == "pid") {
				if (value.second.length() != 9) {
					passportData_validity = false;
					break;
				}
			}
		}
		std::cout << passportData_validity << std::endl;

		if (passportData_validity) {
			valid_counter++;
			std::cout << valid_counter << std::endl;
		}
		std::cout << "=======================" << std::endl;
	}
	passportData.clear();
}

int D4PassportProcessing::parseDataFile(std::string filePath) {
    file.open(filePath);
	if(file.is_open()) {
		while (!file.eof()) {
			getline(file, str); 
			string_data += " " + str;
			if(str.empty() || file.eof()) {
				pos_end = string_data.length();
				pos_start = 0;
				solutionPart1();
				// solutionPart2();
			}
		}
	    std::cout << "Valid counter : " << valid_counter << std::endl;
		return 1;
	} else {
		return 0;
	}
}