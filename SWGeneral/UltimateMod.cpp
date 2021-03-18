// UltimateMod.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include "Patterns/Patterns.h"
#include "Threads/Thread.h"
#include "Tasks/MainGeek.h"
#include "Tasks/AdventOfCode.h"

#include <chrono>
#include "python3.5/Python.h"


#ifdef USE_ML
	#include "ML/LinearRegression.h"
#endif
#ifdef USE_LIB
	#include "Utils/Log.h"
#endif
#ifdef USE_GRAPHICS
	#include "Plotting/DrawGraph.h"
#endif

static PyObject *method_fputs(PyObject *self, PyObject *args) {
    char *str, *filename = NULL;
    int bytes_copied = -1;
    /* Parse arguments */
    if(!PyArg_ParseTuple(args, "ss", &str, &filename)) {
        return NULL;
    }
    FILE *fp = fopen(filename, "w");
    bytes_copied = fputs(str, fp);
    fclose(fp);
    return PyLong_FromLong(bytes_copied);
}

int main(int argc, char**argv) {
	// AdventOfCode _adventCode;
	// MainGeek _mainGeek;
	// Thread _objThread;
	// Patterns _pattern;

#ifdef USE_ML
	// LinearRegression lg;
#endif
	 
#ifdef USE_GRAPHICS
	DrawGraph::createWindow(argc, argv);
#endif

	// Py_Initialize();
	// auto start = std::chrono::high_resolution_clock::now();
	// FILE* PScriptFile = fopen("/home/ilya/Projects/GitProjects/FreeTimeCraft2/_WorkScripts/simplePlot.py", "r");
	// if(PScriptFile) {
	// 	PyRun_SimpleFile(PScriptFile, "simplePlot.py");	
	// }
	// auto stop = std::chrono::high_resolution_clock::now(); 
	// auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start); 
	// std::cout << duration.count() << std::endl; 
	// return 0;
}
