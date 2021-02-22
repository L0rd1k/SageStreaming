#include "Features.h"

Features::Features() {
	mainExec();
}

Features::~Features() {
}

int Features::addition(int first, int second) {
	return first + second;
}

void Features::mainExec() {
	//auto add_func = std::bind(&addition, std::placeholders::_1, std::placeholders::_2);
	//std::cout << add_func(2, 2) << std::endl;
}