#include <iostream>
#include <mutex>
#include <chrono>

#define ADD_SIZE 10000000

std::mutex mtx;
int value = 0;

void FuncWithoutMtxLock() {
	for (int i = 0; i < ADD_SIZE; i++)
	{
		value++;
	}
}

void FuncWithMtxLock() {
	for (int i = 0; i < ADD_SIZE; i++)
	{
		mtx.lock();
		value++;
		mtx.unlock();
	}
}

void FuncWithMtxLockRaised() {
	try
	{
		for (int i = 0; i < ADD_SIZE; i++)
		{
			mtx.lock();
			value++;
			if (value == ADD_SIZE / 2)
			{
				throw std::exception();
			}
			mtx.unlock();
		}
	}
	catch (const std::exception&)
	{
		std::cout << "exception" << std::endl;
	}
}

void FuncWithGuardLockRaised() {
	try
	{
		for (int i = 0; i < ADD_SIZE; i++)
		{
			std::lock_guard grd(mtx);
			value++;
			if (value == ADD_SIZE / 2)
			{
				throw std::exception();
			}
		}
	}
	catch (const std::exception&)
	{
		std::cout << "exception" << std::endl;
	}
}

void FuncWithUniqueLock() {
	for (int i = 0; i < ADD_SIZE; i++)
	{
		std::unique_lock uni_lock(mtx, std::defer_lock);
		uni_lock.lock();
		value++;
	}
}

void FuncWithLoopLocked() {
	std::lock_guard grd(mtx);
	for (int i = 0; i < ADD_SIZE; i++)
	{
		value++;
	}
}

void Func2() {
	for (int i = 0; i < ADD_SIZE; i++)
	{
		mtx.lock();
		value++;
		mtx.unlock();
	}
}

int main(){
	auto t0 = std::chrono::steady_clock::now();

	//std::thread th1([] {FuncWithoutMtxLock(); }); // Maybe Wrong Answer
	//std::thread th1([] {FuncWithMtxLock(); }); // Right, Not So Safe
	//std::thread th1([] {FuncWithMtxLockRaised(); }); // DeadLock
	std::thread th1([] {FuncWithGuardLockRaised(); }); // Right
	//std::thread th1([] {FuncWithUniqueLock(); }); // Right
	//std::thread th1([] {FuncWithLoopLocked(); }); // Right, Faster In the Case
    std::thread th2([] {Func2(); });

    th1.join();
	th2.join();

	auto t1 = std::chrono::steady_clock::now();

	double milli_time = std::chrono::duration_cast<std::chrono::duration<double, 
		std::milli>>(t1 - t0).count();

	std::cout << "Value is: " << value << std::endl;
	std::cout << "Time takes: " << milli_time << "ms" << std::endl;

    return 0;
}