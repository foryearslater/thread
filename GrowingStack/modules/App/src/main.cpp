#include <iostream>

#include <chrono>
#include <thread>

using namespace std::chrono_literals;

void WorkThread() {
   
    std::this_thread::sleep_for(200ms);
    WorkThread();
}

int main(){

	std::thread th1(WorkThread);
	std::thread th2(WorkThread);
	std::thread th3(WorkThread);
	std::thread th4(WorkThread);

	th1.join();
	th2.join();
	th3.join();
	th4.join();

    return 0;
}