#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <bitset>
#include <vector>
#include <ctime>
#include <cmath>

#define PI 3.14159265
#define CHROMOSSOME_LENGHT 31
#define RANGE_STANDARD 15 //The range is take in ammount of bits (is 15 forward and 15 backward)
#define FOR(i,s) for(int i=0; i<s;i++)

using namespace std;

auto f=[](long double x){
    return (x*sin(4.0*x)+(1.1)*x*cos(2.1*x));
};

long long lowerBound = (-1)*pow(2,RANGE_STANDARD)-1;
long long upperBound = pow(2,RANGE_STANDARD)-1;

double step = 2*(pow(2,RANGE_STANDARD)-1)/double(pow(2.0,CHROMOSSOME_LENGHT)-1);
int threshold = CHROMOSSOME_LENGHT*CHROMOSSOME_LENGHT-1;

typedef struct individ{
    bitset<CHROMOSSOME_LENGHT> chromossome;
    long double fitness_value;
    double probability_of_selection;
    void set_probability(long double sumOfFitness_value);
    void set_fitness_value(long double increase);
}individuals;

typedef vector<individuals> Tpopulation;

void individuals::set_probability(long double sumOfFitness_value){
    long double x = lowerBound + (double)step*chromossome.to_ullong();
    probability_of_selection = 100*fitness_value/sumOfFitness_value;
}
void individuals::set_fitness_value(long double increase){
    long double x = lowerBound + (double)step*chromossome.to_ullong();
    fitness_value = (-1)*f(x) + increase;
}

void setFitnessValue_forAll(Tpopulation& population){
    long double smaller = upperBound;
    long double result_operation;
    long double sumOfFitness_value = 0.0;

    for(auto ind:population){
        result_operation = (-1)*f(lowerBound + (double)step*ind.chromossome.to_ullong());
        if(smaller>result_operation)
            smaller = result_operation;
    }
    if(smaller<0){
        smaller = smaller*(-2);
        for(auto& p:population){
            p.set_fitness_value(smaller);
            sumOfFitness_value += p.fitness_value;
        }
    }
    else{
        smaller = 0.0;
        for(auto& p:population){
            p.set_fitness_value(smaller);
            sumOfFitness_value += p.fitness_value;
        }
    }

    for (auto& p:population)
        p.set_probability(sumOfFitness_value);

}

Tpopulation generatePopulation(const int amountOfIndividuals){
    srand(time(NULL));
    Tpopulation population(amountOfIndividuals);
    long double sumOfFitness_value = 0.0;
    long double smaller = upperBound;
    long double result_operation;

    for(auto& p:population){
        for(int i = 0;i<p.chromossome.size();i++){
            p.chromossome.set(i, rand()%2);
        }
        result_operation = (-1)*f(lowerBound + (double)step*p.chromossome.to_ullong());
        if(smaller>result_operation)
            smaller = result_operation;
    }

    if(smaller<0){
        smaller = smaller*(-2);
        for(auto& p:population){
            p.set_fitness_value(smaller);
            sumOfFitness_value += p.fitness_value;
        }
    }
    else{
        smaller = 0.0;
        for(auto& p:population){
            p.set_fitness_value(smaller);
            sumOfFitness_value += p.fitness_value;
        }
    }

    for (auto& p:population)
        p.set_probability(sumOfFitness_value);

    return population;
}

//insertion sort
void sortPopulation(Tpopulation& population);

//if 0 number is negative if 1 positive
int bool_to_int(individuals individual){
    unsigned int num;
    bitset<CHROMOSSOME_LENGHT-1> bit;

    FOR(i,CHROMOSSOME_LENGHT-1){
        bit[i] = individual.chromossome[i];
    }
    return (individual.chromossome[CHROMOSSOME_LENGHT-1])?bit.to_ulong():bit.to_ulong()*(-1);
}
//mutation by swap mutation
individuals mutation(individuals individ){
    int val_1, val_2;
    while(true){
        srand(clock());
        val_1 = rand()%individ.chromossome.size();
        srand(clock());
        val_2 = rand()%individ.chromossome.size();
        if(val_1!=val_2) break;
    }
    bool aux = individ.chromossome[val_1];
    individ.chromossome[val_1] = individ.chromossome[val_2];
    individ.chromossome[val_2] = aux;

    return individ;
}

