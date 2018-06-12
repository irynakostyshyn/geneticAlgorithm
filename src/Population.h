#ifndef POPULATION_H
#define POPULATION_H


#include <vector>
#include <functional>
#include <iostream>


#include "Individual.h"

using namespace std;

class Population {
    vector<Individual> individuals;

    // working params
    long long elite_number;
    double crossover_percent;
    double mutation_probability;
    unsigned int number_of_args;
    unsigned int chromosome_length;
public:
    Population(
            long long elite_number_,
            double crossover_percent_,
            double mutation_probability_,
            unsigned int population_size_,
            unsigned int number_of_args_,
            unsigned int chromosome_length_
    );
    vector<Individual>& get_individuals();
    void update_probability(const double & sum);
    void sort();
    void process_crossover();
    void process_mutations();
    void mutation(Individual &i);
    Individual & select_roulette_individual();
};


#endif //POPULATION_H
