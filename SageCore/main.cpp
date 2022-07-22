#include "core/core.h"

int main(int argc, char**argv) {
	sage::Core::inst().createSubstances(4);
	sage::Core::inst().createWindow(argc, argv);
	sage::Core::inst().connectCamBufToWindow();
	sage::Core::inst().enableWindow();

	return 0;
}
