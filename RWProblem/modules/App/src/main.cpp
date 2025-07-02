#include <iostream>
#include <thread>
#include <shared_mutex>
#include <vector>
#include <chrono>

// WRData 希望设计成一个读写安全的类
// 使用读写锁的方式对WRData类进行修改

class WRData
{
public:
	void read()
	{
		m_mutex.lock();
		std::cout << "Reading data: " << data << std::endl;
		m_mutex.unlock();
	}

	void write(int newData)
	{
		m_mutex.lock();
		std::cout << "Writing data: " << newData << std::endl;
		data = newData;
		m_mutex.unlock();
	}

private:
	int data = 0;
	std::mutex m_mutex;
};

void reader(WRData &rwLock)
{
	for (int i = 0; i < 5; ++i)
	{
		rwLock.read();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

void writer(WRData &rwLock, int newData)
{
	for (int i = 0; i < 5; ++i)
	{
		rwLock.write(newData + i * i);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

int main()
{
	WRData rwLock;

	std::vector<std::thread> threads;

	for (int i = 0; i < 3; ++i)
	{
		threads.emplace_back(reader, std::ref(rwLock));
	}

	threads.emplace_back(writer, std::ref(rwLock), 10);

	for (auto &thread : threads)
	{
		thread.join();
	}

	return 0;
}