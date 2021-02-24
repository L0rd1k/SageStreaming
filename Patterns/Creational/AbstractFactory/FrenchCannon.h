#pragma once
#include "Cannon.h"

class FrenchCannon : public Cannon {
public:
	virtual std::string cannonGunPower() const override;
	virtual std::string cannonReloadSpeed() const override;
};

