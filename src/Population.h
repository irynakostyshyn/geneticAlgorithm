#ifndef POPULATION_H
#define POPULATION_H


#include <vector>
#include <functional>
#include <iostream>


#include "Individual.h"

using namespace std;

class Population {
    // individuals
    unsigned int population_size;
    vector<Individual> individuals;
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
public:
    Population(
            long long elite_number_,
            double crossover_percent_,
            unsigned int number_of_args_,
            function<double(vector<double>)> f_,
            double lower_bound_, double upper_bound_,
            double mutation_probability_,
            unsigned int chromosome_length_,
            unsigned int population_size_
    );
};


#endif //POPULATION_H
