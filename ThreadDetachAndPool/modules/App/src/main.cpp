#include <iostream>
#include <chrono>
#include <thread>
#include <vector>

void download(const std::string& a_file) {
    for (int i = 0; i <= 10; ++i)
    {
        std::cout << "Downloading " << a_file << "  " << i * 10 << "%" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    std::cout << "Download Complete: " << a_file << std::endl;
}

std::vector<std::thread> pool; // std::jthread

void create_thread() {
    std::thread down_thread([&] {download("hello.zip"); });
    //down_thread.detach();
    pool.push_back(std::move(down_thread));
}

int main(){
    create_thread();

    for (auto& th: pool)
    {
        th.join();
    }

    return 0;
}