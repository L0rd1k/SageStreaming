#pragma once
#include "Archer.h"

class FrenchArcher : public Archer
{
public:
	std::string archerShootingSpeed() const override;
	std::string archerArrowsNumber() const override;
};

