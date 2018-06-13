#include <iostream>

#include "GeneticAlgorithm/GeneticAlgorithm.h"

#define PI 3.14159265

using namespace std;

double f3(vector<double> args) {
    return 20 * exp(-0.2 * sqrt(0.5 * (pow(args[0], 2) + pow(args[1], 2)))) -
           exp(0.5 * (cos(2 * PI * args[1]) + cos(2 * PI * args[0]))) + 20 + exp(1);
}

double f2(vector<double > args){
    return exp(-(pow((args[0]-PI),2)+pow((args[1]-PI),2)));
}
double f5(vector<double > args){
    double x = args[0];
    double y= args[1];
    return pow((x*x + y - 11), 2) + pow((x + y*y - 7), 2);
//    return 1.0/(1.0+pow((x*x + y - 11), 2) + pow((x + y*y - 7), 2));
}

int main() {
    GeneticAlgorithm geneticAlgorithm(10, 0.6, 2, f2, -100, 100, 0.05, 24, 100, 4);
    //geneticAlgorithm.update_fitness_values();
    geneticAlgorithm.iterate_until(55);
    geneticAlgorithm.find_max();
}
