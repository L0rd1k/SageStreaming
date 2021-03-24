#include "D1ReportRepair.h"

D1ReportRepair::D1ReportRepair() {
    if(parseDataFile("/home/ilya/Projects/GitProjects/UltimadeMode/SWGeneral/Data/stage_1.txt")) {
			bruteForceSolution();
			// pointerSolution();
			// hashSetSolution();
    } else {
		std::cout << "Can't parse data file!" << std::endl;
	}
}

D1ReportRepair::~D1ReportRepair() {
    file.close();
    complements.clear();
    vec_input.clear();
}

void D1ReportRepair::bruteForceSolution() {
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
}

void D1ReportRepair::hashSetSolution() {
    for (auto& value : vec_input) {
		int complement = 2020 - value;
		if (complements.count(value) != 0) {
			std::cout << value << " " << complement << std::endl;;
			std::cout << value * complement << std::endl;;
			break;
		} else {
			complements.insert(complement);
		}
	}
}

void D1ReportRepair::pointerSolution() {
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
}

int D1ReportRepair::parseDataFile(std::string filePath) {
    file.open(filePath);
    int tempValue;
    if(file.is_open()) {
		while (!file.eof()) {
        	file >> tempValue;
        	vec_input.push_back(tempValue);
		}
		return 1;
    } else {
        return 0;
    }
}