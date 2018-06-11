#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <bitset>
#include <atomic>
#include <vector>
#include <ctime>
#include <cmath>
#include <mutex>
#include <thread>
#include <chrono>

#define PI 3.14159265
#define SUB_CHROMOSSOME_LENGTH 20
#define CHROMOSSOME_LENGHT 40
#define RANGE_STANDARD 10

#define P_m 0.05



using namespace std;

auto f = [](long double x) {
    return (x * sin(4.0 * x) + (1.1) * x * cos(2.1 * x));
};

long long lowerBound = (-1) * pow(2, RANGE_STANDARD) - 1;
long long upperBound = pow(2, RANGE_STANDARD) - 1;

double step = 2 * (pow(2, RANGE_STANDARD) - 1) / double(pow(2.0, CHROMOSSOME_LENGHT) - 1);
int threshold = CHROMOSSOME_LENGHT * CHROMOSSOME_LENGHT - 1;

auto f2(double x, double y){ ;
    return pow(pow(x, 2) + pow(y, 2), 0.5);
}

auto f3(double x1, double x2) {
    return -20*exp(-0.2*sqrt(0.5*(pow(x1,2)+pow(x2,2))))-exp(0.5*(cos(2*PI*x2)+cos(2*PI*x1)))+20+exp(1);
}

inline std::chrono::steady_clock::time_point get_current_time_fenced() {
//    assert(std::chrono::steady_clock::is_steady &&
//           "Timer should be steady (monotonic).");
    std::atomic_thread_fence(std::memory_order_seq_cst);
    auto res_time = std::chrono::steady_clock::now();
    std::atomic_thread_fence(std::memory_order_seq_cst);
    return res_time;
}

template<class D>
inline long long to_us(const D &d) {
    return std::chrono::duration_cast<std::chrono::microseconds>(d).count();
}

typedef struct individ {
    individ() : chromossome(CHROMOSSOME_LENGHT) {}
    vector<int> chromossome;
    long double fitness_value;
    double probability_of_selection;

    unsigned long get_value1() {
        int n = 0;
        unsigned long result = 0;
        for(int i=chromossome.size()/2-1; i>-1; i--) {
            if(chromossome[i]) {
                result += (unsigned long)pow(2, n);
            }
            n++;
        }
        return result;
    }

    unsigned long get_value2() {
        int n = 0;
        unsigned long result = 0;
        for(int i=chromossome.size()-1; i>chromossome.size()/2-1; i--) {
            if(chromossome[i]) {
                result += (unsigned long)pow(2, n);
            }
            n++;
        }
        return result;
    }

    void set_probability(long double sumOfFitness_value, long double min_value);

    void set_fitness_value();
} individuals;

typedef vector <individuals> Tpopulation;

void individuals::set_probability(long double sumOfFitness_value, long double min_value) {
    // cout << "MIN:" << min_value << "Sum:"<< sumOfFitness_value << "Fit:" << fitness_value << endl;
    probability_of_selection = 100 * max((fitness_value - min_value), (long double) 0.0) / sumOfFitness_value;
}

void individuals::set_fitness_value() {
    long double x1 = lowerBound + (double) step * get_value1();
    long double x2 = lowerBound + (double) step * get_value2();
    fitness_value = f3(x1, x2);
}

void setFitnessValue_forAll(Tpopulation &population) {
    long double min_value = 0.0;
    long long number_of_repetitions = 0;
    long double sumOfFitness_value = 0.0;

    for (auto &p:population) {
        p.set_fitness_value();
        number_of_repetitions++;
        if (p.fitness_value >= min_value) {
            sumOfFitness_value += (p.fitness_value - min_value);
        } else {
            // cout << "Min val:" << min_value << endl;

            sumOfFitness_value += (min_value - p.fitness_value) * (number_of_repetitions-1);
            min_value = p.fitness_value;
        }
    }


    for (auto &p:population)
        p.set_probability(sumOfFitness_value, min_value);
}

