﻿#include "core/core.h"

int main(int argc, char**argv) {
	sage::Core::inst().createSubstances(1);
	sage::Core::inst().createWindow(argc, argv);
	sage::Core::inst().enableWindow();

	return 0;
}