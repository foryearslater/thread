#include <iostream>
#include <thread>
#include <chrono>
#include <stop_token>

using namespace std::chrono_literals;

void ThreadRun(std::stop_token token) {
	std::stop_callback cb(token, []() {std::cout << std::endl; std::cout << "Stopped Callback." << std::endl; });
	while (true)
	{
		if (token.stop_requested())
		{
			std::cout << "To Break the Loop" << std::endl;
			break;
		}
		std::cout << ".";
		std::this_thread::sleep_for(200ms);
	}
	std::cout << "Thread To Stop" << std::endl;
}

int main() {
	std::jthread my_thread(ThreadRun);
	std::this_thread::sleep_for(3s);
	my_thread.request_stop();

	return 0;
}