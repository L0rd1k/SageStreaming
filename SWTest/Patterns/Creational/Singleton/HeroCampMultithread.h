#pragma once
#include <iostream>
#include <string>
#include <mutex>

class HeroCampMultithread {
public:
	HeroCampMultithread(HeroCampMultithread& other) = delete;
	void operator=(const HeroCampMultithread&) = delete;
	static HeroCampMultithread* GetInstance(const std::string& value);
	std::string getValue() const;
protected:
	HeroCampMultithread(const std::string value);
	~HeroCampMultithread();
	std::string _value;
private:
	static std::mutex _mutex;
	static HeroCampMultithread* _singleHeroCamp;
	void initializeCampEnvironment();
};

