#include "D3TobogganTrajectory.h"

D3TobogganTrajectory::D3TobogganTrajectory() {
	if(parseDataFile("/home/ilya/Projects/GitProjects/UltimadeMode/SWGeneral/Data/stage_3.txt")) {
		executeSolution();
    } else {
		std::cout << "Can't parse data file!" << std::endl;
	}
}

D3TobogganTrajectory::~D3TobogganTrajectory() {
	file.close();
}

void D3TobogganTrajectory::executeSolution() {
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

int D3TobogganTrajectory::parseDataFile(std::string filePath) {
    file.open(filePath);
	if(file.is_open()) {
		while(!file.eof()) {
			file >> line;
			std::vector<char> cols;
			hill_width = line.length();
			for (unsigned int i = 0; i < line.length(); i++) {
				cols.push_back(line[i]);
			}
			input_vec.push_back(cols);
		}
		return 1;
	} else {
		return 0;	
	}
}