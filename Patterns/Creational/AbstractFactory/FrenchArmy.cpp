#include "FrenchArmy.h"

SwordsMan* FrenchArmy::CreateSwordsMan() const {
	return new FrenchSwordsMan();
}

Archer* FrenchArmy::CreateArcher() const {
	return new FrenchArcher();
}

Cannon* FrenchArmy::CreateCannon() const {
	return new FrenchCannon();
}

