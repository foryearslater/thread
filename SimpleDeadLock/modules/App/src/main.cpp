#include <iostream>
#include <mutex>
#include <thread>

std::mutex mutex1;
std::mutex mutex2;
std::mutex mutex3;

void Bar() {
	mutex1.lock();
	mutex1.unlock();
}

void Foo() {
	mutex1.lock();
	Bar();
	mutex1.unlock();
}

void Work1() {
	for (int i = 0; i < 1000; ++i)
	{
		//mutex1.lock(); mutex2.lock();
		//mutex2.unlock(); mutex1.unlock();
		std::scoped_lock sl(mutex1, mutex2);
	}
}

void Work2() {
	for (int i = 0; i < 1000; ++i)
	{
		//mutex2.lock(); mutex1.lock();
		//mutex1.unlock(); mutex2.unlock();
		std::scoped_lock sl(mutex2, mutex1);
	}
}

int main() {
	std::jthread th1(Work1);
	std::jthread th2(Work2); // Work1, Work2;

	th1.join();
	th2.join();

	std::cout << "Done" << std::endl;

	return 0;
}