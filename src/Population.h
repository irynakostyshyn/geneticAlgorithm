#ifndef POPULATION_H
#define POPULATION_H


#include <vector>
#include <functional>
#include <iostream>


#include "Individual.h"

using namespace std;

void basic_mutation(Individual &ind, double mutation_probability);
void crossover_with_one_point(Individual &par1, Individual &par2, Individual &ch1, Individual &ch2);

class Population {
    vector<Individual> individuals;

    // working params
    long long elite_number;
    double crossover_percent;
    double mutation_probability;
    unsigned int number_of_args;
    unsigned int chromosome_length;

    function<void(Individual&, double)> mutation;
    function<void(Individual&, Individual&, Individual&, Individual&)> crossover;

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
    void process_crossover(bool percent_usage);
    void process_mutations(bool percent_usage);
    Individual & select_roulette_individual();
    void print();
};


#endif //POPULATION_H
