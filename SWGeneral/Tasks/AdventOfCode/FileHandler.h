#pragma once 

#include "FileHandler.h"
#include <string>

class FileHandler {
public:
    ~FileHandler() {};
    virtual int parseDataFile(std::string filePath) = 0;
};