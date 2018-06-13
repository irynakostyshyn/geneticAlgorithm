#ifndef GENETICALGORITHM_H
#define GENETICALGORITHM_H

#include <vector>
#include <functional>
#include <iostream>
#include <utility>

#include "../ThreadedFunctions/ThreadedFunctions.h"
#include "../Population/Population.h"

class GeneticAlgorithm {
    Population population;

    // number of threads
    unsigned int threads_number;

    // individuals
    unsigned int population_size;
    unsigned int chromosome_length;

    // number of args of the function
    unsigned int number_of_args;

    // working params
    long long elite_number;
    double crossover_percent;
    double mutation_probability;

    // step of the population distribution
    long double step;
    // bounds
    double lower_bound;
    double upper_bound;
    // function to fit
    function<double(vector<double>)> f;
public:
    GeneticAlgorithm(
            long long int elite_number_,
            double crossover_percent_,
            unsigned int number_of_args_,
            function<double(vector<double>)> f_,
            double lower_bound_, double upper_bound_,
            double mutation_probability_,
            unsigned int chromosome_length_,
            unsigned int population_size_,
            unsigned int threads_number_
    );

    void update_fitness_values();
    void update_fitness_values_threaded();
    void find_max();
    void iterate_until(unsigned long numb_itr, bool parallel);
};

#endif //GENETICALGORITHM_H
