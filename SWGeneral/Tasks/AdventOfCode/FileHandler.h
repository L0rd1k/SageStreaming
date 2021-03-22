#pragma once 

#include "FileHandler.h"

class FileHandler {
public:
    ~FileHandler() {};
    virtual void parseDataFile() = 0;
};