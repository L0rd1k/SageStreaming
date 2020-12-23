#pragma once
#include <iostream>

class SwordsMan
{
public:
	virtual ~SwordsMan() {};
	virtual std::string swordsmanProtectionLevel() const = 0;
	virtual std::string swordsmanHitValue() const = 0;
};

