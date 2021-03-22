#pragma once

#include "FileHandler.h"

class D1ReportRepair : public FileHandler {
public:
    D1ReportRepair();
    ~D1ReportRepair();
private:
    void bruteForceSolution();
    void hashSetSolution();
    void pointerSolution(); 
    void parseDataFile() override;
};