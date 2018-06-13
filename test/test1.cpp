#include <iostream>
#include <vector>

#include "../src/GeneticAlgorithm.h"

#define PI 3.14159265

using namespace std;


double f(vector<double > args){
    return exp(-(pow((args[0]-PI),2)+pow((args[1]-PI),2)));
}


int main() {
    GeneticAlgorithm geneticAlgorithm(10, 0.6, 2, f, -100, 100, 0.05, 24, 100, 4);
    //geneticAlgorithm.update_fitness_values();
    geneticAlgorithm.iterate_until(55);
}
