#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include <vector>
#include <functional>
#include <utility>
#include <cmath>

using namespace std;

class Individual {
    // chromosomes BITS
    vector<bool> chromosomes;
    unsigned int chromosome_length;

    // indexes of steps
    vector<double> values;



public:
    // Probability
    double selection_probability;
    // fitness
    double fitness_value;

    Individual(
            unsigned int number_of_args_,
            unsigned int chromosome_length_
    );

    void update_values();

    vector<double> &get_values();
    void set_fitness_value(double val);
    double get_fitness_value() const;
    void update_probability(const double & sum);
    void set(unsigned long i, bool val);
    void set(Individual & o, unsigned long i);
    void flip(unsigned long i);
};

#endif //INDIVIDUAL_H
