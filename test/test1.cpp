#include <iostream>
#include <vector>
#include <chrono>
#include <thread>

#include "../src/GeneticAlgorithm/GeneticAlgorithm.h"

#define PI 3.14159265

using namespace std;


inline std::chrono::steady_clock::time_point get_current_time_fenced() {
    std::atomic_thread_fence(std::memory_order_seq_cst);
    auto res_time = std::chrono::steady_clock::now();
    std::atomic_thread_fence(std::memory_order_seq_cst);
    return res_time;
}

template<class D>
inline long long to_us(const D &d) {
    return std::chrono::duration_cast<std::chrono::microseconds>(d).count();
}

double f(vector<double> args) {
    return exp(-(pow((args[0] - PI), 2) + pow((args[1] - PI), 2)));
}


int main() {
    GeneticAlgorithm geneticAlgorithm(10, 0.6, 2, f, -100, 100, 0.05, 24, 100, 4);
    auto start = get_current_time_fenced();

    geneticAlgorithm.iterate_until(1000, true);
    auto finish = get_current_time_fenced() - start;

    geneticAlgorithm.find_max();
    std::cout << to_us(finish) << std::endl;
}
