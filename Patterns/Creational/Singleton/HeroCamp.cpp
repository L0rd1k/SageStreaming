#include "HeroCamp.h"

HeroCamp* HeroCamp::_singleHeroCamp = nullptr;

HeroCamp::HeroCamp(const std::string value) : _value(value) {
	
}

HeroCamp* HeroCamp::GetInstance(const std::string& value) {
	if (_singleHeroCamp == nullptr) {
		_singleHeroCamp = new HeroCamp(value);
	}
	return _singleHeroCamp;
}

void HeroCamp::BusinessLogic() {

}

std::string HeroCamp::getValue() const {
	return _value;
}