//mutation by flip
individuals mutationByFlip(individuals ind){
    srand(time(NULL));
    ind.chromossome.flip(rand()%ind.chromossome.size());
    ind.chromossome.flip(rand()%ind.chromossome.size());
    return ind;
}

//Roulette wheel method using the probability
individuals selectIndividual(Tpopulation population){//select to crossover (natural selection)
    double sum = 0.0;
    for(auto p:population){
        sum += p.probability_of_selection;
    }
    double selectedValue = rand()%(int)sum;
    double aux = 0.0;//used to sum fitness value

    for(auto p:population){
        aux += p.probability_of_selection;
        if(aux>=selectedValue)
            return p;
    }
}

//Method: Mult Point Crossover
Tpopulation crossover(Tpopulation population){

    int numberOfCrosses = population.size()/2;
    while(numberOfCrosses>=0){
        numberOfCrosses--;
        //srand(time(NULL));
        srand(clock());
        int position1 = rand()%CHROMOSSOME_LENGHT;
        int position2 = rand()%CHROMOSSOME_LENGHT;

        //ensure that position 1 will be bigger than position 2
        if(position1>position2){
            position1 = position1+position2;
            position2 = position1-position2;
            position1 = position1-position2;
        }

        individuals ind_1 = selectIndividual(population);
        individuals ind_2 = selectIndividual(population);

        individuals child_1;
        individuals child_2;

        for(int i = 0; i<CHROMOSSOME_LENGHT;i++){
            if(i>=position1 && i<position2){
                child_1.chromossome.set(i,ind_2.chromossome[i]);
                child_2.chromossome.set(i,ind_1.chromossome[i]);
            }
            else{
                child_1.chromossome.set(i,ind_1.chromossome[i]);
                child_2.chromossome.set(i,ind_2.chromossome[i]);
            }
        }
        child_1 = mutationByFlip(child_1);
        population.push_back(child_1);
        population.push_back(child_2);
    }
    return population;
}

void naturalSelection(Tpopulation& population){

    long double sumOfFitness_value = 0.0;
    setFitnessValue_forAll(population);

    sortPopulation(population);

    int numberOfKills = population.size()/2;
    while(numberOfKills>=0){
        numberOfKills--;
        population.pop_back();
    }

}
int main(int argc, char *argv[]){

    if(argc == 3){
        lowerBound = atoll(argv[1]);
        upperBound = atoll(argv[2]);
        step = abs(upperBound-lowerBound)/double(pow(2.0,CHROMOSSOME_LENGHT)-1);
    }

    Tpopulation population;
    int size_of_population;
    cout<<"Type the population size: ";
    cin >> size_of_population;
    population = generatePopulation(size_of_population);
    sortPopulation(population);
    for(auto p:population){
        long double x = p.chromossome.to_ulong()*step+lowerBound;
        cout<<"chromossome: "<<p.chromossome<<" fitness_value: "<<p.fitness_value<<" probability: "<<p.probability_of_selection<<" x = "<<x<<" f(x) = "<<f(x)<<endl;
    }

    int numberOfGeneration = 0, generations;
    Tpopulation newPopulation;
    cout<<"Type number of generation you want: "<<endl;
    cin>>generations;
    while(numberOfGeneration<generations){
        newPopulation = crossover(population);
        naturalSelection(newPopulation);
        population = newPopulation;
        numberOfGeneration++;
    }

    ofstream result("result.dat");
    cout<<"Result: "<<endl;
    for(auto p:population){
        long double x = p.chromossome.to_ulong()*step+lowerBound;
        cout<<"chromossome: "<<p.chromossome<<" fitness_value: "<<p.fitness_value<<" probability: "<<p.probability_of_selection<<" x = "<<x<<" f(x) = "<<f(x)<<endl;
        result<<x<<" "<<f(x)<<endl;
    }
    result.close();
    system("gnuplot -p chart.gnu");
    return 0;
}

void sortPopulation(Tpopulation& population){

    for (int i = 1; i < population.size(); i++){
        individuals value = population[i];
        int j = i - 1;

        while ((j >= 0) && (population[j].fitness_value < value.fitness_value)) {
            population[j + 1] = population[j];
            j--;
        }

        population[j + 1] = value;
    }
}