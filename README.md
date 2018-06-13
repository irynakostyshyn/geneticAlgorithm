## Parallel Genetic Algorithm Library


**Description**

This is a simple implementation of the genetic algorithm.
It is created to find `the optimal solution` (maximum of a function).


**What is implemented?**

> [Selection types](https://www.tutorialspoint.com/genetic_algorithms/genetic_algorithms_parent_selection.htm)

1) Roulette wheel selection
2) K-Way tournament selection

> [Mutation](https://www.tutorialspoint.com/genetic_algorithms/genetic_algorithms_mutation.htm)

1) Bit Flip Mutation

> [Crossover](https://www.tutorialspoint.com/genetic_algorithms/genetic_algorithms_crossover.htm)

1) One Point Crossover
2) Multi Point Crossover


**Usage**

```
#include "GeneticAlgorithm.h"

double function(vector<double> args) {
    return 20 * exp(-0.2 * sqrt(0.5 * (pow(args[0], 2) + pow(args[1], 2)))) -
           exp(0.5 * (cos(2 * PI * args[1]) + cos(2 * PI * args[0]))) + 20 + exp(1);
}


int main() {

    long long elite_number = 2;
    double crossover_percent = 0.8;
    unsigned int number_of_args = 2;
    double lower_bound = -100;
    double upper_bound = 100;
    double mutation_probability = 0.05;
    unsigned int chromosome_length = 24;
    unsigned int population_size = 100;
    unsigned int threads_number = 4;

    GeneticAlgorithm geneticAlgorithm(elite_number, crossover_percent, number_of_args,
                                      function, lower_bound, upper_bound, mutation_probability,
                                      chromosome_length, population_size, threads_number);

    geneticAlgorithm.iterate_until(5);
}
```

Example of usage you can find in the directory `tests`.


**Contributors**

Kostiantyn Liepieshov, Iryna Kostyshyn, Victoria Yuzkiv
