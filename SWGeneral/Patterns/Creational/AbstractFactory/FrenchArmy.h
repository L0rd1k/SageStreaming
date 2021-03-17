#pragma once
#include "Army.h"
#include "FrenchSwordsMan.h"
#include "FrenchArcher.h"
#include "FrenchCannon.h"

class FrenchArmy : public Army
{
public:
	SwordsMan* CreateSwordsMan() const override;
	Archer* CreateArcher() const override;
	Cannon* CreateCannon() const override;
};


