#include "pch.h"
#include <windows.h>
#include <iostream>
#include <vector>
#include <thread>
#include <algorithm>
#include <cstdlib> 
#include "../ICompute.h"

int compare_ints(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

void SortChunk(int* chunk_start, size_t chunk_size) {
    std::qsort(chunk_start, chunk_size, sizeof(int), compare_ints);

    for (volatile int i = 0; i < 10000000; ++i) {}
}

class ConcreteCompute : public ICompute {
public:
    void RunComputation() override {
        std::cout << "  [ComputationDLL]: Обчислення розпочато..." << std::endl;

        const size_t TOTAL_ELEMENTS = 200000000;
        std::vector<int> data;
        try {
            data.resize(TOTAL_ELEMENTS);
            std::cout << "  [ComputationDLL]: Виділено пам'ять для " << TOTAL_ELEMENTS << " елементів." << std::endl;
        }
        catch (const std::bad_alloc& e) {
            std::cout << "  [ComputationDLL]: ПОМИЛКА: Не вдалося виділити пам'ять: " << e.what() << std::endl;
            return;
        }

        for (size_t i = 0; i < TOTAL_ELEMENTS; ++i) {
            data[i] = rand();
        }

        unsigned int num_threads = std::thread::hardware_concurrency();
        if (num_threads == 0) num_threads = 4;

        std::cout << "  [ComputationDLL]: Запуск " << num_threads << " потоків для сортування..." << std::endl;

        std::vector<std::thread> threads;
        size_t chunk_size = TOTAL_ELEMENTS / num_threads;

        for (unsigned int i = 0; i < num_threads; ++i) {
            int* chunk_start = &data[i * chunk_size];
            size_t current_chunk_size = (i == num_threads - 1) ? (TOTAL_ELEMENTS - i * chunk_size) : chunk_size;

            threads.emplace_back(SortChunk, chunk_start, current_chunk_size);
        }

        for (auto& th : threads) {
            th.join();
        }

        std::cout << "  [ComputationDLL]: Усі потоки завершили сортування." << std::endl;
    }
};

extern "C" __declspec(dllexport) ICompute* CreateComputeInstance() {
    return new ConcreteCompute();
}