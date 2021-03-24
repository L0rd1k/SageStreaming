// UltimateMod.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include "Patterns/Patterns.h"
#include "Threads/Thread.h"
#include "Tasks/GeekForGeeks/MainGeek.h"
#include "Tasks/AdventOfCode/AdventOfCode.h"

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

std::vector<int> twoSum(std::vector<int>& nums, int target) {
    std::vector<int> test_vec;
    for(int i = 0; i < nums.size() - 2; i++) {
		std::cout << i << std::endl;
        for(int j = i+1; j < nums.size() - 1; j++) {
            if(nums.at(i) + nums.at(j) == target) {
                test_vec.push_back(i);
                test_vec.push_back(j);
				std::cout << i << " " << j << std::endl;
                return test_vec;
            }
        }
    }
    return test_vec;
}

int main(int argc, char**argv) {
	// AdventOfCode _adventCode;
	// MainGeek _mainGeek;
	// Thread _objThread;
	// Patterns _pattern;
	std::vector<int> vec{2,7,11,15};
	twoSum(vec, 6);


#ifdef USE_ML
	// LinearRegression lg;
#endif
	 
#ifdef USE_GRAPHICS
	// DrawGraph::createWindow(argc, argv);
#endif

#ifdef USE_LIB
	// Log(COLOR::BLUE) << "Test string for logs\n";
	// Log::printTime();
#endif
	 Py_Initialize();
	 auto start = std::chrono::high_resolution_clock::now();
	 const char* scriptPath = "D:\\GitProjects\\FreeTimeCraft\\_WorkScripts\\simplePlot.py";
	 FILE* PScriptFile = fopen(scriptPath, "r");
	 if(PScriptFile) {
	 	PyRun_SimpleFile(PScriptFile, "simplePlot.py");	
	 }
	 auto stop = std::chrono::high_resolution_clock::now(); 
	 auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start); 
	// std::cout << duration.count() << std::endl; 
	return 0;
}
