#include <iostream>
#include <mutex>
#include <vector>

std::mutex mutex;
std::condition_variable cv;

std::vector<double> data;

using namespace std::chrono_literals;

void work_thread() {
	std::unique_lock lk(mutex);
	while (true)
	{
		cv.wait(lk, [] {return data.size() > 0; });

		double back = data.back();
		data.pop_back();

		std::cout << "Work Thread Get " << back << std::endl;
	}
}

int main() {
	std::jthread th(work_thread);

	std::this_thread::sleep_for(1s);
	data.push_back(1);
	cv.notify_one();

	return 0;
}