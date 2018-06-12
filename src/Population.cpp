#include "Population.h"
#include <utility>

Population::Population(
        long long elite_number_,
        double crossover_percent_,
        unsigned int number_of_args_,
        function<double(vector<double>)> f_,
        double lower_bound_, double upper_bound_,
        double mutation_probability_,
        unsigned int chromosome_length_,
        unsigned int population_size_
) {
    elite_number = elite_number_;
    crossover_percent = crossover_percent_;
    number_of_args = number_of_args_;
    mutation_probability = mutation_probability_;
    lower_bound = lower_bound_;
    upper_bound = upper_bound_;
    chromosome_length = chromosome_length_;
    step = fabs(upper_bound_ - lower_bound_) / (pow(2.0, chromosome_length_) - 1);
    population_size = population_size_;

    for (auto i = 0; i < population_size_; ++i) {
        individuals.emplace_back(
                f_, number_of_args_, chromosome_length_
        );
    }
}
