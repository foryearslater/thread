#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <queue>
#include <sstream>
#include <format>

using namespace std::chrono_literals;

class Task {
public:
	virtual void Execute() = 0;
};

class MyThreadPool {
public:
	void StartThreads();

	void Run();

	void AddTask(Task* a_task);
private:
	std::vector<std::thread> m_threads;
	std::queue<Task*> m_tasks;
	std::condition_variable cv;
	std::mutex m_mutex;
};

void MyThreadPool::StartThreads()
{
	std::lock_guard m(m_mutex);
	for (int i = 0; i < 4; ++i)
	{
		m_threads.emplace_back([this] {this->Run(); });
	}
}

void MyThreadPool::Run()
{
	while (true)
	{
		std::unique_lock lock(m_mutex);
		cv.wait(lock, [&] { return !m_tasks.empty(); });
		Task* cur_task = m_tasks.front();
		m_tasks.pop();
		lock.unlock();
		cur_task->Execute();
	}
}

void MyThreadPool::AddTask(Task* a_task)
{
	std::unique_lock lock(m_mutex);
	m_tasks.push(a_task);
	cv.notify_one();
}

class MyShowValueTask : public Task {
public:
	MyShowValueTask(int value) { m_value = value; }
	void Execute() override {
		auto tid = std::this_thread::get_id();
		std::stringstream ss;
		ss << "ThreadID " << tid << ": " << m_value << std::endl;
		std::cout << ss.str();
	};
private:
	int m_value;
};

int main() {

	MyThreadPool pool;
	pool.StartThreads();

	for (int i = 0; i < 100; ++i)
	{
		pool.AddTask(new MyShowValueTask(i));
	}

	std::this_thread::sleep_for(500s);
	return 0;
}