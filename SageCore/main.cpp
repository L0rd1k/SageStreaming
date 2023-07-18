#include "core/core.h"

#include "config/ini_parser.h"
#include "utils/log.h"
#include "net/address.h"
int main(int argc, char**argv) {

    sage::IniParser::inst().setIniFile("../data/cameraConf.ini");

	//> Create cameras substances.
	sage::Core::inst().createSubstances();
	//> Create opengl window
	sage::Core::inst().createWindow(argc, argv);
	//> Assign image buffers to textures.
	sage::Core::inst().connectCamBufToWindow();
	sage::Core::inst().enableCallbacks();
	sage::Core::inst().runSubstances();
	sage::Core::inst().enableWindow();


	return 0;
}
