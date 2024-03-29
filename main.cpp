#include "tape.hpp"

int main(int argc, char** argv) {

	if (argc != 4) {
		std::cout << "Incorrect params given. Aborting...\n";
		return 0;		
	}

	auto start = std::chrono::system_clock::now();

	int RAM_limit = atoi(argv[3]);
	if (RAM_limit <= 0) {
		std::cout << "RAM limit must be positive number\n";
		return 0;
	}

    Sorter sorter_instance(argv[1], argv[2], atoi(argv[3]));
    sorter_instance.externalSort();

	auto end = std::chrono::system_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	std::cout << "Elapsed time: " <<  elapsed.count() << " ms\n";
    
	return 0;
}
