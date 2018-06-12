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

    // function to fit
    function<double(vector<double>)> f;
public:
    Individual(
            function<double(vector<double>)> f_,
            unsigned int number_of_args_,
            unsigned int chromosome_length_
    );

    void update_values();
};

#endif //INDIVIDUAL_H
