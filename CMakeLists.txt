cmake_minimum_required (VERSION 3.9)

#set(CMAKE_CXX_COMPILER:FILEPATH /usr/bin/g++-7)
#set(CMAKE_CXX_FLAGS:STRING -std=gnu++17)
#set(CMAKE_C_COMPILER:FILEPATH /usr/bin/gcc-7)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project (geneticAlgorithm)
find_package(Threads)



add_executable(genetic src/main.cpp src/Population.cpp src/Population.h src/Individual.cpp src/Individual.h src/GeneticAlgorithm.cpp src/GeneticAlgorithm.h src/ThreadedFunctions.cpp src/ThreadedFunctions.h)
target_link_libraries( genetic ${CMAKE_THREAD_LIBS_INIT} )
