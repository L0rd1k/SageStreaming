#pragma once
#include "string"
#include <iostream>
class Archer
{
public:
	virtual ~Archer() {};
	virtual std::string archerShootingSpeed() const = 0;
	virtual std::string archerArrowsNumber() const = 0;
};

