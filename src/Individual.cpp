#include "Individual.h"

Individual::Individual(function<double(vector<double>)> f_, unsigned int number_of_args_,
                       unsigned int chromosome_length_) {
    srand(time(NULL));
    f = std::move(f_);
    chromosome_length = chromosome_length_;
    for (auto i = 0; i < chromosome_length_ * number_of_args_; ++i) {
        chromosomes.emplace_back(rand() % 2);
    }
    update_values();
}

void Individual::update_values() {
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
