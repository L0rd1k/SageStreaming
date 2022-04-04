#include "Patterns/Patterns.h"
#include "Threads/Thread.h"
#include <chrono>

#ifdef __linux__
	#include <python3.7m/Python.h>
#elif _WIN32
	#ifdef _DEBUG
		#undef _DEBUG
		#include <Python.h>
	#define _DEBUG
	#else
		#include <Python.h>
	#endif
#endif

#ifdef USE_ML
	#include "ML/LinearRegression.h"
#endif

#ifdef USE_LIB
	#include "utils/Log.h"
#endif

#ifdef USE_GRAPHICS
	#include "Plotting/DrawGraph.h"
#endif


int main(int argc, char**argv) {

	 return 0;
}
