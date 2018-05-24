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

#define CHROMOSSOME_LENGHT 31
#define RANGE_STANDARD 15

#define P_m 0.05


using namespace std;

auto f = [](long double x) {
    return (x * sin(4.0 * x) + (1.1) * x * cos(2.1 * x));
};

long long lowerBound = (-1) * pow(2, RANGE_STANDARD) - 1;
long long upperBound = pow(2, RANGE_STANDARD) - 1;

double step = 2 * (pow(2, RANGE_STANDARD) - 1) / double(pow(2.0, CHROMOSSOME_LENGHT) - 1);
int threshold = CHROMOSSOME_LENGHT * CHROMOSSOME_LENGHT - 1;

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
    bitset<CHROMOSSOME_LENGHT> chromossome;
    long double fitness_value;
    double probability_of_selection;

    void set_probability(long double sum_fitness_values, long double min_value);

    void set_fitness_value();
} individuals;

typedef vector <individuals> Tpopulation;

void individuals::set_probability(long double sum_fitness_values, long double min_value) {
    // #cout << "MIN:" << min_value << "Sum:"<< sum_fitness_values << "Fit:" << fitness_value << endl;
    probability_of_selection = 100 * max((fitness_value - min_value), (long double) 0.0) / sum_fitness_values;
}

void individuals::set_fitness_value() {
    long double x = lowerBound + (double) step * chromossome.to_ullong();
    fitness_value = f(x);
}


void update_fitnesses_wrapper(Tpopulation &population, int start, int finish) {
    for(int i=start; i < finish; ++i) {
        population[i].set_fitness_value();
    }
}

void update_probability_wrapper(Tpopulation &population, long double sum_fitness_values, long double min_value, int start, int finish) {
    for(int i=start; i < finish; ++i) {
        population[i].set_probability(sum_fitness_values, min_value);
    }
}

void update_fitness_values(Tpopulation &population) {
    long double min_value = 0.0;
    long long number_of_repetitions = 0;
    long double sum_fitness_values = 0.0;

    // parallelize this

    int threads_num = 25;
    vector <thread> myThreads;
    int cur_ind = 0;
    int step = (population.size() + threads_num - 1) / threads_num;

    for(int i=0; i < threads_num; i++) {
        myThreads.push_back(thread(update_fitnesses_wrapper, std::ref(population), cur_ind, min(cur_ind+step, (int)population.size())));
        cur_ind += step;
    }

    for(int i=0; i < threads_num; i++) {
        myThreads[i].join();
    }


    for (auto &p:population) {
        number_of_repetitions++;
        if (p.fitness_value >= min_value) {
            sum_fitness_values += (p.fitness_value - min_value);
        } else {
            sum_fitness_values += (min_value - p.fitness_value) * (number_of_repetitions-1);
            min_value = p.fitness_value;
        }
    }


    cur_ind = 0;
    myThreads.clear();
    for(int i=0; i < threads_num; i++) {
        myThreads.push_back(thread(update_probability_wrapper, std::ref(population),sum_fitness_values,min_value, cur_ind, min(cur_ind+step, (int)population.size())));
        cur_ind += step;
    }
    // parallelize this
//    for (auto &p:population)
//        p.set_probability(sum_fitness_values, min_value);
    for(int i=0; i < threads_num; i++) {
        myThreads[i].join();
    }
}

Tpopulation generatePopulation(const int amountOfIndividuals) {
    srand(time(NULL));
    Tpopulation population(amountOfIndividuals);

    for (auto &p:population) {
        for (int i = 0; i < p.chromossome.size(); i++) {
            p.chromossome.set(i, rand() % 2);
        }
    }

    update_fitness_values(population);
    return population;
}

//insertion sort
void sortPopulation(Tpopulation &population);

//mutation by swap mutation
void mutation(individuals &individ) {
    int n = individ.chromossome.size();
    while (n > 0) {
        if (((double) rand() / (RAND_MAX)) <= P_m) {
            individ.chromossome.flip(n - 1);
        }
        --n;
    }
}

//Roulette wheel method using the probability
individuals selectIndividual(Tpopulation population) {//select to crossover (natural selection)
    double sum = 0.0;
    // #cout << "Counting ";
    for (auto p:population) {
        // #cout << p.probability_of_selection << " ";
        sum += p.probability_of_selection;
    }
    // #cout << endl << "The sum was:" << sum << endl;
    if ((int) sum == 0) {
        // #cout << "It was zero" << sum << endl;
        sum = 1.0;
    }
    double selectedValue = rand() % 100;
    // #cout << "SelectedValue:"  << selectedValue << ":::"<<(int)sum<<endl;
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
                child_1.chromossome.set(i, ind_2.chromossome[i]);
                child_2.chromossome.set(i, ind_1.chromossome[i]);
            } else {
                child_1.chromossome.set(i, ind_1.chromossome[i]);
                child_2.chromossome.set(i, ind_2.chromossome[i]);
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
    int size_of_population=50;
    // #cout << "Type the population size: ";
//    cin >> size_of_population;
    population = generatePopulation(size_of_population);

    for (auto p:population) {
        long double x = p.chromossome.to_ulong() * step + lowerBound;
        // #cout << "chromossome: " << p.chromossome << " fitness_value: " << p.fitness_value << " probability: "
           //  << p.probability_of_selection << " x = " << x << " f(x) = " << f(x) << endl;
    }

    int numberOfGeneration = 0, generations=100000;
    Tpopulation newPopulation;
    // #cout << "Type number of generation you want: " << endl;
//    cin >> generations;
    // #cout << generations;

    while (numberOfGeneration < generations) {
        population = crossover(population);
        update_fitness_values(population);

        numberOfGeneration++;
    }

    for (auto p:population) {
        long double x = p.chromossome.to_ulong() * step + lowerBound;
        // #cout << "chromossome: " << p.chromossome << " fitness_value: " << p.fitness_value << " probability: "
           //  << p.probability_of_selection << " x = " << x << " f(x) = " << f(x) << endl;
        // #cout << x << " " << f(x) << endl;
    }
    auto time_ = get_current_time_fenced() - time;
    cout << to_us(time_) << endl;

    return 0;
}
