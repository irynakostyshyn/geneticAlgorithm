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
    long long int elite_number = 10;
    double crossover_percent = 0.6;
    unsigned int number_of_args = 2;
    const function<double(vector<double>)> &func = f;
    double lower_bound = -100;
    double upper_bound = 100;
    double mutation_probability = 0.05;
    unsigned int chromosome_length = 24;
    unsigned int population_size = 100;
    unsigned int threads_number = 4;

    GeneticAlgorithm geneticAlgorithm(
            elite_number, crossover_percent, number_of_args,
            func,
            lower_bound, upper_bound, mutation_probability,
            chromosome_length, population_size,
            threads_number
    );
    auto start = get_current_time_fenced();

    geneticAlgorithm.iterate_until(1000, true);
    auto finish = get_current_time_fenced() - start;

    geneticAlgorithm.find_max();
    std::cout << to_us(finish) << std::endl;
}
