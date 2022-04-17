#pragma once
#include "SwordsMan.h"
#include "Archer.h"
#include "Cannon.h"

class Army {
public:
	virtual SwordsMan *CreateSwordsMan() const = 0;
	virtual Archer *CreateArcher() const = 0;
	virtual Cannon *CreateCannon() const = 0;
};

