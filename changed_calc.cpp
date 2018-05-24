#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <bitset>
#include <vector>
#include <ctime>
#include <cmath>

#define CHROMOSSOME_LENGHT 31
#define RANGE_STANDARD 15 //The range is take in ammount of bits (is 15 forward and 15 backward)
#define FOR(i,s) for(int i=0; i<s;i++)

#define P_m 0.05


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
    void set_probability(long double sumOfFitness_value, long double min_value);
    void set_fitness_value();
}individuals;

typedef vector<individuals> Tpopulation;

void individuals::set_probability(long double sumOfFitness_value, long double min_value){
    probability_of_selection = 100*max((fitness_value-min_value),(long double)0.0)/sumOfFitness_value;
}
void individuals::set_fitness_value(){
    long double x = lowerBound + (double)step*chromossome.to_ullong();
    fitness_value = f(x);
}

void setFitnessValue_forAll(Tpopulation& population){
    long double result_operation = 0.0;
    long long number_of_repetitions = 0;
    long double sumOfFitness_value = 0.0;


    for(auto& p:population){
        p.set_fitness_value();
        number_of_repetitions++;
        if (p.fitness_value >= result_operation) {
            sumOfFitness_value += (p.fitness_value - result_operation);
        }
        else {
            cout << result_operation << endl;

            sumOfFitness_value += (result_operation - p.fitness_value) * number_of_repetitions;
            result_operation = p.fitness_value;
        }
    }


    for (auto& p:population)
        p.set_probability(sumOfFitness_value, result_operation);

//    return population;
}

Tpopulation generatePopulation(const int amountOfIndividuals){
    srand(time(NULL));
    Tpopulation population(amountOfIndividuals);

    for(auto& p:population){
        for(int i = 0;i<p.chromossome.size();i++){
            p.chromossome.set(i, rand()%2);
        }
    }

    setFitnessValue_forAll(population);
    return population;
}

//insertion sort
void sortPopulation(Tpopulation& population);

//mutation by swap mutation
individuals mutation(individuals individ){
    int n = individ.chromossome.size();
    while (n > 0) {
        if(((double)rand() / (RAND_MAX)) <= P_m) {
            individ.chromossome.flip(n-1);
        }
        --n;
    }
    return individ;
}

//Roulette wheel method using the probability
individuals selectIndividual(Tpopulation population){//select to crossover (natural selection)
    double sum = 0.0;
    for(auto p:population){
        sum += p.probability_of_selection;
    }
    if((int)sum == 0) {
        cout << "It was zero" << sum << endl;
        sum = 1.0;
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
    Tpopulation new_population;
    int numberOfCrosses = population.size()/2;
    while(numberOfCrosses>0){
        numberOfCrosses--;
        srand(clock());
        int position1 = rand()%CHROMOSSOME_LENGHT;

        individuals ind_1 = selectIndividual(population);
        individuals ind_2 = selectIndividual(population);

        individuals child_1;
        individuals child_2;

        for(int i = 0; i<ind_1.chromossome.size();i++){
            if(i>position1){
                child_1.chromossome.set(i,ind_2.chromossome[i]);
                child_2.chromossome.set(i,ind_1.chromossome[i]);
            }
            else{
                child_1.chromossome.set(i,ind_1.chromossome[i]);
                child_2.chromossome.set(i,ind_2.chromossome[i]);
            }
        }
        child_1 = mutation(child_1);
        child_2 = mutation(child_2);


        new_population.push_back(child_1);
        new_population.push_back(child_2);
    }
    return new_population;
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

    for(auto p:population){
        long double x = p.chromossome.to_ulong()*step+lowerBound;
        cout<<"chromossome: "<<p.chromossome<<" fitness_value: "<<p.fitness_value<<" probability: "<<p.probability_of_selection<<" x = "<<x<<" f(x) = "<<f(x)<<endl;
    }

    int numberOfGeneration = 0, generations;
    Tpopulation newPopulation;
    cout<<"Type number of generation you want: "<<endl;
    cin >> generations;
    cout << generations;

    while(numberOfGeneration<generations){
//        cout << "A"<<population.size() << endl;
        population = crossover(population);
//        cout << "B"<<population.size() << endl;
        setFitnessValue_forAll(population);

        numberOfGeneration++;
    }

    for(auto p:population){
        long double x = p.chromossome.to_ulong()*step+lowerBound;
        cout<<"chromossome: "<<p.chromossome<<" fitness_value: "<<p.fitness_value<<" probability: "<<p.probability_of_selection<<" x = "<<x<<" f(x) = "<<f(x)<<endl;
        cout<<x<<" "<<f(x)<<endl;
    }

    return 0;
}
