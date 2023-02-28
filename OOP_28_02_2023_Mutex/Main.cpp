#include<iostream>
#include<string>
#include<mutex>
//#include<>

class Logger {
public:
	void log(const std::string& massage) {
		std::lock_guard<std::mutex> lock(_mutex);
		std::cout << "[" << _counter << ']' << massage << '\n';
		_counter++;
		std::lock_guard<std::mutex> lock2(_secondMutex);
	}
	/*void successMessage() {
		std::lock_guard<std::mutex> lock(_mutex);
		std::cout << "succes Message\n";
	}*/

	void Func() {
		std::lock_guard<std::mutex> lock(_secondMutex);
		std::cout << "some text\n";
		std::lock_guard<std::mutex> lock2(_mutex);
	}
	// deadlock
private:
	int _counter = 0;
	std::mutex _mutex;
	std::mutex _secondMutex;
};

int main() {

	Logger logger;
	logger.log("hello!");
	logger.log("info massage");
	std::cout << "Hello World!\n";

	std::thread t1 = std::thread([&logger]() {
		std::string threadName = "t1";
		for (size_t i = 0; i < 100; i++) {
			logger.log(threadName + " going " + std::to_string(i) + " iteration of work\n");
			logger.Func();
		}
		});

	std::thread t2 = std::thread([&logger]() {
		std::string threadName = "t2";
		for (size_t i = 0; i < 100; i++) {
			logger.log(threadName + " going " + std::to_string(i) + " iteration of work\n");
		}
		});
	t1.join(); t2.join();
	return 0;
}