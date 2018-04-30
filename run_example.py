from geneticalgorithm_example import *
target = 371
p_count = 100
i_length = 6
i_min = 0
i_max = 100
p = population(p_count, i_length, i_min, i_max)
fitness_history = [grade(p, target),]
for i in range(100):
    p = evolve(p, target)
    fitness_history.append(grade(p, target))

for info in fitness_history:
    print(info)
