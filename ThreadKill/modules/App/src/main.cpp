#include <iostream>
#include <thread>
#include <windows.h>
#include <processthreadsapi.h>

using namespace std::chrono_literals;

class MyResource {
public:
    MyResource() { std::cout << "Resource Init" << std::endl; }
    ~MyResource() { std::cout << "Resource Final" << std::endl; }
};

void Foo() {
    MyResource res;
    // Lock?
    std::this_thread::sleep_for(5s);
}

int main(){
    std::thread th(Foo);

    std::this_thread::sleep_for(2s);

    TerminateThread(th.native_handle(), 0);
	th.detach();
	std::this_thread::sleep_for(5s);
    //th.join();

    return 0;
}