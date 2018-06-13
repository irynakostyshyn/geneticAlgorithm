#include "Population.h"




Population::Population(
        long long elite_number_,
        double crossover_percent_,
        double mutation_probability_,
        unsigned int population_size_,
        unsigned int number_of_args_,
        unsigned int chromosome_length_
) {
    mutation = basic_mutation;
    crossover = crossover_with_two_point;
    selection = select_k;
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
    std::sort(individuals.begin(), individuals.end(),
         [this](const Individual &ind1, const Individual &ind2) {
             return ind1.fitness_value > ind2.fitness_value;
         }
    );
}

void Population::process_crossover(bool percent_usage) {
    vector<Individual> new_individuals;

    sort();

    int number_of_pairs;
    if (percent_usage) {
        number_of_pairs = ((int) ((individuals.size() - elite_number) * crossover_percent)) / 2;
    } else {
        number_of_pairs = ((int) (individuals.size() - elite_number)) / 2;
    }
    for(int i = 0; i < elite_number; ++i){
        new_individuals.push_back(individuals[i]);
    }

    for (int i = 0; i < number_of_pairs; ++i) {
        srand(clock());

        Individual ind_1 = selection(individuals);
        Individual ind_2 = selection(individuals);

        Individual child_1{number_of_args, chromosome_length};
        Individual child_2{number_of_args, chromosome_length};

        crossover(ind_1, ind_2, child_1, child_2);

        new_individuals.push_back(child_1);
        new_individuals.push_back(child_2);

    }
    for (int i = 0; i < new_individuals.size(); ++i) {
        if (i + elite_number < individuals.size()) {
            individuals[i + elite_number] = move(new_individuals[i]);
        }
    }
    new_individuals.clear();
}

void Population::process_mutations(bool percent_usage) {

    int number_of_pairs;
    if (percent_usage) {
        number_of_pairs = ((int) ((individuals.size() - elite_number) * crossover_percent)) / 2;
    } else {
        number_of_pairs = 0;
    }

    for (int i = elite_number + number_of_pairs * 2; i < individuals.size(); ++i) {
        mutation(individuals[i], mutation_probability);
    }
}

void Population::print() {
    for (auto &individ : individuals) {
        individ.print();
    }

}

void Population::set_mutation(function<void(Individual &, double)> mutation_) {

    mutation = std::move(mutation_);
}

void Population::set_selection(function<void(Individual &, Individual &, Individual &, Individual &)> crossover_) {
    crossover = std::move(crossover_);
}

void Population::set_crossover(function<Individual(vector<Individual> &)> selection_) {
    selection = std::move(selection_);
}


void basic_mutation(Individual &ind, double mutation_probability) {
    auto n = ind.size();
    while (n > 0) {
        if (((double) rand() / (RAND_MAX)) <= mutation_probability) {
            ind.flip(n - 1);
        }
        --n;
    }
}


void crossover_with_one_point(Individual &par1, Individual &par2, Individual &ch1, Individual &ch2) {
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


bool sort_Pop(Individual ind1, Individual ind2) {
    return ind1.fitness_value > ind2.fitness_value;
}

Individual select_roulette_individual(vector<Individual> &individuals) {
    double sum = 1.0;

    double selectedValue = rand() % (int) sum;
    double aux = 0.0;//used to sum fitness value

//    Individual * i2;
    for (auto &i:individuals) {
        aux += i.selection_probability;
        if (aux >= selectedValue)
            return i;
    }
}

Individual select_k(vector<Individual> &individuals) {
    int k = 2;
    srand(clock());
    vector<Individual> pop;
    for (int r = 0; r < k; r++) {
        auto i = (rand() % individuals.size());
        pop.push_back(individuals[i]);
    }
    sort(pop.begin(), pop.end(), sort_Pop);
    return pop[0];
//    return individuals[0];
}

void crossover_with_two_point(Individual &par1, Individual &par2, Individual &ch1, Individual &ch2) {
    unsigned long position1 = rand() % (par1.size());
    unsigned long position2 = rand() % (par1.size() - position1) + position1;
    for (auto bit_i = 0UL; bit_i < par1.size(); bit_i++) {
        if (bit_i < position1) {
            ch1.set(par2, bit_i);
            ch2.set(par1, bit_i);
        }
        if (bit_i > position2) {
            ch1.set(par2, bit_i);
            ch2.set(par1, bit_i);
        }
    }
}
