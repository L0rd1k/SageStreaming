#pragma once
#include <iostream>

class Cannon
{
public:
	virtual ~Cannon() {};
	virtual std::string cannonGunPower() const = 0;
	virtual std::string cannonReloadSpeed() const = 0;
};

