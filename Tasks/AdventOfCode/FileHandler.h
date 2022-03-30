#pragma once 

#include "FileHandler.h"
#include <string>
#include <fstream>

class FileHandler {
public:
    ~FileHandler() {};
    virtual int parseDataFile(std::string filePath) = 0;
};