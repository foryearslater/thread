#include <iostream>
#include <thread>

using namespace std::chrono_literals;

class TThread
{
public:
	void Execute() { DoExecute(); }
	void Start() { m_thread = std::thread(&TThread::Execute, this); }
	void Stop()
	{
		m_exit = true;
		Wait();
	}
	void Wait()
	{
		if (m_thread.joinable())
			m_thread.join();
	}
	bool IsExit() { return m_exit; }

protected:
	virtual void DoExecute() = 0;

private:
	std::thread m_thread;
	std::atomic<bool> m_exit{ false };
};

class TMyThread : public TThread
{
protected:
	void DoExecute() override;
};

void TMyThread::DoExecute()
{
	std::cout << "Thread Begin" << std::endl;

	while (!IsExit())
	{
		std::cout << "." << std::flush;
		std::this_thread::sleep_for(100ms);
	}

	std::cout << std::endl;
	std::cout << "ThreadExit" << std::endl;
}

class MyDocument;

class SaveFileThread : public TThread {
public:
	void SetSavePath() {};
	void SetDocument() {};
protected:
	void DoExecute() override {
		//m_document->SaveToFile(m_file_name); 
	};
private:
	std::string m_file_name;
	MyDocument* m_document;
};

int main()
{
	TMyThread my_thread;
	my_thread.Start();

	std::this_thread::sleep_for(2s);

	my_thread.Stop();

	return 0;
}