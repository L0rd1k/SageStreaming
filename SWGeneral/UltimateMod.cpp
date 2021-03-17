// UltimateMod.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include "Patterns/Patterns.h"
#include "Threads/Thread.h"
#include "Tasks/MainGeek.h"
#include "Tasks/AdventOfCode.h"
#include "ML/LinearRegression.h"
#include "Utils/Log.h"
#include "python3.5/Python.h"
#include <chrono>

int main() {
	// AdventOfCode _adventCode;
	// MainGeek _mainGeek;
	// Thread _objThread;
	// Patterns _pattern;
	//LinearRegression lg;
	Py_Initialize();

	// Run function from file
	// PyObject *pName, *pModule, *pFunc, *pArgs, *pValue;
	// pName = PyUnicode_FromString((char*)"script");
	// pFunc = PyObject_GetAttrString(pModule, (char*)"test");
	// pArgs = PyTuple_Pack(1, PyUnicode_FromString((char*)"Greg"));
	// pValue = PyObject_CallObject(pFunc, pArgs);
	// auto result = _PyUnicode_AsString(pValue);
	// std::cout << result << std::endl;
	
	// Run script file
	auto start = std::chrono::high_resolution_clock::now();
	FILE* PScriptFile = fopen("/home/ilya/Projects/GitProjects/FreeTimeCraft2/Testing/Strings.py", "r");
	if(PScriptFile) {
		PyRun_SimpleFile(PScriptFile, "Strings.py");	
	}
	auto stop = std::chrono::high_resolution_clock::now(); 
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start); 
	std::cout << duration.count() << std::endl; 
	return 0;
}
