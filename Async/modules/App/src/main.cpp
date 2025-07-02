#include <iostream>
#include <thread>
#include <chrono>
#include <future>

#define FIBO_NUM 50000000

int64_t fibonacci(int64_t n) {
	if (n <= 1) {
		return n;
	}

	int64_t a = 0, b = 1;
	for (int i = 2; i <= n; ++i) {
		int64_t temp = a + b;
		a = b;
		b = temp;
	}

	return b;
}

void UsePromise() {
	auto t0 = std::chrono::steady_clock::now();
	std::promise<int64_t> pret;
	std::thread calc_thread([&] {
		auto ret = fibonacci(FIBO_NUM);
		pret.set_value(ret);
		});
	std::future<int64_t> fret = pret.get_future();
	auto t1 = std::chrono::steady_clock::now();
	
	int64_t real_answer = fret.get();

	auto t2 = std::chrono::steady_clock::now();

	std::cout << "Fibo " << FIBO_NUM << ": " << real_answer << std::endl;
	int time1 = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
	int time2 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t0).count();
	std::cout << "time1 takes " << time1 << "ms" << std::endl;
	std::cout << "time2 takes " << time2 << "ms" << std::endl;

	calc_thread.join();
}

int main() {

	UsePromise();

	auto t0 = std::chrono::steady_clock::now();
	std::future<int64_t> future_answer = std::async([&] {return fibonacci(FIBO_NUM);});
	auto t1 = std::chrono::steady_clock::now();

	// future_answer.wait_for();
	// future_answer.wait_until();
	int64_t real_answer = future_answer.get();

	auto t2 = std::chrono::steady_clock::now();

	std::cout << "Fibo " << FIBO_NUM << ": " << real_answer << std::endl;
	int time1 = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
	int time2 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t0).count();

	std::cout << "time1 takes " << time1 << "ms" << std::endl;
	std::cout << "time2 takes " << time2 << "ms" << std::endl;
	
    return 0;
}