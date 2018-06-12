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
    mutation = basic_mutation;
    crossover = crossover_with_one_point;
    elite_number = elite_number_;
    crossover_percent = crossover_percent_;
    mutation_probability = mutation_probability_;
    number_of_args = number_of_args_;
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

void Population::process_crossover(bool percent_usage) {
    vector<Individual> new_individuals;

    int number_of_pairs;
    if (percent_usage) {
        number_of_pairs = ((int) ((individuals.size() - elite_number) * crossover_percent)) / 2;
    } else {
        number_of_pairs = ((int) (individuals.size() - elite_number)) / 2;
    }



    for (int i = 0; i < number_of_pairs; ++i) {
        srand(clock());

        Individual ind_1 = select_roulette_individual();
        Individual ind_2 = select_roulette_individual();

        Individual child_1{number_of_args, chromosome_length};
        Individual child_2{number_of_args, chromosome_length};

        crossover(ind_1, ind_2, child_1, child_2);

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

//    Individual * i2;
    for (auto & i:individuals) {
        aux += i.selection_probability;
        if (aux >= selectedValue)
            return i;
//        i2 = &i;
    }
//    return *i2;
}

void Population::process_mutations(bool percent_usage) {

    int number_of_pairs;
    if (percent_usage) {
        number_of_pairs = ((int) ((individuals.size() - elite_number) * crossover_percent)) / 2;
    } else {
        number_of_pairs = 0;
    }

    for(int i=elite_number + number_of_pairs * 2; i<individuals.size(); ++i) {
        mutation(individuals[i], mutation_probability);
    }
}


void basic_mutation(Individual &ind, double mutation_probability) {
    auto n = ind.size();
    while (n > 0) {
        if (((double) rand() / (RAND_MAX)) <= mutation_probability) {
            ind.flip(n-1);
        }
        --n;
    }
}


void crossover_with_one_point(Individual &par1, Individual &par2, Individual &ch1, Individual &ch2){
    unsigned long position1 = rand() % (par1.size());
    for (auto bit_i = 0UL; bit_i < par1.size(); bit_i++) {
        if (bit_i > position1) {
            ch1.set(par2, bit_i);
            ch2.set(par1, bit_i);
        } else {
            ch1.set(par1, bit_i);
            ch2.set(par2, bit_i);
        }
    }
}

