#pragma once
#include "SwordsMan.h"

class FrenchSwordsMan : public SwordsMan
{
public:
	std::string swordsmanProtectionLevel() const override;
	std::string swordsmanHitValue() const override;
};

