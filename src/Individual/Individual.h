#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include <vector>
#include <functional>
#include <utility>
#include <cmath>
#include <iostream>

using namespace std;

class Individual {
    // chromosomes BITS
    vector<bool> chromosomes;
    unsigned int chromosome_length;

    // indexes of steps
    vector<double> values;

public:
    // Probability
//    double selection_probability;
    // fitness
    double fitness_value;

    Individual(
            unsigned int number_of_args_,
            unsigned int chromosome_length_
    );

    void update_values(long double &step, double &lower_bound);

    vector<double> &get_values();
    void set_fitness_value(double val);
    double get_fitness_value() const;
//    void update_probability(const double & sum);
    void set(unsigned long i, bool val);
    void set(Individual & o, unsigned long i);
    void flip(unsigned long i);
    void swap(unsigned long position1, unsigned long position2);
    unsigned long size();
    void print();
};

#endif //INDIVIDUAL_H
