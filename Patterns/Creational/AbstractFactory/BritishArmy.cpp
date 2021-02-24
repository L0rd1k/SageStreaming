#include "BritishArmy.h"

SwordsMan *BritishArmy::CreateSwordsMan() const {
	return new BritishSwordsMan();
}

Archer* BritishArmy::CreateArcher() const {
	return new BritishArcher();
}

Cannon* BritishArmy::CreateCannon() const {
	return new BritishCannon();
}
