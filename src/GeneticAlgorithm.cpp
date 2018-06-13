#include "GeneticAlgorithm.h"

GeneticAlgorithm::GeneticAlgorithm(
        long long int elite_number_,
        double crossover_percent_,
        unsigned int number_of_args_,
        function<double(vector<double>)> f_,
        double lower_bound_, double upper_bound_,
        double mutation_probability_,
        unsigned int chromosome_length_,
        unsigned int population_size_,
        unsigned int threads_number_
) : population(elite_number_,
               crossover_percent_,
               mutation_probability_,
               population_size_,
               number_of_args_,
               chromosome_length_),
    elite_number(elite_number_),
    crossover_percent(crossover_percent_),
    number_of_args(number_of_args_),
    f(std::move(f_)),
    lower_bound(lower_bound_), upper_bound(upper_bound_),
    mutation_probability(mutation_probability_),
    chromosome_length(chromosome_length_),
    population_size(population_size_),
    threads_number(threads_number_) {
    step = fabs(upper_bound_ - lower_bound_) / (pow(2.0, chromosome_length_) - 1);
}

void GeneticAlgorithm::update_fitness_values() {

    double fitnessesSum = 0.0;
    for(auto & individual : population.get_individuals()) {
        individual.update_values();
        auto values = individual.get_values();
        vector<double> double_values;
        for (double & value : values) {
            double_values.push_back(lower_bound + step * value);
        }
        auto result = f(double_values);
        individual.set_fitness_value(result);
        fitnessesSum += result;
    }
    population.update_probability(fitnessesSum);
}

void GeneticAlgorithm::iterate_until(unsigned long numb_itr) {
    update_fitness_values();
//    update_fitness_values_threaded();
    while (numb_itr > 0) {
        --numb_itr;
        population.process_crossover(true);
        population.process_mutations(false);
        update_fitness_values();
//        update_fitness_values_threaded();
        population.print();

    }
    population.print();


}

void GeneticAlgorithm::update_fitness_values_threaded() {
    double fitnessesSum = fitness_value_threads_divider(population.get_individuals(), f, step, lower_bound,
                                                        threads_number);
    population.update_probability(fitnessesSum);
}

void GeneticAlgorithm::find_max() {
    population.sort();
    population.get_individual();

}

