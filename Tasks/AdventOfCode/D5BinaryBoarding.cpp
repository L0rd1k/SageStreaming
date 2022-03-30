#include "D5BinaryBoarding.h"

D5BinaryBoarding::D5BinaryBoarding() {
	if(parseDataFile("/home/ilya/Projects/GitProjects/UltimadeMode/SWGeneral/Data/stage_5.txt")) {
		std::cout << "Max value: " << max_value << std::endl;
		std::sort(id_values.begin(), id_values.end());
		for (std::vector<int>::size_type i = 1; i != id_values.size() - 1; i++) {
			if (id_values[i - 1] + 2 == id_values[i]) {
				std::cout << "Result task: " << id_values[i] - 1 << std::endl;
			}
		}
	} else {
		std::cout << "Can't parse data file!" << std::endl;
	}
}

D5BinaryBoarding::~D5BinaryBoarding() {
	file.close();
}

void D5BinaryBoarding::executeSolver( ) {
	int min_Value = 0, max_Value = 128;
	int mid_Value = 128 / 2;
	int mid_range = 8 / 2, min_range = 0, max_range = 8;
	for (auto &elem : seat_partitioning) {
		if (elem == 'F') {
			min_Value = min_Value;
			max_Value = mid_Value;
			// std::cout << min_Value << " " << max_Value << " - " << mid_Value << std::endl;
		} else if (elem == 'B') {
			min_Value = mid_Value;
			max_Value = max_Value;
			// std::cout << min_Value << " " << max_Value << " - " << mid_Value << std::endl;
		} else if (elem == 'L') {
			min_range = min_range;
			max_range = mid_range;
			// std::cout << min_range << " " << max_range << " - " << mid_range << std::endl;
		} else if (elem == 'R') {
			min_range = mid_range;
			max_range = max_range;
			// std::cout << min_range << " " << max_range << " - " << mid_range << std::endl;
		}
		mid_Value = (min_Value + max_Value) / 2;
		mid_range = (min_range + max_range) / 2;
	}

	id_values.push_back(mid_Value * 8 + mid_range);
	if ((mid_Value * 8 + mid_range) > max_value) {
		max_value = (mid_Value * 8 + mid_range);
	}
}

int D5BinaryBoarding::parseDataFile(std::string filePath)
{
	file.open(filePath);
	if (file.is_open()) {
		while (!file.eof()) {
			file >> seat_partitioning;
			executeSolver();
		}
		return 1;
	} else {
		return 0;	
	}
}