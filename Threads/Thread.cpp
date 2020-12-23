#include "Thread.h"

Wallet::Wallet() : nMoney(0) {
}

int Wallet::getMoney() {
	return nMoney;
}

void Wallet::addMoney(int money) {
	std::lock_guard<std::mutex> lockGuard(mutex);
	for (int i = 0; i < money; ++i)
		nMoney++;
}

//=================================================================

void DisplayThread::operator()() {
	for (int i = 0; i < 10000; i++) {
		std::cout << "Display Thread" << std::endl;
	}
}
//=================================================================

DummyClass::DummyClass() {
}

DummyClass::DummyClass(const DummyClass& obj) {
}

void DummyClass::sampleMemberFunction(int x) {
	std::cout << "Inside sampleMemberFunction " << x << std::endl;
}

//=================================================================

void threadFunction() {
	for (int i = 0; i < 10000; i++) {
		std::cout << "Function Thread" << std::endl;
	}
}

void threadFunctionCallback(int const &c_value) {
	int& value = const_cast<int&>(c_value);
	value+=50;
	std::cout << "Inside Thread value = " << value << std::endl;
}

int testMultithreadWallet() {
	Wallet walletObject;
	std::vector<std::thread> threads;
	for (int i = 0; i < 5; ++i) {
		threads.push_back(std::thread(&Wallet::addMoney, &walletObject, 1000));
	}  

	for (unsigned int i = 0; i < threads.size(); i++) {
		threads.at(i).join();
	}

	return walletObject.getMoney();

}

void initializer(std::promise<int>* promiseObj) {
	std::cout << "Thread: " << std::this_thread::get_id() << std::endl;
	promiseObj->set_value(35);
}

//=================================================================

TestApp::TestApp() {
	flagDataLoaded = false;
}

void TestApp::loadData() {
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	std::cout << "Load Data from XML" << std::endl;
	std::lock_guard<std::mutex> guard(mutex);
	flagDataLoaded = true;
}	

void TestApp::mainTask() {
	std::cout << "Do Some Handshaking" << std::endl;
	mutex.lock();
	while (flagDataLoaded != true) {
		mutex.unlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		mutex.lock();
	}
	mutex.unlock();
	std::cout << "Do Processing On loaded Data" << std::endl;
}

TestAppConditions::TestAppConditions() {
	flagDataLoaded = false;
}

void TestAppConditions::loadData() {
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	std::cout << "Load Data from XML" << std::endl;
	std::lock_guard<std::mutex> guard(mutex);
	flagDataLoaded = true;
	conditionVar.notify_one();
}

bool TestAppConditions::isDataLoaded() {
	return flagDataLoaded;
}

void TestAppConditions::mainTask() {
	std::cout << "Do Some Handshaking" << std::endl;
	std::unique_lock<std::mutex> mlock(mutex);
	conditionVar.wait(mlock, std::bind(&TestAppConditions::isDataLoaded, this));
	std::cout << "Do Processing On loaded Data" << std::endl;
}



//=================================================================

Thread::Thread() {
	mainExecutor();
}

Thread::~Thread() {
}

std::string fetchDataFromDB(std::string recoveredData) {
	std::this_thread::sleep_for(std::chrono::seconds(5));
	return "DB_" + recoveredData;
}

std::string fetchDataFromFile(std::string recoveredData) {
	std::this_thread::sleep_for(std::chrono::seconds(5));
	return "File_" + recoveredData;
}


