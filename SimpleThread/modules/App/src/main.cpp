#include <iostream>
#include <chrono>
#include <thread>
#include <vector>

void download(const std::string& a_file) {
    for (int i = 0; i <= 10; ++i)
    {
        std::cout << "Downloading " << a_file << "  " << 
            i * 10 << "%" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    std::cout << "Download Complete: " << a_file << std::endl;
}

int main() {
	std::thread t1([&] {download("hello.zip"); });
	//std::thread t2(download, "hello.zip");

    t1.join();
    //t2.join();

    return 0;
}