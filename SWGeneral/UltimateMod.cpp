#include "Patterns/Patterns.h"
#include "Threads/Thread.h"
#include "Tasks/GeekForGeeks/MainGeek.h"
#include "Tasks/AdventOfCode/AdventOfCode.h"
#include "Tasks/LeetCode/LeetCode.h"
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

#ifdef USE_VIDEO
	#include "VideoProcessing.h"
#endif

std::vector<int> twoSum(std::vector<int>& nums, int target) {
    std::vector<int> test_vec;
    for(int i = 0; i < nums.size() - 1; i++) {
		std::cout << nums.at(i) << std::endl;
        for(int j = i+1; j < nums.size(); j++) {
			std::cout << nums.at(j) << " ";
            if(nums.at(i) + nums.at(j) == target) {
                test_vec.push_back(i);
                test_vec.push_back(j);
				std::cout << "Result:" << i << " " << j << std::endl;
                return test_vec;
            }
        }
		std::cout << "\n";
    }
    return test_vec;
}

int main(int argc, char**argv) {
	// AdventOfCode _adventCode;
	// MainGeek _mainGeek;
	// Thread _objThread;
	// Patterns _pattern;
	// LeetCode _lcode;

#ifdef USE_VIDEO
	VideoProcessing vidProc;
#endif

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
#ifdef __linux__
	 FILE* PScriptFile = fopen(scriptPath, "r");
#elif _WIN32 
	 PyObject* object = Py_BuildValue("s", scriptPath);
	 FILE* PScriptFile = _Py_fopen_obj(object, "r+");
#endif
	 if(PScriptFile) {
	 	// PyRun_SimpleFile(PScriptFile, "simplePlot.py");	
	 }
	 auto stop = std::chrono::high_resolution_clock::now(); 
	 auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start); 
	 // std::cout << duration.count() << std::endl; 
	 return 0;
}
