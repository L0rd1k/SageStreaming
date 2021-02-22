#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <unordered_set>
#include <string>
#include <array>
#include <map>
class AdventOfCode
{
public:
	AdventOfCode();
	virtual ~AdventOfCode();
private:
	void Day1_ReportRepair(std::string filePath);
	void Day2_Password_Philosophy(std::string filePath);
	void Day3_Toboggan_Trajectory(std::string filePath);
	void Day4_Passport_Processing(std::string filePath);
};

