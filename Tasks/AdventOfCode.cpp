#include "AdventOfCode.h"

AdventOfCode::AdventOfCode() {
	// Day1_ReportRepair("./Data/stage_1.txt");
	// Day2_Password_Philosophy("./Data/stage_2.txt");
	// Day3_Toboggan_Trajectory("./Data/stage_3.txt");
	Day4_Passport_Processing("./Data/stage_4.txt");
}

AdventOfCode::~AdventOfCode() {
}

void AdventOfCode::Day1_ReportRepair(std::string filePath) {
	std::vector<int> vec_input;
	std::ifstream file;
	file.open(filePath);
	if (file.is_open()) {
		int value;
		while (!file.eof()) {
			file >> value;
			vec_input.push_back(value);
		}
	}

#ifdef BRUTE_FORCE	// BruteForce Approach O(n^2)
	for (int i = 0; i < vec_input.size() - 2; i++) {
		for (int j = i + 1; j < vec_input.size() - 1 ; j++) {
			for (int k = j + 1; k < vec_input.size(); k++) {
				if (vec_input.at(i) + vec_input.at(j) + vec_input.at(k) == 2020) {
					std::cout << vec_input.at(i) << " + " << vec_input.at(j) << " + " << vec_input.at(k) << std::endl;
					std::cout << vec_input.at(i) * vec_input.at(j) * vec_input.at(k) << std::endl;
				}
			}
		}
	}
#endif

#ifdef TWO_POINTER	// Two Pointer Approach O(nlogn) 
	std::sort(vec_input.begin(), vec_input.end());
	int lastElem = vec_input.size() - 1;
	for (int i = 0; i < vec_input.size(); ) {
		if ((vec_input[i] + vec_input[lastElem]) > 2020) {
			lastElem--;
		}
		else if ((vec_input[i] + vec_input[lastElem]) < 2020) {
			i++;
		}
		else {
			std::cout << vec_input[i] << " + " << vec_input[lastElem] << std::endl;
			std::cout << vec_input[i] * vec_input[lastElem] << std::endl;
			break;
		}
	}
#endif

#define HASH_SET
#ifdef HASH_SET
	std::unordered_set<int> complements;
	for (auto& value : vec_input) {
		int complement = 2020 - value;
		if (complements.count(value) != 0) {
			std::cout << value << " " << complement << std::endl;;
			std::cout << value * complement << std::endl;;
			break;
		}
		else {
			complements.insert(complement);
		}
	}

#endif

}

void AdventOfCode::Day2_Password_Philosophy(std::string filePath) {
	std::ifstream file;
	file.open(filePath);
	if (file.is_open()) {
		std::string value_numbers, value_letter, value_password;
	    int valid_password = 0;
		while (!file.eof()) {
			file >> value_numbers >> value_letter >> value_password;
			std::string token_lowest = value_numbers.substr(0, value_numbers.find("-"));
			std::string token_highest = value_numbers.substr(value_numbers.find("-") + 1, value_letter.length());
			std::string token_letter = value_letter.substr(0, value_letter.find(":"));
			const char* char_value = token_letter.c_str();

#ifdef PART_1
			int char_itr = 0;
			for (auto &elem : value_password) {
				if (int(elem) == static_cast<int>(*char_value)) {
					char_itr++;
				}
			}
			if ((std::stoi(token_lowest) <= char_itr) && (char_itr <= std::stoi(token_highest))) {
				valid_password++;
			}
#endif 

#ifndef PART_2	
			// std::cout << value_numbers << " " << value_letter << " " << value_password << " - " << value_password[std::stoi(token_lowest) - 1] << " " << value_password[std::stoi(token_highest) - 1] << std::endl;
			bool b_1 = (int(value_password[std::stoi(token_lowest) - 1]) == static_cast<int>(*char_value));
			bool b_2 = (int(value_password[std::stoi(token_highest) - 1]) == static_cast<int>(*char_value));			
			if (b_1 && !b_2) {
				valid_password++;
			} else if (!b_1 && b_2) {
				valid_password++;
			}
#endif

		}
		// std::cout << "PART_1: Valid passwords number: " << valid_password << "\n";
		std::cout << "PART_2: Valid passwords number: " << valid_password << "\n";

	}


}

void AdventOfCode::Day3_Toboggan_Trajectory(std::string filePath) {
	std::ifstream file;
	file.open(filePath);
	if (file.is_open()) {
		std::string line;
		std::vector<std::vector<char>> input_vec;
		unsigned int hill_width = 0;
		unsigned int general_counter = 1;
		int arr[5][2] = { {1,1} , {3,1}, {5,1}, {7,1}, {1,2} };
		while (!file.eof()) {
			file >> line;
			std::vector<char> cols;
			hill_width = line.length();
			for (unsigned int i = 0; i < line.length(); i++) {
				cols.push_back(line[i]);
			}
			input_vec.push_back(cols);
		}
		for (unsigned int itr = 0; itr < sizeof(arr) / sizeof(arr[0]); itr++) {
			int tree_counter = 0;
			unsigned int current_step_x = arr[itr][0], current_step_y = arr[itr][1];
			while (current_step_y < input_vec.size()) {
				if (input_vec[current_step_y][current_step_x] == '#') {
					tree_counter++;
				}
				current_step_x += arr[itr][0];
				current_step_x %= hill_width;
				current_step_y += arr[itr][1];
			}
			general_counter *= tree_counter;
			std::cout << "Trees_" + std::to_string(itr+1) + " :"  << tree_counter << std::endl;
		}
		std::cout << "Trees in general: " << general_counter << std::endl;
	}
}

void AdventOfCode::Day4_Passport_Processing(std::string filePath) {
	std::ifstream file;
	file.open(filePath);
	if (file.is_open()) {
		std::string string_data, str;
		int valid_counter = 0;
		while (!file.eof()) {
			getline(file, str);
			string_data += str;
			if (str.empty() || file.eof()) {
				std::size_t pos_end = string_data.length();
				std::size_t pos_start = 0;
				std::vector<std::string> fields;
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
			
		}
		std::cout << "Valid counter: " << valid_counter << std::endl;
	}
}