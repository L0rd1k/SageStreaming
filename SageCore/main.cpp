#include "core/core.h"

#include "config/ini_parser.h"

int main(int argc, char**argv) {
    sage::IniParser::inst().setIniFile("../data/cameraConf.ini");

	sage::Core::inst().createSubstances(3);
	sage::Core::inst().createWindow(argc, argv);
	sage::Core::inst().connectCamBufToWindow();
	sage::Core::inst().enableCallbacks();
	sage::Core::inst().enableWindow();

	return 0;
}
