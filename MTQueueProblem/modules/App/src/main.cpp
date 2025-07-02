#include <iostream>
#include <thread>
#include <vector>
// #include <mutex>
// #include <condition_variable>

using namespace std::chrono_literals;

// MTQueue 希望设计是一个多线程安全的类， 目前没有加锁
// 请通过 mutex 和 condition variable 使得 MTQueue成为一个多线程安全的类， 并使得消费者可以在没有生产的产品时可以阻塞等待被唤醒

template <class T>
class MTQueue
{
	std::vector<T> m_arr;

public:
	T pop()
	{
		T ret = std::move(m_arr.back());
		m_arr.pop_back();
		return ret;
	}

	void push(T val)
	{
		m_arr.push_back(std::move(val));
	}
};

int main()
{
	MTQueue<int> foods;

	std::thread t1([&]
				   {
		for (int i = 0; i < 2; i++) {
			auto food = foods.pop();
			std::cout << "t1 got food:" << food << std::endl;
		} });

	std::thread t2([&]
				   {
		for (int i = 0; i < 2; i++) {
			auto food = foods.pop();
			std::cout << "t2 got food:" << food << std::endl;
		} });

	std::this_thread::sleep_for(500ms);

	foods.push(42);
	foods.push(233);

	t1.join();
	t2.join();

	return 0;
}