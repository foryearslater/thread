#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>

using namespace std::chrono_literals;

#define ADD_COUNT 100000000 

int race_data = 0;
std::atomic<int> atom_race_data = 0;
std::mutex mutex;

void ThreadWrite() {
	for (int i = 0; i < ADD_COUNT; ++i)
	{
		race_data++;
	}
}

void ThreadSafeWrite1() {
	race_data = 0;
	mutex.lock();
	for (int i = 0; i < ADD_COUNT; ++i)
	{
		++race_data;
	}
	mutex.unlock();
}

void ThreadSafeWrite2() {
	for (int i = 0; i < ADD_COUNT; ++i)
	{
		mutex.lock();
		race_data++;
		mutex.unlock();
	}
}

void ThreadSafeWrite3() {
	atom_race_data.store(0);
	for (int i = 0; i < ADD_COUNT; ++i)
	{
		atom_race_data.fetch_add(1, std::memory_order::memory_order_relaxed);
	}
}

int main() {
	//std::thread th1(ThreadWrite);
	//std::thread th2(ThreadWrite);
	//std::thread th1(ThreadSafeWrite1);
	//std::thread th2(ThreadSafeWrite1);
	std::thread th1(ThreadSafeWrite2);
	std::thread th2(ThreadSafeWrite2);

	th1.join();
	th2.join();

	std::cout << "Racing Data: " << race_data << std::endl;
	{
		std::chrono::steady_clock::time_point t0 = std::chrono::steady_clock::now();
		ThreadSafeWrite1();
		std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
		int64_t milli_secs = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();

		std::cout << "Write1 takes: " << milli_secs << "ms" << std::endl;
	}
	{
		std::chrono::steady_clock::time_point t0 = std::chrono::steady_clock::now();
		ThreadSafeWrite2();
		std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
		int64_t milli_secs = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
		std::cout << "Write2 takes: " << milli_secs << "ms" << std::endl;
	}
	{
		std::chrono::steady_clock::time_point t0 = std::chrono::steady_clock::now();
		ThreadSafeWrite3();
		std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
		int64_t milli_secs = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
		std::cout << "Write3 takes: " << milli_secs << "ms" << std::endl;
	}
	return 0;
}