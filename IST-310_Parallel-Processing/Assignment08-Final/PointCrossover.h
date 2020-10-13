#pragma once

#include <random>
#include "Operator.h"

/**
 * Subclass of Operator, used to perform single point crossover on the genetic
 * algorithm's population. 
 *
 * \author Mark M. Meysenburg
 * \version 3/13/2020
 */
class PointCrossover : public Operator
{
public:
	/**
	 * Constructor. Create the PointCrossover Operator, composing it with the 
	 * Problem being solved, the probability of performing a crossover on an
	 * individual, the population size, and the pseudo-random number generator
	 * being used in the genetic algorithm.
	 *
	 * \param pProb Pointer to the Problem being solved.
	 * \param probXOver Probability of performing a crossover operation on an
	 *			individual, in [0, 1].
	 * \param popSize Population size
	 * \param pRNG Pseudo-random number generator being used by the genetic
	 *			algorithm.
	 */
	PointCrossover(Problem* pProb, double probXOver, int popSize,
		std::mt19937* pRNG) : Operator(pProb, popSize), chi(probXOver),
		pPRNG(pRNG)
	{
		// allocate the distribution used to pick mates
		pMateDist = new std::uniform_int_distribution<int>(0, popSize - 1);
	}

	/**
	 * Destructor. Free the memory used by the mate-picking distribution. 
	 */
	~PointCrossover() { if (pMateDist != 0) delete pMateDist; }

	/**
	 * Perform the crossover. Each individual in the population undergoes 
	 * crossover with probability chi. If doing crossover, randomly pick a mate
	 * and a crossover point, then swap the genes between the two after that 
	 * point. 
	 *
	 * \param ppPop Pointer to a two-dimensional array of doubles,
	 *          representing the individuals in the genetic algorithm
	 *          population.
	 */
	void operate(double** ppPop);

private:
	/** Probability of undergoing crossover. */
	double chi;

	/** Pseudo-random number generator used in the genetic algorithm. */
	std::mt19937* pPRNG;

	/** Uniform distribution used to select mates. */
	std::uniform_int_distribution<int>* pMateDist;
};