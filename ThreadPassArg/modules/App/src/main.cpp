#include <iostream>
#include <chrono>
#include <thread>
#include <vector>

using namespace std;

struct DownloadOptions {
public:
    double Foo = 3.14;
    int Bar = 5;
    DownloadOptions() = default;
    DownloadOptions(const DownloadOptions& that) {
        std::cout << "copy constructor..." << std::endl;
        this->Foo = that.Foo;
        this->Bar = that.Bar;
    }
};

void download(const std::string& a_file, const DownloadOptions& opt) {
	std::cout << "Options: " << opt.Foo << ", " << opt.Bar << std::endl;
    for (int i = 0; i <= 10; ++i)
    {
        std::cout << "Downloading " << a_file << "  " << i * 10 << "%" << std::endl;
        std::this_thread::sleep_for(200ms);
    }
    std::cout << "Download Complete: " << a_file << std::endl;
}

int main(){
    DownloadOptions opt;

    std::thread t1([&] {download("hello.zip", opt); });

    t1.join();

	//std::thread t2(download, "hello.zip", opt);
	std::thread t2(download, "hello.zip", std::ref(opt));
    t2.join();

    return 0;
}