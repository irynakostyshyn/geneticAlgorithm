cmake_minimum_required (VERSION 3.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project (geneticAlgorithm)
add_executable(genetic src/Population.cpp src/Population.h src/Individual.cpp src/Individual.h src/main_new.cpp src/GeneticAlgorithm.cpp src/GeneticAlgorithm.h)
