
#include "Individual.h"

Individual::Individual(unsigned int number_of_args_,
                       unsigned int chromosome_length_) {
    srand(clock());
    chromosome_length = chromosome_length_;
    for (auto i = 0; i < chromosome_length_ * number_of_args_; ++i) {
        chromosomes.emplace_back(rand() % 2);
    }
    //update_values();
}

void Individual::update_values() {
    values = vector<double>(chromosomes.size() / chromosome_length);
    int number;
    int power;
    int current_ind = 0;
    for(unsigned int i=0, j=chromosome_length; j<=chromosomes.size(); j+=chromosome_length, i+=chromosome_length) {
        number = 0;
        power = 0;
        for(unsigned int current_bit = i; current_bit < j; current_bit++) {
            if(chromosomes[current_bit]) {
                number += pow(2, power);
            }
            ++power;
        }
        values[current_ind] = number;
        current_ind++;
    };
}

vector<double> & Individual::get_values() {
    return values;
}

void Individual::set_fitness_value(double val) {
    fitness_value = val;
}

void Individual::update_probability(const double & sum) {
    selection_probability = fitness_value / sum;
}

double Individual::get_fitness_value() const {
    return fitness_value;
}

void Individual::set(unsigned long i, bool val) {
    chromosomes[i] = val;
}

void Individual::set(Individual &o, unsigned long i) {
    chromosomes[i] = o.chromosomes[i];
}

void Individual::flip(unsigned long i) {
    chromosomes[i] = 1 - chromosomes[i];
}

unsigned long Individual::size() {
    return chromosomes.size();
}

void Individual::print() {
    cout<<"Individ"<<endl;
    for(int i = 0; i<values.size(); i++){
        cout<<"x"<<i+1<<": "<<values[i]<<"  ";
    }
    cout<<"\nFitness value: "<< fitness_value<<endl;
}

void Individual::swap(unsigned long position1, unsigned long position2) {
    auto sw = chromosomes[position1];
    chromosomes[position1] = chromosomes[position2];
    chromosomes[position2] = sw;
}
