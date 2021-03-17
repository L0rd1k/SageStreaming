#pragma once
#include "Army.h"
#include "BritishSwordsMan.h"
#include "BritishArcher.h"
#include "BritishCannon.h"

class BritishArmy : public Army {
public:
	SwordsMan *CreateSwordsMan() const override;
	Archer *CreateArcher() const override;
	Cannon *CreateCannon() const override;
};

