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
	std::vector<std::thread> m_threads; // 1. 线程容器  存放所有创建的工作线程对象
	std::queue<Task*> m_tasks;  // 2. 任务队列 (共享资源)  因为主线程会向里面添加任务，而多个工作线程会从中取出任务
	std::condition_variable cv;   // 3. 条件变量  保护共享资源 m_task, 只允许一个线程访问任务队列，防止多个线程同时修改队列导致数据损坏
	std::mutex m_mutex;  // 4. 互斥锁  优化线程等待 条件变量可以让线程在没有任务时进入休眠/阻塞状态，当新任务被添加时，再由添加任务的线程唤醒它们
};

//创建 4 个线程，每个线程都立即开始执行 MyThreadPool::Run() 方法
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
		std::unique_lock lock(m_mutex);                  // 1. 加锁
		cv.wait(lock, [&] { return !m_tasks.empty(); }); // 2. 等待条件满足
		Task* cur_task = m_tasks.front();                // 3. 从队列取任务
		m_tasks.pop();                                  
		lock.unlock();                                   // 4. 提前解锁
		cur_task->Execute();                             // 5. 执行任务
	}
}

void MyThreadPool::AddTask(Task* a_task)
{
	std::unique_lock lock(m_mutex); // 1. 加锁
	m_tasks.push(a_task);           // 2. 添加任务到队列
	cv.notify_one();                // 3. 唤醒一个等待的线程
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