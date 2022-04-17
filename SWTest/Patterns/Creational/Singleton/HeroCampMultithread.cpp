#include "HeroCampMultithread.h"

HeroCampMultithread* HeroCampMultithread::_singleHeroCamp = nullptr;
std::mutex HeroCampMultithread::_mutex;

HeroCampMultithread::HeroCampMultithread(const std::string value) : _value(value) {
	initializeCampEnvironment();
}

HeroCampMultithread::~HeroCampMultithread() {
}

HeroCampMultithread* HeroCampMultithread::GetInstance(const std::string& value) {
	std::lock_guard<std::mutex> lock(_mutex);
	std::cout << "(Hint) Run lock mutex" << std::endl;
	if (_singleHeroCamp == nullptr) {
		std::cout << "(Hint) Create object" << std::endl;
		_singleHeroCamp = new HeroCampMultithread(value);
	}
	else {
		std::cout << "(Hint) Object already object" << std::endl;
	}
	return _singleHeroCamp;
}

void HeroCampMultithread::initializeCampEnvironment() {
	std::cout << ".....Create hero's hall" << std::endl;
	std::cout << ".....Set armoors room " << std::endl;
	std::cout << ".....Create armoors room " << std::endl;
}

std::string HeroCampMultithread::getValue() const {
	return _value;
}