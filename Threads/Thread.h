#pragma once
#include <thread>
#include <iostream>
#include <vector>
#include <mutex>
#include <functional>
#include <condition_variable>
#include <future>

//#define FUNCTION_POINTER
//#define FUNCTION_OBJECTS
//#define FUNCTION_LAMBDAS
//#define FUTURE_PROMISE_EXAMPLE
//#define CONDITION_VARIABLES_EXAMPLE
//#define EVENT_HANDLING_EXAMPLE
//#define RACE_CONDITIONS_EXAMPLE
//#define ARGUMENT_EXAMPLE
//#define SHARING_DATA
//#define ASYNC_EXAMPLE
#define PACKAGE_TASK_EXAMPLE
class DisplayThread {
public:
	void operator()();
};

class DummyClass {
public:
	DummyClass();
	DummyClass(const DummyClass& obj);
	void sampleMemberFunction(int x);
};

// RACE CONDITIONS
class Wallet {
private:
	int nMoney = 0;
	std::mutex mutex;
public:
	Wallet();
	int getMoney();
	void addMoney(int money);
};

class Thread {
public:
	Thread();
	virtual ~Thread();
	void mainExecutor();
};

class TestApp {
private:
	std::mutex mutex;
	bool flagDataLoaded;
public:
	TestApp();
	void loadData();
	void mainTask();
};

class TestAppConditions {
private:
	std::mutex mutex;
	std::condition_variable conditionVar;
	bool flagDataLoaded;
public:
	TestAppConditions();
	void loadData();
	bool isDataLoaded();
	void mainTask();
};

struct StrDBDataFetcher {
	std::string operator() (std::string token) {
		std::string data = "Data From " + token;
		return data;
	}
};

