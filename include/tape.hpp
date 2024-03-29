
#include <fstream>
#include <ios>
#include <string>
#include <thread>
#include <vector>
#include <chrono>
#include <iostream>
#include <algorithm>
#include <limits.h>

#include "heap.hpp"

class Tape {
private:
    std::fstream tape_file;
    // IO delay
    unsigned io_delay = 1;
    // Shift delay
    unsigned shift_delay = 1;
    // Skip delay
    unsigned skip_delay = 1;

private:
    // Read delay info from config.txt file
    void ReadConfigParams() {
        std::fstream config_file("config.txt");
        if (!config_file) {
            std::cout << "[-] \"config.txt\" file doesn't exist, create one.\n";
        }

        config_file >> io_delay;
        config_file >> shift_delay;
        config_file >> skip_delay;
    }

public:
    Tape(const std::string& filename): tape_file(filename) {
        ReadConfigParams();
    }

    Tape(const std::string& filename, std::ios::openmode mode): tape_file(filename, mode) {
        ReadConfigParams();

        if (!tape_file.is_open()) {
            tape_file.open(filename, std::fstream::app | std::fstream::out | std::fstream::in);
        }
    }

    // Read data from tape
    template<typename T>
    bool ReadSingle(T& data) {
        // Assuming this method reads and moves the tape at the same time
        std::this_thread::sleep_for(std::chrono::milliseconds(io_delay + shift_delay));
        if (tape_file >> data) {
            return true;
        }
        return false;
    }

    // Put data onto tape
    template<typename T>
    bool PutSingle(T& data) {
        // Assuming this method reads and moves the tape at the same time
        std::this_thread::sleep_for(std::chrono::milliseconds(io_delay + shift_delay));

        if (tape_file << data << ' ') {
            return true;
        }
        return false;
    }

    // Move tape to the begining
    void ResetPos() {
        std::this_thread::sleep_for(std::chrono::milliseconds(skip_delay));
        tape_file.seekg(0, std::ios::beg);
    }
};  // class Tape

// Implementation of External sort
class Sorter {
private:
    std::string input_fname_;
    std::string output_fname_;

    // Default param, changed 
    // upon construction
    int tape_size_ = 10;

    // Vector of support tapes, 
    // where bits of array stored
    std::vector<Tape> sup_tapes_;

public:
    Sorter() = default;

    Sorter(std::string input, std::string output, int RAM_limit):
        input_fname_(input), output_fname_(output), tape_size_(RAM_limit) {
    }

    void externalSort() {
        // Generate sorted support tapes
        InitAndFillSupTapes();

        // Merge suppport tapes onto main one
        MergeKFiles();
    }

private:

    // Merging all support tapes together, without overusing of RAM
    // Using heap for effecitve search of min. element in all sup tapes
    void MergeKFiles() {
        Tape output_tape(output_fname_, std::fstream::trunc | std::fstream::out | std::fstream::in);

        MinHeapNode* heap_array = new MinHeapNode[sup_tapes_.size()];

        int i;
        for (i = 0; i < sup_tapes_.size(); i++) {
            if (!(sup_tapes_[i].ReadSingle(heap_array[i].element)))
                break;
            heap_array[i].tape_index = i;
        }
        MinHeap MinHeapInstance(heap_array, i);

        int count = 0;
        while (count != i) {
            MinHeapNode root = MinHeapInstance.getMin();

            output_tape.PutSingle(root.element);

            if (!(sup_tapes_[root.tape_index].ReadSingle(root.element))) {
                root.element = INT_MAX;
                count++;
            }

            MinHeapInstance.replaceMin(root);
        }

        delete[] heap_array;
    }

    // Sort parts of data, which can fit inside RAM
    // Then write it on support tapes
    void InitAndFillSupTapes() {
        Tape InTape(input_fname_);

        // Create files, related to support tapes
        // Fill each support tape with sorted bit of data

        system("mkdir -p tmp");

        std::vector<int> tape_array(tape_size_);
        bool             more_input       = true;
        int              next_output_file = 0;

        while (more_input) {
            sup_tapes_.emplace_back("tmp/" + std::to_string(next_output_file) + "_tape.tp",
                                    std::fstream::trunc | std::fstream::out | std::fstream::in);

            // Read elements from input tape into RAM
            int num_elements = 0;
            for (int i = 0; i < tape_size_; i++) {
                if (!(InTape.ReadSingle(tape_array[i]))) {
                    more_input = false;
                    break;
                }
                num_elements++;
            }

            // Sort array
            std::sort(tape_array.begin(), tape_array.begin() + num_elements);

            // Write the sorted elements to one of support tapes
            for (int j = 0; j < num_elements; j++) {
                sup_tapes_[next_output_file].PutSingle(tape_array[j]);
            }

            next_output_file++;
        }

        // Reset poisitions of cursor in support tapes
        for (int i = 0; i < sup_tapes_.size(); ++i) {
            sup_tapes_[i].ResetPos();
        }
    }
};  // class Sorter
