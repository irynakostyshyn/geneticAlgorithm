#include "Population.h"
#include <utility>

Population::Population(
        long long elite_number_,
        double crossover_percent_,
        double mutation_probability_,
        unsigned int population_size_,
        unsigned int number_of_args_,
        unsigned int chromosome_length_
) {
    elite_number = elite_number_;
    crossover_percent = crossover_percent_;
    mutation_probability = mutation_probability_;
    number_of_args = number_of_args_,
    chromosome_length = chromosome_length_;
    for (auto i = 0; i < population_size_; ++i) {
        individuals.emplace_back(
                number_of_args_, chromosome_length_
        );
    }
}

vector<Individual> &Population::get_individuals() {
    return individuals;
}

void Population::update_probability(const double &sum) {
    for (auto &individual: individuals) {
        individual.update_probability(sum);
    }
}

void Population::sort() {
    //TODO: Sorting
//    sort(individuals.begin(),
//         individuals.end(), [](auto &&i1, auto &&i2) {
//                return i1.fitness_value < i2.fitness_value;
//            });
}

void Population::process_crossover() {
    vector<Individual> new_individuals;

//    for (int i = 0; i < elite_number; i++) {
//        new_individuals.push_back(individuals[i]);
//    }

    int number_of_pairs = ((int) ((individuals.size() - elite_number) * crossover_percent)) / 2;

    for (int i = 0; i < number_of_pairs; ++i) {
        srand(clock());
        unsigned long position1 = rand() % (number_of_args * chromosome_length);

        Individual ind_1 = select_roulette_individual();
        Individual ind_2 = select_roulette_individual();

        Individual child_1{number_of_args, chromosome_length};
        Individual child_2{number_of_args, chromosome_length};

        for (auto bit_i = 0UL; bit_i < number_of_args * chromosome_length; i++) {
            if (bit_i > position1) {
                child_1.set(ind_2, bit_i);
                child_2.set(ind_1, bit_i);
            } else {
                child_1.set(ind_1, bit_i);
                child_2.set(ind_2, bit_i);
            }
        }
        new_individuals.push_back(child_1);
        new_individuals.push_back(child_2);

    }
    for(int i = 0; i < new_individuals.size(); ++i) {
        if(i + elite_number < individuals.size()) {
            individuals[i + elite_number] = move(new_individuals[i]);
        }
    }
    new_individuals.clear();
}

Individual &Population::select_roulette_individual() {
    double sum = 1.0;

    double selectedValue = rand() % (int) sum;
    double aux = 0.0;//used to sum fitness value

    Individual *i;
    for ((*i):individuals) {
        aux += (*i).selection_probability;
        if (aux >= selectedValue)
            return *i;
    }
    return *i;
}

void Population::process_mutations() {
    int number_of_pairs = ((int) ((individuals.size() - elite_number) * crossover_percent)) / 2;
    for(auto i=elite_number + number_of_pairs * 2; i<individuals.size(); ++i) {
        mutation(individuals[i]);
    }
}

void Population::mutation(Individual &i) {
    int n = chromosome_length * number_of_args;
    while (n > 0) {
        if (((double) rand() / (RAND_MAX)) <= mutation_probability) {
            i.flip(n-1);
        }
        --n;
    }
}
