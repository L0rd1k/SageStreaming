// UltimateMod.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include "Patterns/Patterns.h"
#include "Threads/Thread.h"
#include "Tasks/MainGeek.h"
#include "Tasks/AdventOfCode.h"

#include <chrono>

#ifdef __linux__
	#include "python3.5/Python.h"
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
	#include "Utils/Log.h"
#endif

#ifdef USE_GRAPHICS
	#include "Plotting/DrawGraph.h"
#endif



//static PyObject *method_fputs(PyObject *self, PyObject *args) {
//    char *str, *filename = NULL;
//    int bytes_copied = -1;
//    /* Parse arguments */
//    if(!PyArg_ParseTuple(args, "ss", &str, &filename)) {
//        return NULL;
//    }
//    FILE *fp = fopen(filename, "w");
//    bytes_copied = fputs(str, fp);
//    fclose(fp);
//    return PyLong_FromLong(bytes_copied);
//}

int main(int argc, char**argv) {
	// AdventOfCode _adventCode;
	// MainGeek _mainGeek;
	// Thread _objThread;
	// Patterns _pattern;

#ifdef USE_ML
	LinearRegression lg;
#endif
	 
#ifdef USE_GRAPHICS
	DrawGraph::createWindow(argc, argv);
#endif

#ifdef USE_LIB
	Log(COLOR::BLUE) << "Test string for logs\n";
	Log::printTime();
#endif

	 Py_Initialize();
	 auto start = std::chrono::high_resolution_clock::now();
	 FILE* PScriptFile = fopen("D:\GitProjects\FreeTimeCraft\_WorkScripts\simplePlot.py", "r");
	 if(PScriptFile) {
	 	PyRun_SimpleFile(PScriptFile, "simplePlot.py");	
	 }
	 auto stop = std::chrono::high_resolution_clock::now(); 
	 auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start); 
	// std::cout << duration.count() << std::endl; 
	return 0;
}
