#include "D6CustomCustoms.h"

#define TASK_2

D6CustomCustoms::D6CustomCustoms() {
	memset(string_data, 0, 27 * sizeof(char));
	result_group.set(); // set all bitset to 1
	std::cout << parseDataFile("/home/ilya/Projects/GitProjects/UltimadeMode/SWGeneral/Data/stage_6.txt") << std::endl;
}

D6CustomCustoms::~D6CustomCustoms() {
	file.close();
}

void D6CustomCustoms::solutionPart1() {
	if (data != "") {
		for (int i = 0; i < data.size(); i++) {
			if (string_data[data[i] - 'a'] == 0) {
				++sum;
				string_data[data[i] - 'a'] = 1;
			}
		}
		for (int i = 0; i < data.size(); i++) {
			std::cout << data[i] << std::endl;
		}
	} else { 
		std::cout << "=================" << std::endl;
		memset(string_data, 0, 27 * sizeof(char));
	}
}

void D6CustomCustoms::solutionPart2() {
	if (data != "") {
		std::cout << data << std::endl;
		for (int i = 0; i < data.size(); ++i) {
			current_group[data[i]-'a'] = 1;
		}
		result_group &= current_group;
		current_group.reset();
	} else {
		std::cout << result_group << std::endl;
		std::cout << "======================" << std::endl;
		sum += result_group.count();
		result_group.set();
	} 
}

int D6CustomCustoms::parseDataFile(std::string filePath) {
	file.open(filePath);
	if (file.is_open()) {
		while (getline(file, data)) {
			// solutionPart1();
			solutionPart2();
		}

#ifdef TASK_2
		sum += result_group.count();
		result_group.set();
#endif

		return sum;
	} else {
		return 0;
	}
}