Tpopulation generatePopulation(const int amountOfIndividuals) {
    srand(time(NULL));
    Tpopulation population(amountOfIndividuals);

    for (auto &p:population) {
        for (int i = 0; i < p.chromossome.size(); i++) {
            p.chromossome[i] =rand() % 2;
        }
    }

    setFitnessValue_forAll(population);
    return population;
}

//insertion sort
void sortPopulation(Tpopulation &population);

//mutation by swap mutation
void mutation(individuals &individ) {
    int n = individ.chromossome.size();
    while (n > 0) {
        if (((double) rand() / (RAND_MAX)) <= P_m) {
            individ.chromossome[n - 1] = 1 - individ.chromossome[n-1];
        }
        --n;
    }
}

//Roulette wheel method using the probability
individuals selectIndividual(Tpopulation population) {//select to crossover (natural selection)
    double sum = 0.0;
    // cout << "Counting ";
    for (auto p:population) {
        // cout << p.probability_of_selection << " ";
        sum += p.probability_of_selection;
    }
    // cout << endl << "The sum was:" << sum << endl;
    if ((int) sum == 0) {
        // cout << "It was zero" << sum << endl;
        sum = 1.0;
    }
    double selectedValue = rand() % 100;
    // cout << "SelectedValue:"  << selectedValue << ":::"<<(int)sum<<endl;
    double aux = 0.0;//used to sum fitness value

    for (auto p:population) {
        aux += p.probability_of_selection;
        if (aux >= selectedValue)
            return p;
    }
}

//Method: Mult Point Crossover
Tpopulation crossover(Tpopulation &population) {
    Tpopulation new_population;
    int numberOfCrosses = population.size() / 2;
    while (numberOfCrosses > 0) {
        numberOfCrosses--;
        srand(clock());
        int position1 = rand() % CHROMOSSOME_LENGHT;

        individuals ind_1 = selectIndividual(population);
        individuals ind_2 = selectIndividual(population);

        individuals child_1;
        individuals child_2;

        for (int i = 0; i < ind_1.chromossome.size(); i++) {
            if (i > position1) {
                child_1.chromossome[i] = ind_2.chromossome[i];
                child_2.chromossome[i] = ind_1.chromossome[i];
            } else {
                child_1.chromossome[i] = ind_1.chromossome[i];
                child_2.chromossome[i] = ind_2.chromossome[i];
            }
        }
        mutation(child_1);
        mutation(child_2);


        new_population.push_back(child_1);
        new_population.push_back(child_2);
    }
    return new_population;
}

int main(int argc, char *argv[]) {

    if (argc == 3) {
        lowerBound = atoll(argv[1]);
        upperBound = atoll(argv[2]);
        step = abs(upperBound - lowerBound) / double(pow(2.0, CHROMOSSOME_LENGHT) - 1);
    }
    auto time = get_current_time_fenced();


    Tpopulation population;

    int size_of_population=100;

    // cout << "Type the population size: ";
//    cin >> size_of_population;
    population = generatePopulation(size_of_population);

    for (auto p:population) {
//        long double x = p.get_value() * step + lowerBound;
        // cout << "chromossome: " << p.chromossome << " fitness_value: " << p.fitness_value << " probability: "
          //   << p.probability_of_selection << " x = " << x << " f(x) = " << f(x) << endl;
    }

    int numberOfGeneration = 0, generations=10;
    Tpopulation newPopulation;
    // cout << "Type number of generation you want: " << endl;
//    cin >> generations;
    // cout << generations;

    while (numberOfGeneration < generations) {
        population = crossover(population);
        setFitnessValue_forAll(population);

        numberOfGeneration++;
    }

    for (auto p:population) {
        long double x1 = p.get_value1() * step + lowerBound;
        long double x2 = p.get_value2() * step + lowerBound;
         cout << " fitness_value: " << p.fitness_value << " probability: "
             << p.probability_of_selection << " x1 = " << x1 << " x2 = " << x2 << " f(x) = " << f3(x1, x2) << endl;
    }
    auto time_ = get_current_time_fenced() - time;
    cout << to_us(time_) << endl;
    return 0;
}
