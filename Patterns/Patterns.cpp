#include "Patterns.h"

Patterns::Patterns() {
	mainPatternExecutor();
}

Patterns::~Patterns() {
}

#ifdef PATTERN_ABSTRACT_FACTORY
#include "Creational/AbstractFactory/Army.h"
#include "Creational/AbstractFactory/BritishArmy.h"
#include "Creational/AbstractFactory/FrenchArmy.h"
void AbstractFactoryClient(const Army& army) {
	const SwordsMan* _swordsman = army.CreateSwordsMan();
	const Archer* _archer = army.CreateArcher();
	const Cannon* _cannon = army.CreateCannon();
	std::cout << _swordsman->swordsmanHitValue() << _swordsman->swordsmanProtectionLevel();
	std::cout << _archer->archerArrowsNumber() << _archer->archerShootingSpeed();
	std::cout << _cannon->cannonGunPower() << _cannon->cannonReloadSpeed();
	delete _swordsman;
	delete _archer;
	delete _cannon;
}
#endif

#ifdef PATTERN_SINGLETON
#include "Creational/Singleton/HeroCamp.h"
void ThreadFirst() {
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	HeroCamp* _heroCamp = HeroCamp::GetInstance("FIRST HERO THREAD");
	std::cout << _heroCamp->getValue() << std::endl;
}

void ThreadSecond() {
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	HeroCamp* _heroCamp = HeroCamp::GetInstance("SECOND HERO THREAD");
	std::cout << _heroCamp->getValue() << std::endl;
}
#endif

#ifdef PATTERN_SINGLETON_MULTITHREDING
#include "Creational/Singleton/HeroCampMultithread.h"
void ThreadFirstML() {
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	HeroCampMultithread* _heroCamp = HeroCampMultithread::GetInstance("FIRST HERO THREAD");
	std::cout << _heroCamp->getValue() << std::endl;
}

void ThreadSecondML() {
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	HeroCampMultithread* _heroCamp = HeroCampMultithread::GetInstance("SECOND HERO THREAD");
	std::cout << _heroCamp->getValue() << std::endl;
}
#endif


void Patterns::mainPatternExecutor() {
#ifdef PATTERN_ABSTRACT_FACTORY
	std::cout << "1) BRITISH ARMY factory!" << std::endl;
	BritishArmy* _britishArmy = new BritishArmy();
	AbstractFactoryClient(*_britishArmy);
	delete _britishArmy;
	std::cout << "2) FRENCH ARMY factory!" << std::endl;
	FrenchArmy* _frenchArmy = new FrenchArmy();
	AbstractFactoryClient(*_frenchArmy);
	delete _frenchArmy;
#endif

#ifdef PATTERN_SINGLETON
	ThreadFirst();
	ThreadSecond();
	std::thread t1(ThreadFirst);
	std::thread t2(ThreadSecond);
	t1.join();
	t2.join();
#endif

#ifdef PATTERN_SINGLETON_MULTITHREDING
	ThreadFirstML();
	ThreadSecondML();
	// std::thread t1(ThreadFirstML);
	// std::thread t2(ThreadSecondML);
	// t1.join();
	// t2.join();
#endif
}