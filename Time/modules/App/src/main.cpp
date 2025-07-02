#include <iostream>
#include <chrono>
#include <thread>

using namespace std::chrono_literals;

int main(){

    auto t0 = std::chrono::steady_clock::now();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    auto t1 = std::chrono::steady_clock::now();

    auto dur = t1 - t0;

    int64_t sec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
    double milli_seconds = std::chrono::duration_cast<std::chrono::duration
        <double, std::milli>>(dur).count();

    std::cout << sec << std::endl;
    std::cout << milli_seconds << std::endl;

	std::cout << std::chrono::milliseconds(1s).count() << std::endl;
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(1s).count() << std::endl;

    std::cout << "HelloWorld" << std::endl;
    return 0;
}