#include "ThreadedFunctions.h"


double fitness_value_threads_divider(
        std::vector<Individual> &individuals,
        std::function<double(std::vector<double>)> &f,
        long double &step,
        double &lower_bound,
        unsigned int &threads_number
) {
    double fitnessesSum = 0.0;

    unsigned long single_part = individuals.size() / threads_number;
    unsigned long remainder = individuals.size() % threads_number;
    unsigned long cur = 0;

    std::vector<std::thread> threads;
    std::mutex m;

    for (int i = 0; i < threads_number; ++i) {
        if (i == threads_number - 1) {
            single_part += remainder;
        }
        threads.emplace_back(fitness_value_worker,
                             std::ref(m), std::ref(individuals), std::ref(fitnessesSum),
                             cur, std::min(cur + single_part, individuals.size()),
                             std::ref(lower_bound), std::ref(step),
                             std::ref(f)
        );
        cur += single_part;
    }

    for (auto &t: threads) {
        t.join();
    }

    return fitnessesSum;
}

void fitness_value_worker(
        std::mutex &m, std::vector<Individual> &individuals, double &fitnessesSum,
        unsigned long first, unsigned long last, double &lower_bound, long double &step,
        std::function<double(std::vector<double>)> &f
) {
    double localFitnessesSum = 0.0;
    std::vector<double> double_values;
    for (; first < last; ++first) {
        individuals[first].update_values();
        auto values = individuals[first].get_values();
        for (double &value : values) {
            double_values.push_back(lower_bound + step * value);
        }
        auto result = f(double_values);
//        auto result = 0;
        individuals[first].set_fitness_value(result);
        localFitnessesSum += result;
    }
    m.lock();
    fitnessesSum += localFitnessesSum;
    m.unlock();
}
