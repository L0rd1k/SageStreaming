#pragma once
#include "Archer.h"

class BritishArcher : public Archer
{
public:
	std::string archerShootingSpeed() const override;
	std::string archerArrowsNumber() const override;
};

