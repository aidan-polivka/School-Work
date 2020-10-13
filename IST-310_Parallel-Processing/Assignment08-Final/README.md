# ist310-ga: multi-processor GA code for IST 310

 The purpose of this assignment was to convert a uni-processor Genetic Algorith
 to a parallelized genetic algorithm. The files written by me are Ackley.cpp and 
 DeJong.cpp.

# This repo contains the multiprocessor genetic algorithm code for the spring 2020
# IST 310 course.

Directory contents:

- **Ackley.cpp, Ackley.h** Sample program to solve Ackley's function.

- **DeJong01.cpp, DeJong01.h** Sample program to solve DeJong's first function.

- **Evaluate.cpp, Evaluate.h** Subclass of *Operator* to evaluate the fitness 
of the individuals in the population.

- **Operator.h** Abstract superclass for all genetic operators.

- **PointCrossover.cpp, PointCrossover.h** Subclass of *Operator* implementing
single-point crossover.

- **PointMutation.cpp, PointMutation.h** Subclass of *Operator* implementing 
single-point mutation.

- **Problem.h** Abstract superclass for all sample programs.

- **README.md** This file.

- **StandardStats.cpp, StandardStats.h** Subclass of *Operator* that gathers 
and prints to standard output generation-by-generation statistics for the 
population.

- **TournamentSelection.cpp, TournamentSelection.h** Subclass of *Operator* 
implementing elitist tournament selection.

- **makefile** Makefile to build the samples. Make sure you create the *bin*
subdirectory before executing `make` for the first time. 