void Thread::mainExecutor() {
#ifdef	FUNCTION_POINTER
	std::thread _objThread_Pointer(threadFunction);
	for (int i = 0; i < 10000; i++) {
		std::cout << "Main Thread" << std::endl;
	}
	_objThread_Pointer.join();
#endif
#ifdef FUNCTION_OBJECTS
	std::thread _objThread_Objects((DisplayThread()));
	for (int i = 0; i < 10000; i++) {
		std::cout << "Main Thread" << std::endl;
	}
	_objThread_Objects.join();
#endif
#ifdef FUNCTION_LAMBDAS
	std::thread _objThread_Lambda([](){
		for (int i = 0; i < 10000; i++) {
			std::cout << "Function Thread: " << std::this_thread::get_id() << std::endl;
		}
		});
	for (int i = 0; i < 10000; i++) {
		std::cout << "Main Thread: " << std::this_thread::get_id() << std::endl;
	}

	if (_objThread_Lambda.joinable()) {
		_objThread_Lambda.join();

	}
#endif

#ifdef PASSING ARGUMENT_EXAMPLE
	int value = 100;
	std::cout << "In Main Thread : Before Thread Start value = " << value << std::endl;
	std::thread threadObject(threadFunctionCallback, std::ref(value));
	threadObject.join();
	std::cout << "In Main Thread : After Thread Joins value = " << value << std::endl;
#endif

#ifdef SHARING_DATA
	DummyClass dummyObj;
	int value = 100;
	std::thread threadObj(&DummyClass::sampleMemberFunction, &dummyObj, value);
	threadObj.join();
#endif

#ifdef RACE_CONDITIONS_EXAMPLE
	int value = 0;
	for(int k = 0; k < 1000; k++) 
		if((value = testMultithreadWallet()) != 5000)
			std::cout << "Error at count = " << k << " Money in Wallet = " << value << std::endl;
		else
			std::cout << "Count = " << k << " Money in Wallet = " << value << std::endl;
#endif

#ifdef EVENT_HANDLING_EXAMPLE
	TestApp app;
	std::thread thread_1(&TestApp::mainTask, &app);
	std::thread thread_2(&TestApp::loadData, &app);
	thread_2.join();
	thread_1.join();
#endif

#ifdef CONDITION_VARIABLES_EXAMPLE
	TestAppConditions app;
	std::thread thread_1(&TestAppConditions::mainTask, &app);
	std::thread thread_2(&TestAppConditions::loadData, &app);
	thread_2.join();
	thread_1.join();
#endif

#ifdef FUTURE_PROMISE_EXAMPLE
	std::promise<int> promiseObj;
	std::future<int> futureObj = promiseObj.get_future();
	std::thread th(initializer, &promiseObj);
	std::cout << futureObj.get() << std::endl;
	th.join();
#endif

#ifdef ASYNC_EXAMPLE
	std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
	std::future<std::string> resultFromDB = std::async(std::launch::async, fetchDataFromDB, "Data");
	std::string fileData = fetchDataFromFile("Data");
	std::string dbData = resultFromDB.get();
	std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
	auto diff = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
	std::cout << "Total Time Taken = " << diff << " Seconds" << std::endl;
	std::string data = dbData + " :: " + fileData;
	std::cout << "Data = " << data << std::endl;
#endif

#ifdef PACKAGE_TASK_EXAMPLE

	//std::packaged_task<std::string(std::string)> task(fetchDataFromDB);
	//std::future<std::string> result = task.get_future();
	//std::thread thread_t(std::move(task), "Arg");
	//thread_t.join();
	//std::string output = result.get();
	//std::cout << output << std::endl;

	/*  LAMBDA FUNCTION */
	//std::packaged_task<std::string(std::string)> task([](std::string token) {
	//	std::string data = "Got Data " + token;
	//	return data;
	//	});
	//std::future<std::string> result = task.get_future();
	//std::thread thread_t(std::move(task), "Arg");
	//thread_t.join();
	//std::string output = result.get();
	//std::cout << output << std::endl;
	
	/* FUNCTION OBJECT*/

	std::packaged_task<std::string(std::string)> task(std::move(StrDBDataFetcher()));
	std::future<std::string> result = task.get_future();
	std::thread th(std::move(task), "Arg");
	th.join();
	std::string data = result.get();
	std::cout << data << std::endl;
#endif
}
