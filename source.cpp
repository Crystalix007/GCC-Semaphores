#include <iostream>
#include <semaphore>
#include <future>
#include <thread>
#include <vector>

const constexpr std::uint32_t threadCount = 10000;

std::atomic_uint32_t wakeCount{ 0 };

std::counting_semaphore<threadCount> runSemaphore{0};
std::counting_semaphore<threadCount> wakeSemaphore{0};

void sleep_function();

int main() {
	std::vector<std::future<void>> asyncFunctions{};

	std::cout << "Creating sleeping functions\n";

	for (std::uint32_t i = 0; i < threadCount; i++) {
		asyncFunctions.push_back(std::async(sleep_function));
	}

	std::cout << "Waiting for sleep functions to run\n";

	for (std::uint32_t i = 0; i < threadCount; i++) {
		runSemaphore.acquire();
	}

	std::cout << "Waiting for sleep functions to wake up\n";

	for (std::uint32_t i = 0; i < threadCount; i++) {
		wakeSemaphore.release();
		// std::this_thread::sleep_for(std::chrono::microseconds{ 1 });
	}

	// This however works fine, since the implementation wakes all threads if parameter > 1.
	// wakeSemaphore.release(threadCount);

	std::cout << "So far " << wakeCount << " threads woken\n";
	std::cout << "Waiting for sleep functions to finish\n";

	for (std::uint32_t i = 0; i < threadCount; i++) {
		asyncFunctions[i].get();
	}

	std::cout << "Finished\n";

	return 0;
}

void sleep_function() {
	runSemaphore.release();
	wakeSemaphore.acquire();
	wakeCount++;
}
