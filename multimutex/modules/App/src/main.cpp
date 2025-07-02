#include <iostream>
#include <mutex>
#include <chrono>
#include <thread>
std::mutex mtx1;
std::mutex mtx2;

using namespace std::chrono_literals;

void Lock12() {
	std::lock_guard grd1(mtx1);
	std::this_thread::sleep_for(500ms);
	std::lock_guard grd2(mtx2);
}

void Lock21() {
	std::lock_guard grd1(mtx2);
	std::this_thread::sleep_for(500ms);
	std::lock_guard grd2(mtx1);
}

void ScopedLock12() {
	for (int i = 0; i < 20; ++i)
	{
		std::scoped_lock sl(mtx1, mtx2);
		std::this_thread::sleep_for(20ms);
	}
}

void ScopedLock21() {
	for (int i = 0; i < 20; ++i)
	{
		std::scoped_lock sl(mtx2, mtx1);
		std::this_thread::sleep_for(20ms);
	}
}

int main() {
	std::thread th1([] {Lock12(); }); // DEADLOCK
	std::thread th2([] {Lock21(); }); // DEADLOCK
 
	//std::thread th1([] {ScopedLock12(); });
	//std::thread th2([] {ScopedLock21(); });

	th1.join();
	th2.join();

	std::cout << "end" << std::endl;

    return 0;
}