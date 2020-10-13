#pragma once

#include "Operator.h"

/**
 * Subclass of Operator, used to perform single point mutation on the genetic
 * algorithm's population.
 *
 * \author Mark M. Meysenburg
 * \version 3/13/2020
 */
class PointMutation : public Operator
{
public:
    /**
     * Constructor. Create this PointMutation Operator, composing it with the 
     * Problem being solved, the probability of performing a mutation, and the 
     * population size.
     *
     * \param pProb Pointer to the Problem being solved.
     * \param probMutation Probability of an individual undergoing mutation.
     * \param popSize Population size.
     */
	PointMutation(Problem* pProb, double probMutation, int popSize) : 
		Operator(pProb, popSize), mu(probMutation) { }

    /**
     * Perform the mutation operation. Each individual in the population 
     * undergoes mutation with probability mu. If a mutation happens, choose a
     * point in the individual at random, then replace that gene with a new, 
     * randomly-selected gene value.
     *
     * \param ppPop Pointer to a two-dimensional array of doubles,
     *          representing the individuals in the genetic algorithm
     *          population.
     */
	void operate(double** ppPop);

private:
    /** Probability that an individual in the population undergoes mutation. */
	double mu;
};