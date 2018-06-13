#include "ThreadedFunctions.h"


void fitness_value_threads_divider(
        std::vector<Individual> &individuals,
        std::function<double(std::vector<double>)> &f,
        long double &step,
        double &lower_bound,
        unsigned int &threads_number
) {

    unsigned long single_part = individuals.size() / threads_number;
    unsigned long remainder = individuals.size() % threads_number;
    unsigned long cur = 0;

    std::vector<std::thread> threads;
    std::mutex m;

    for (int i = 0; i < threads_number; ++i) {
        if (i == threads_number - 1) {
            single_part += remainder;
        }
        threads.emplace_back(fitness_value_worker, std::ref(individuals),
                             cur, std::min(cur + single_part, individuals.size()),
                             std::ref(lower_bound), std::ref(step),
                             std::ref(f)
        );
        cur += single_part;
    }

    for (auto &t: threads) {
        t.join();
    }
}

void fitness_value_worker(std::vector<Individual> &individuals,
        unsigned long first, unsigned long last, double &lower_bound, long double &step,
        std::function<double(std::vector<double>)> &f
) {
    for (; first < last; ++first) {
        individuals[first].update_values(step, lower_bound);
        auto result = f(individuals[first].get_values());
        individuals[first].set_fitness_value(result);
    }
}
