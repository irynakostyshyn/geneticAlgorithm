cmake_minimum_required (VERSION 3.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project (geneticAlgorithm)
add_executable(genetic src/main.cpp src/Population.cpp src/Population.h src/Individual.cpp src/Individual.h src/GeneticAlgorithm.cpp src/GeneticAlgorithm.h)
