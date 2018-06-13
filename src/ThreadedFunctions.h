#ifndef THREADEDFUNCTIONS_H
#define THREADEDFUNCTIONS_H

#include "Population.h"
#include <thread>
#include <iostream>
#include <mutex>
#include <functional>


void fitness_value_worker(std::mutex &m, std::vector<Individual> &individuals, double &fitnessesSum,
                          unsigned long first, unsigned long last, double &lower_bound, long double &step,
                          std::function<double(std::vector<double>)> &f
);

double fitness_value_threads_divider(
        std::vector<Individual> &individuals,
        std::function<double(std::vector<double>)> &f,
        long double &step,
        double &lower_bound,
        unsigned int &threads_number
);


#endif //THREADEDFUNCTIONS_H
