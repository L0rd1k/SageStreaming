#pragma once

#define PATTERN_ABSTRACT_FACTORY
#define PATTERN_SINGLETON
#define PATTERN_SINGLETON_MULTITHREDING
/*-------------------------------------*/

#include <iostream>
#include <chrono>
#include <thread>


class Patterns {
public:
	Patterns();
	virtual ~Patterns();
	void mainPatternExecutor();
};

