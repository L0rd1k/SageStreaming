#pragma once
#include "SwordsMan.h"

class BritishSwordsMan : public SwordsMan
{
public:
	std::string swordsmanProtectionLevel() const override;
	std::string swordsmanHitValue() const override;
};

