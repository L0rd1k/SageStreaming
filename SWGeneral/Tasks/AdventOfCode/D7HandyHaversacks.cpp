#include "D7HandyHaversacks.h"

D7HandyHaversacks::D7HandyHaversacks() {

}

D7HandyHaversacks::~D7HandyHaversacks() {

}

int D7HandyHaversacks::parseDataFile(std::string filePath) {
	file.open(filePath);
	if(file.is_open()) {
		std::string currentString;

		std::regex input_value_pattern(R"((\w+ \w+) bags contain)");
		std::regex inherited_value_pattern(R"(,? (\d+) (\w+ \w+) bags?)");
		
		std::smatch root_match;
		std::smatch inherited_root_match;
		while(getline(file, currentString)) {
			assert(std::regex_search(currentString, root_match, input_value_pattern));
			auto words_begin = std::sregex_iterator(currentString.begin(), currentString.end(), inherited_value_pattern);
			auto words_end = std::sregex_iterator();
			std::cout << "Found " << std::distance(words_begin, words_end) << " words:\n";
			std::cout << root_match[0] << " - " << root_match[1] << std::endl;
			for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
				std::smatch match = *i;
				std::string match_str = match.str();
				std::cout << match_str << '\n';
			}
			std::cout << "---------------------------" << std::endl;
		}


	
	}


	return 0;
}