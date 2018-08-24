#include <mutex>
#include <thread>
#include <chrono>
#include <iostream>

std::mutex mutex1;
std::mutex mutex2;

void func1() {
	using namespace std::chrono_literals;
	mutex1.lock();
	std::cout << "mutex1 lock" << std::endl;
	//mutex1.lock();
	std::cout << "re lock mutex1" << std::endl;
	std::this_thread::sleep_for(1s);
	mutex2.lock();
	std::cout << "Func1 is running" << std::endl;
	mutex2.unlock();
	mutex1.unlock();
}

void func2() {
	mutex2.lock();
	std::cout << "mutex2 lock" << std::endl;
	mutex1.lock();
	std::cout << "Func2 is running" << std::endl;
	mutex1.unlock();
	mutex2.unlock();
}

int main() {
	//std::thread t1(func1);
	//std::thread t2(func2);
	//t1.join();
	//t2.join();
	{
		std::mutex mu;
		mu.lock();
	}
	std::cout << "No dead lock" << std::endl;
}