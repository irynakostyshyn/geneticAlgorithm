#include <iostream>

#include "GeneticAlgorithm.h"

#define PI 3.14159265

using namespace std;

double f3(vector<double> args) {
    return 20 * exp(-0.2 * sqrt(0.5 * (pow(args[0], 2) + pow(args[1], 2)))) -
           exp(0.5 * (cos(2 * PI * args[1]) + cos(2 * PI * args[0]))) + 20 + exp(1);
}

int main() {
    GeneticAlgorithm geneticAlgorithm(2, 0.8, 2, f3, -100, 100, 0.05, 24, 100);
    //geneticAlgorithm.update_fitness_values();
    geneticAlgorithm.iterate_until(5);
}
