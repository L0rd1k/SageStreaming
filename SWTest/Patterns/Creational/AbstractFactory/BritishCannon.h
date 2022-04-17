#pragma once
#include "Cannon.h"

class BritishCannon : public Cannon
{
public:
	virtual std::string cannonGunPower() const override;
	virtual std::string cannonReloadSpeed() const override;
};

