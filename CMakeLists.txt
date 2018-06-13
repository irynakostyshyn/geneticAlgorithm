cmake_minimum_required(VERSION 3.9)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(geneticAlgorithm)
find_package(Threads)

add_executable(test1 test/test1.cpp src/Population/Population.cpp src/Population/Population.h src/Individual/Individual.cpp src/Individual/Individual.h src/GeneticAlgorithm/GeneticAlgorithm.cpp src/GeneticAlgorithm/GeneticAlgorithm.h src/ThreadedFunctions/ThreadedFunctions.cpp src/ThreadedFunctions/ThreadedFunctions.h)
target_link_libraries(test1 ${CMAKE_THREAD_LIBS_INIT})
