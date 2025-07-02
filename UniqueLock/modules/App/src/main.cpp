#include <iostream>
#include <mutex>

using namespace std::chrono_literals;

std::timed_mutex mutex;

void Work1() {
	std::unique_lock ulock(mutex, std::defer_lock);
	ulock.lock();
	std::cout << "Work1 Locked" << std::endl;
	std::this_thread::sleep_for(3s);
	std::cout << "Work1 To Die" << std::endl;
}

void Work2() {
	std::unique_lock ulock(mutex, std::defer_lock);
	if (ulock.try_lock_for(1s))
	{
		std::cout << "Work2 Locked" << std::endl;
	}
	else
	{
		std::cout << "Work2 Not Lock" << std::endl;
	}
}

void Work3() {
	std::unique_lock ulock(mutex, std::defer_lock);
	if (ulock.try_lock_for(4s))
	{
		std::cout << "Work3 Locked" << std::endl;
	}
	else
	{
		std::cout << "Work3 Not Lock" << std::endl;
	}
}

int main() {
	std::jthread th1(Work1);

	std::this_thread::sleep_for(200ms);

	std::jthread th2(Work2);
	std::jthread th3(Work3);

	return 0;
}