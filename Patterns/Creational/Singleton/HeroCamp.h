#pragma once
#include <iostream>
#include <string>

class HeroCamp {
public:
	HeroCamp(HeroCamp& other) = delete;
	void operator=(const HeroCamp&) = delete;
	static HeroCamp *GetInstance(const std::string& value);
	void BusinessLogic();
	std::string getValue() const;
protected:
	HeroCamp(const std::string value);
	std::string _value;
	static HeroCamp* _singleHeroCamp;
};

